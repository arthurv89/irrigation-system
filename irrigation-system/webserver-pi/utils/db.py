import mysql.connector as mariadb
from datetime import datetime
import uuid
import time

connection = mariadb.connect(user='irsys', password='Waterme1', database='irsys')
connection.reconnect(attempts=1, delay=0)

cursor = connection.cursor(buffered=True)

# time_bucket_size = 300 # seconds
time_bucket_size = 60 * 30 # seconds

def get_moisture_values_per_device_per_hour(low_timestamp, high_timestamp):
    query = """
SELECT DISTINCT(deviceId) AS deviceId, AVG(moisture) AS avg_moisture, FLOOR(UNIX_TIMESTAMP(time)/1800) AS time_bucket
FROM moisture
WHERE UNIX_TIMESTAMP(time) >= {low_timestamp} AND UNIX_TIMESTAMP(time) <= {high_timestamp}
GROUP BY FLOOR(UNIX_TIMESTAMP(time)/1800), deviceId
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
    values = {
      'id': uuid.uuid4().bytes,
      'deviceId': deviceId,
      'time': datetime.fromtimestamp(time).strftime('%Y-%m-%d %H:%M:%S')
    }

    query = ("INSERT INTO sensors "
              "(id, deviceId, time) "
              "VALUES (%(id)s, %(deviceId)s, %(time)s)")

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
