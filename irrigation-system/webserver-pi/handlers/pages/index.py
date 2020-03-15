import logging
import json
from flask import Flask, request, render_template, jsonify
import time

from utils import db

app = Flask(__name__, template_folder="jinja_templates")

max_lines = 20
interval_minutes = 5
one_day = 86400 # seconds
time_bucket_size = 300

def handle():
    high_timestamp = int(time.time())
    low_timestamp = high_timestamp - 1 * one_day

    res = db.get_temperature_values_per_device_per_timebucket(low_timestamp, high_timestamp, time_bucket_size)

    data = {}
    device_ids_obj = {}
    timestamps_obj = {}
    for _, row in enumerate(res):
        device_id = row['deviceId']
        timestamp = row['timestamp_bucket'] * 1000
        # moisture = (1024 - row['moisture']) / 1024*100
        value = row['temperature']

        timestamps_obj[timestamp] = {}
        device_ids_obj[device_id] = {}

        if timestamp not in data:
            data[timestamp] = {}

        if device_id not in data[timestamp]:
            data[timestamp][device_id] = {}

        data[timestamp][device_id] = value

    logging.debug("DATA: " + json.dumps(data))


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
            "max_y": 50,
            "device_ids": device_ids,
            "low_timestamp_ms": low_timestamp * 1000,
            "high_timestamp_ms": high_timestamp * 1000
        }
    }
    logging.debug("TIMESERIES")
    logging.debug(timeseries)
    template_context = dict(
        page="index",
        timeseries=json.dumps(timeseries)
    )


    # template_context = dict(
    #     page="index",
    #     timeseries=json.dumps([])
    # )
    return render_template('template.html', **template_context)

# def debug(v):
#     logging.debug("-------------> ")
#     logging.debug(json.dumps(v))
