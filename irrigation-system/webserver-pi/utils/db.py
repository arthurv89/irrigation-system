import logging
import mysql.connector as mariadb
from datetime import datetime, timedelta
import uuid
import time
import os
import pytz

def connect():
    connection = mariadb.connect(user='irsys', password='Waterme1!@#', database='irsys')
    connection.reconnect(attempts=1, delay=0)
    return connection

def execute(sql, values=None):
    try:
      # logging.debug(sql)
      # logging.debug(values)
      cursor = connection.cursor(buffered=True)
      cursor.execute(sql, values)
      connection.commit() # To avoid caching the result
    except mariadb.Error as error:
      connect()
      os.system('play -n synth %s sin %s' % (500/1000, 300))
      cursor = connection.cursor(buffered=True)
      cursor.execute(sql, values)
    return cursor

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

    cursor = execute(query)

    return list(map(lambda row: {
        "deviceId": row[0],
        "time": row[1],
        "owner": row[2],
        "type": row[3],
        "value": row[4]
    }, cursor.fetchall()))


def get_valve_openings():
    query = """
        SELECT hose_id, starttime, endtime, opening_time
        FROM open_times
        ORDER BY starttime DESC
        LIMIT 5"""

    cursor = execute(query)

    return list(map(lambda row: {
        "hose_id": row[0],
        "starttime": row[1],
        "endtime": row[2],
        "opening_time": row[2]
    }, cursor.fetchall()))


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
    cursor = execute(query)
    return cursor.fetchall()


def get_wifi_credentials():
    query = """
        SELECT ssid, password
        FROM wifi"""
    cursor = execute(query)

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
    execute(query, values)
    connection.commit()



def put_sensor_value(deviceId, time, owner, type, value):
    values = {
      'id': uuid.uuid4().bytes,
      'deviceId': deviceId,
      'time': datetime.fromtimestamp(time).strftime('%Y-%m-%d %H:%M:%S'),
      'owner': owner,
      'type': type,
      'value': value
    }

    query = (" INSERT INTO sensor_values"
             " (id, deviceId, time, owner, type, value)"
             " VALUES (%(id)s, %(deviceId)s, %(time)s, %(owner)s, %(type)s, %(value)s)")

    execute(query, values)
    connection.commit()


def write_sensor_association(deviceId, time):
    # logging.debug("deviceId", deviceId)
    # logging.debug("time", time)
    values = {
      'id': uuid.uuid4().bytes,
      'deviceId': deviceId,
      'time': datetime.fromtimestamp(time).strftime('%Y-%m-%d %H:%M:%S')
    }

    query = (" INSERT INTO sensors"
             " (id, deviceId, time)"
             " VALUES (%(id)s, %(deviceId)s, %(time)s)")
    execute(query, values)
    connection.commit()


def get_last_opened(hose_ids):
    query = (" SELECT hose_id, MAX(endtime) AS last_closed"
             " FROM open_times"
             " WHERE hose_id IN('" + ('\', \''.join(hose_ids)) + "')"
             " GROUP BY hose_id"
            )
    cursor = execute(query)

    result = cursor.fetchall()
    # logging.debug(result)

    rows = {
        row[0]: {
            "hose_id": row[0],
            "last_closed": pytz.utc.localize(row[1])
        } for row in result
    }
    # logging.debug(rows)

    return {
        hose_id: rows[hose_id] if hose_id in rows else None
        for hose_id in hose_ids
    }


def average_moisture(hose_ids):
    # logging.info(hose_ids)
    if len(hose_ids) == 0:
        hose_ids = ['']
    format_strings = ','.join(['%s'] * len(hose_ids))
    sql = (""
            " SELECT deviceId, AVG(value) AS avg_value"
            " FROM sensor_values"
            " WHERE time < NOW() - INTERVAL 20 MINUTE"
            "   AND type = 'moisture'"
            "   AND deviceId IN(%s)"
            " GROUP BY deviceId") % format_strings

    cursor = execute(sql,
                tuple(hose_ids))

    return list(map(lambda row: {
        "deviceId": row[0],
        "avg_value": float(row[1])
    }, cursor.fetchall()))


def write_opening(hose_id, opening_time):
    values = {
      'id': uuid.uuid4().bytes,
      'hose_id': hose_id,
      'starttime': datetime.now().astimezone(pytz.utc).strftime('%Y-%m-%d %H:%M:%S'),
      'endtime': (datetime.now().astimezone(pytz.utc) + timedelta(0, opening_time)).strftime('%Y-%m-%d %H:%M:%S'),
      'opening_time': opening_time
    }

    query = (" INSERT INTO open_times"
             " (id, hose_id, starttime, endtime, opening_time)"
             " VALUES (%(id)s, %(hose_id)s, %(starttime)s, %(endtime)s, %(opening_time)s)")

    execute(query, values)
    connection.commit()

def get_hoses_for_valve(valveId):
    query = (" SELECT hose_id, hose_position"
             " FROM valve_connector"
             " WHERE valve_id='{valveId}'").format(valveId=valveId)
    # logging.debug(query)

    cursor = execute(query)

    return list(map(lambda row: {
        "hose_id": row[0],
        "hose_position": row[1]
    }, cursor.fetchall()))


def get_connected_sensors():
    query = """ SELECT deviceId
                FROM sensors"""
    cursor = execute(query)

    return list(map(lambda row: {
        "ssid": row[0]
    }, cursor.fetchall()))


# Init: connect now
connection = connect()
