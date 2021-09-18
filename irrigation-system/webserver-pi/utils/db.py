import logging
import uuid
from dataclasses import dataclass
from datetime import datetime, timedelta
from typing import Any, Callable, Optional

import pytz
from mysql.connector import pooling


@dataclass
class ValveId:
    value: str


@dataclass
class ValveBoxId:
    value: str


@dataclass
class LastOpened:
    valve_id: ValveId
    last_closed: datetime


@dataclass
class Valve:
    valve_id: ValveId
    valve_position: int


# Create Connection Pool
mysql_user = 'irsys'
mysql_password = 'Waterme1!@#'
mysql_db = 'irsys'

fetchall = lambda c: c.fetchall()
fetchone = lambda c: c.fetchone()
donothing = lambda c: ()

"""Creates and returns a Connection Pool"""


def create_connection_pool():
    # Create Connection Pool
    # connection = mariadb.connect(user=mysql_user, password=mysql_password, database=mysql_db)
    pool = pooling.MySQLConnectionPool(
        user=mysql_user,
        password=mysql_password,
        database=mysql_db,
        pool_name="web-app",
        pool_size=20
    )

    # Return Connection Pool
    return pool


def execute(sql: str,
            fetch: Optional[Callable],
            values=None):
    try:
        connection = pool.get_connection()

        # logging.debug(sql)
        # logging.debug(values)
        cursor = connection.cursor()
        cursor.execute(sql, values)
        if values is not None:
            connection.commit()
        result = fetch(cursor)
        try:
            connection.close()
        except Exception as e:
            message = e.message if hasattr(e, 'message') else e
            logging.error("Could not close connection:")
            logging.error(f"{message}")
        return result
    except Exception as e:
        message = e.message if hasattr(e, 'message') else e

        logging.error(f'SQL Error in query {sql}: \n{message}')
        raise Exception("SQL error")


def get_sensor_data(field, low_timestamp_seconds, high_timestamp_seconds, time_bucket_size_seconds):
    data = get_sensor_data_from_db(field, low_timestamp_seconds, high_timestamp_seconds, time_bucket_size_seconds)

    return list(map(lambda row: {
        "deviceId": row[0],
        field: float(row[1]),
        "timestamp_bucket": row[2] * time_bucket_size_seconds
    }, data))


def get_latest_sensor_data():
    query = """
        select t1.deviceId, t1.time, t1.owner, t1.type, t1.value
        from sensor_values t1
        join (
            SELECT type, MAX(time) AS time
            FROM sensor_values
            GROUP BY type
        ) t2 on t1.time = t2.time and t1.type = t2.type
        ORDER BY time DESC, type"""

    result = execute(query, fetchall)

    return list(map(lambda row: {
        "deviceId": row[0],
        "time": row[1],
        "owner": row[2],
        "type": row[3],
        "value": row[4]
    }, result))


def get_valve_openings():
    query = """
        SELECT valve_id, starttime, endtime, opening_time
        FROM open_times
        ORDER BY starttime DESC
        LIMIT 5"""

    result = execute(query, fetchall)

    return list(map(lambda row: {
        "valve_id": row[0],
        "starttime": row[1],
        "endtime": row[2],
        "opening_time": row[2]
    }, result))


def get_sensor_data_from_db(field, low_timestamp_seconds, high_timestamp_seconds, time_bucket_size_seconds):
    query = """
            SELECT DISTINCT(deviceId) AS deviceId, AVG(value) AS avg_{field}, FLOOR(UNIX_TIMESTAMP(time)/{time_bucket_size_seconds}) AS time_bucket
            FROM sensor_values
            WHERE UNIX_TIMESTAMP(time) >= {low_timestamp_seconds} AND UNIX_TIMESTAMP(time) <= {high_timestamp_seconds}
              AND `type` = "{field}"
            GROUP BY FLOOR(UNIX_TIMESTAMP(time)/{time_bucket_size_seconds}), deviceId
            ORDER BY time_bucket""".format(
        time_bucket_size_seconds=time_bucket_size_seconds,
        low_timestamp_seconds=low_timestamp_seconds,
        high_timestamp_seconds=high_timestamp_seconds,
        field=field)
    # print(query)
    result = execute(query, fetchall)
    return result


def get_wifi_credentials():
    query = """
        SELECT ssid, password
        FROM wifi"""
    cursor = execute(query, fetch=lambda c: c.fetchone())

    row = cursor.fetchone()
    return {
        "ssid": row[0],
        "password": row[1]
    }


def put_wifi_credentials(ssid, password):
    values = {
        "ssid": ssid,
        "password": password
    }

    query = (" INSERT INTO wifi"
             " (ssid, password)"
             " VALUES (%(ssid)s, %(password)s)"
             " ON DUPLICATE KEY UPDATE"
             "    password = %(password)s")
    execute(query, values=values, fetch=donothing)


def put_sensor_value(device_id: str, time: int, owner: str, type: str, value: str):
    values = {
        'id': uuid.uuid4().bytes,
        'deviceId': device_id,
        'time': datetime.fromtimestamp(time).strftime('%Y-%m-%d %H:%M:%S.%f'),
        'owner': owner,
        'type': type,
        'value': value
    }

    query = (" INSERT INTO sensor_values"
             " (id, deviceId, time, owner, type, value)"
             " VALUES (%(id)s, %(deviceId)s, %(time)s, %(owner)s, %(type)s, %(value)s)")

    execute(query, values=values, fetch=donothing)


def write_sensor_association(device_id, time):
    # logging.debug("deviceId", deviceId)
    # logging.debug("time", time)
    values = {
        'id': uuid.uuid4().bytes,
        'deviceId': device_id,
        'time': datetime.fromtimestamp(time).strftime('%Y-%m-%d %H:%M:%S.%f')
    }

    query = (" INSERT INTO sensors"
             " (id, deviceId, time)"
             " VALUES (%(id)s, %(deviceId)s, %(time)s)")
    execute(query, fetch=donothing, values=values)


def get_last_opened(valves: list[Valve]) -> dict[str, LastOpened]:
    valve_id_strs = [x.valve_id.value for x in valves]

    query = (" SELECT valve_id, MAX(endtime) AS last_closed"
             " FROM open_times"
             " WHERE valve_id IN('" + ('\', \''.join(valve_id_strs)) + "')"
                                                                       " GROUP BY valve_id"
             )
    result = execute(query, fetchall)

    return {
        row[0]: LastOpened(
            valve_id=ValveId(row[0]),
            last_closed=pytz.utc.localize(row[1])
        ) for row in result
    }


def average_moisture(valve_ids):
    # logging.info(valve_ids)
    if len(valve_ids) == 0:
        valve_ids = ['']
    format_strings = ','.join(['%s'] * len(valve_ids))
    sql = (""
           " SELECT deviceId, AVG(value) AS avg_value"
           " FROM sensor_values"
           " WHERE time < NOW() - INTERVAL 20 MINUTE"
           "   AND type = 'moisture'"
           "   AND deviceId IN(%s)"
           " GROUP BY deviceId") % format_strings

    result = execute(sql, values=tuple(valve_ids), fetch=fetchall)

    return list(map(lambda row: {
        "deviceId": row[0],
        "avg_value": float(row[1])
    }, result))


def write_opening(valve_id: ValveId, opening_time: timedelta):
    values: dict[str, Any] = {
        'id': uuid.uuid4().bytes,
        'valve_id': valve_id.value,
        'starttime': datetime.now().astimezone(pytz.utc).strftime('%Y-%m-%d %H:%M:%S.%f'),
        'endtime': (datetime.now().astimezone(pytz.utc) + opening_time).strftime('%Y-%m-%d %H:%M:%S.%f'),
        'opening_time': opening_time.total_seconds()
    }

    query = (" INSERT INTO open_times"
             " (id, valve_id, starttime, endtime, opening_time)"
             " VALUES (%(id)s, %(valve_id)s, %(starttime)s, %(endtime)s, %(opening_time)s)")

    execute(query, fetch=fetchall, values=values)


def get_valves_for_device(valve_box_id: ValveBoxId) -> list[Valve]:
    query = (" SELECT valve_id, valve_position"
             " FROM valve_box"
             " WHERE valve_box_id='{valve_box_id}'").format(valve_box_id=valve_box_id.value)
    # logging.debug(query)

    result = execute(query, fetchall)

    return list(map(lambda row: Valve(
        valve_id=ValveId(row[0]),
        valve_position=row[1]
    ), result))


def get_connected_sensors():
    query = """ SELECT deviceId
                FROM sensors"""
    result = execute(query, fetchall)

    return list(map(lambda row: {
        "ssid": row[0]
    }, result))


# Init: connect now
pool = create_connection_pool()
