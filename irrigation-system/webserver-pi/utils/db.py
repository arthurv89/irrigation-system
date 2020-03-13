import mysql.connector as mariadb
from datetime import datetime
import uuid
import time

connection = mariadb.connect(user='irsys', password='Waterme1', database='irsys')
connection.reconnect(attempts=1, delay=0)

cursor = connection.cursor(buffered=True)

def get_moisture_values_per_device_per_timebucket(low_timestamp, high_timestamp, time_bucket_size):
    query = """
SELECT DISTINCT(deviceId) AS deviceId, AVG(moisture) AS avg_moisture, FLOOR(UNIX_TIMESTAMP(time)/{time_bucket_size}) AS time_bucket
FROM moisture
WHERE UNIX_TIMESTAMP(time) >= {low_timestamp} AND UNIX_TIMESTAMP(time) <= {high_timestamp}
GROUP BY FLOOR(UNIX_TIMESTAMP(time)/{time_bucket_size}), deviceId
""".format(time_bucket_size=time_bucket_size,
           low_timestamp=low_timestamp,
           high_timestamp=high_timestamp)
    print(query)
    res = cursor.execute(query)

    return list(map(lambda row: {
        "deviceId": row[0],
        "moisture": float(row[1]),
        "timestamp_bucket": row[2] * time_bucket_size
    }, cursor.fetchall()))


def get_wifi_credentials():
    query = """
SELECT ssid, password
FROM wifi"""
    # print(query)
    res = cursor.execute(query)

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

    query = ("INSERT INTO wifi "
              "(ssid, password) "
              "VALUES (%(ssid)s, %(password)s) "
              "ON DUPLICATE KEY UPDATE "
              "  password = %(password)s")
    print(query)
    res = cursor.execute(query, values)
    connection.commit()



def put_moisture_value(deviceId, time, owner, moisture):
    values = {
      'id': uuid.uuid4().bytes,
      'deviceId': deviceId,
      'time': datetime.fromtimestamp(time).strftime('%Y-%m-%d %H:%M:%S'),
      'owner': owner,
      'moisture': moisture
    }

    query = ("INSERT INTO moisture "
              "(id, deviceId, time, owner, moisture) "
              "VALUES (%(id)s, %(deviceId)s, %(time)s, %(owner)s, %(moisture)s)")

    cursor.execute(query, values)
    connection.commit()


def write_sensor_association(deviceId, time):
    # print("deviceId", deviceId)
    # print("time", time)
    values = {
      'id': uuid.uuid4().bytes,
      'deviceId': deviceId,
      'time': datetime.fromtimestamp(time).strftime('%Y-%m-%d %H:%M:%S')
    }

    query = ("INSERT INTO sensors "
              "(id, deviceId, time) "
              "VALUES (%(id)s, %(deviceId)s, %(time)s)")
    # print("query", query, values)

    cursor.execute(query, values)
    connection.commit()



def write_opening(valve, opening_time):
    values = {
      'id': uuid.uuid4().bytes,
      'valve': valve,
      'time': datetime.now() .strftime('%Y-%m-%d %H:%M:%S'),
      'opening_time': opening_time
    }

    query = ("INSERT INTO open_times "
              "(id, valve, time, opening_time) "
              "VALUES (%(id)s, %(valve)s, %(time)s, %(opening_time)s)")

    cursor.execute(query, values)
    connection.commit()


def get_connected_sensors():
    query = """
SELECT deviceId
FROM sensors"""
    # print(query)
    res = cursor.execute(query)

    return list(map(lambda row: {
        "ssid": row[0]
    }, cursor.fetchall()))
