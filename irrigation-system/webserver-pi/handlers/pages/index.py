import json
from flask import Flask, render_template
import time

from utils import db
import socket

app = Flask(__name__, template_folder="jinja_templates")

one_day = 86400 # seconds
time_bucket_size = 5 * 60 # seconds

def handle():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    address = s.getsockname()[0]
    s.close()

    latest_sensor_values = db.get_latest_sensor_data()
    valve_openings = db.get_valve_openings()

    timeseries_arr = [
        timeseries("moisture", lambda row: 100 - (row['moisture'] / 1024 * 100), 100),
        timeseries("temperature", lambda row: row['temperature'], 50),
        timeseries("light", lambda row: row['light'], 100),
        timeseries("humidity", lambda row: row['humidity'], 100)
    ]
    # print(timeseries_arr)
    template_context = dict(
        page="index",
        timeseries_moisture=json.dumps(timeseries_arr[0]),
        timeseries_temperature=json.dumps(timeseries_arr[1]),
        timeseries_light=json.dumps(timeseries_arr[2]),
        timeseries_humidity=json.dumps(timeseries_arr[3]),
        latest_sensor_values=latest_sensor_values,
        valve_openings=valve_openings,
        address=address
    )


    # template_context = dict(
    #     page="index",
    #     timeseries=json.dumps([])
    # )
    return render_template('template.html', **template_context)

def timeseries(key, transform_value_func, max_y):
    high_timestamp = int(time.time())
    low_timestamp = high_timestamp - 2 * one_day

    res = db.get_sensor_data(key, low_timestamp, high_timestamp, time_bucket_size)
    # res = db.get_temperature_values_per_device_per_timebucket(low_timestamp, high_timestamp, time_bucket_size)

    data = {}
    device_ids_obj = {}
    timestamps_obj = {}
    for _, row in enumerate(res):
        device_id = row['deviceId']
        timestamp = row['timestamp_bucket'] * 1000
        value = transform_value_func(row)
        # value = (1024 - row['moisture']) / 1024*100
        # value = row['temperature']

        timestamps_obj[timestamp] = {}
        device_ids_obj[device_id] = {}

        if timestamp not in data:
            data[timestamp] = {}

        if device_id not in data[timestamp]:
            data[timestamp][device_id] = {}

        data[timestamp][device_id] = value

    # logging.debug("DATA: " + json.dumps(data))


    timestamps = list(timestamps_obj.keys())
    timestamps.sort()

    device_ids = list(device_ids_obj.keys())
    device_ids.sort()


    rows = []
    for timestamp in timestamps:
        row = [timestamp]
        for device_id in device_ids:
            if device_id in data[timestamp]:
                v = data[timestamp][device_id]
                row.append(v)
            else:
                row.append(None)
        rows.append(row)

    if len(rows) == 1:
        cloned_row = rows[0].copy()
        cloned_row[0] = cloned_row[0]-100
        rows.append(cloned_row)

    timeseries = {
        "rows": rows,
        "meta": {
            # "max_y": 50,
            # "max_y": 100,
            "max_y": max_y,
            "device_ids": device_ids,
            "low_timestamp_ms": low_timestamp * 1000,
            "high_timestamp_ms": high_timestamp * 1000
        }
    }

    return timeseries
    # logging.debug("TIMESERIES")
    # logging.debug(timeseries)

# def debug(v):
#     logging.debug("-------------> ")
#     logging.debug(json.dumps(v))
