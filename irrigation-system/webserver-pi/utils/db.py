import logging
import mysql.connector as mariadb
from datetime import datetime
import uuid
import time
import os

def connect():
    connection = mariadb.connect(user='irsys', password='Waterme1', database='irsys')
    connection.reconnect(attempts=1, delay=0)
    return connection

def execute(sql, values=None):
    try:
      # logging.debug(sql)
      # logging.debug(values)
      cursor = connection.cursor(buffered=True)
      cursor.execute(sql, values)
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


def get_recently_opened_hoses():
    query = (" SELECT hose_id"
             " FROM open_times"
             " WHERE time > NOW() - INTERVAL 20 MINUTE"
             " GROUP BY hose_id")
    cursor = execute(query)

    return list(map(lambda row: {
        "hose_id": row[0]
    }, cursor.fetchall()))


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
      'time': datetime.now() .strftime('%Y-%m-%d %H:%M:%S'),
      'opening_time': opening_time
    }

    query = (" INSERT INTO open_times"
             " (id, hose_id, time, opening_time)"
             " VALUES (%(id)s, %(hose_id)s, %(time)s, %(opening_time)s)")

    execute(query, values)
    connection.commit()

def get_hoses_for_valve(valveId):
    query = (" SELECT hose_id, hose_position"
             " FROM valve_connector"
             " WHERE valve_id='{valveId}'").format(valveId=valveId)

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
