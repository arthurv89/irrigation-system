import json
from flask import Flask, request, render_template, jsonify
import time

from utils import db

app = Flask(__name__, template_folder="jinja_templates")

max_lines = 20
interval_minutes = 15

def handle():
    high_timestamp_ms = int(time.time() * 1000)
    one_day_ms = 86400 * 1000
    low_timestamp_ms = high_timestamp_ms - 1 * one_day_ms

    res = db.moisture_values_per_device_per_hour()
    print(res)

    # data = {}
    # device_ids_obj = {}
    # timestamps_obj = {}
    # for _, moisture_data_bucket in enumerate(aggregations["moisture_data"]['buckets']):
    #     device_id = moisture_data_bucket['key']
    #     data[device_id] = {}
    #     for _, date_bucket in enumerate(moisture_data_bucket['date_buckets']['buckets']):
    #         timestamp = date_bucket['key']
    #         moisture_value = date_bucket['moisture_value']['value']
    #
    #         timestamps_obj[timestamp] = {}
    #         device_ids_obj[device_id] = {}
    #
    #         if timestamp not in data:
    #             data[timestamp] = {}
    #         data[timestamp][device_id] = moisture_value

    # timestamps = list(timestamps_obj.keys())
    # timestamps.sort()
    #
    # device_ids = list(device_ids_obj.keys())
    # device_ids.sort()
    #
    #
    # rows = []
    # for timestamp in timestamps:
    #     # row = {
    #     #     "time": timestamp
    #     # }
    #     row = [timestamp]
    #     for device_id in device_ids:
    #         if device_id in data[timestamp]:
    #             v = data[timestamp][device_id]
    #             row.append(v)
    #         else:
    #             row.append(None)
    #     rows.append(row)
    #
    # if len(rows) == 1:
    #     cloned_row = rows[0].copy()
    #     cloned_row[0] = cloned_row[0]-100
    #     rows.append(cloned_row)
    #
    # timeseries = {
    #     "rows": rows,
    #     "meta": {
    #         "device_ids": device_ids,
    #         "low_timestamp_ms": low_timestamp_ms,
    #         "high_timestamp_ms": high_timestamp_ms
    #     }
    # }
    # print("TIMESERIES")
    # print(timeseries)
    # template_context = dict(
    #     page="index",
    #     timeseries=json.dumps(timeseries)
    # )


    template_context = dict(
        page="index",
        timeseries=json.dumps([])
    )
    return render_template('template.html', **template_context)

def debug(v):
    print("-------------> ")
    print(json.dumps(v))
