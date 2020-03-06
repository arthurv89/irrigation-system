import json
from flask import Flask, request, render_template, jsonify
import time
from elasticsearch import Elasticsearch

es = Elasticsearch([{'host':'localhost','port':9200}])

app = Flask(__name__, template_folder="jinja_templates")

max_lines = 20
interval_minutes = 15

def handle():
    high_timestamp_ms = int(time.time() * 1000)
    one_day_ms = 86400 * 1000
    low_timestamp_ms = high_timestamp_ms - 1 * one_day_ms

    es_query = {
      "aggs": {
        "moisture_data": {
          "terms": {
            "field": "deviceId.keyword",
            "size": 60 / interval_minutes * max_lines,
            "order": {
              "_key": "desc"
            },
            "missing": "__missing__"
          },
          "aggs": {
            "date_buckets": {
              "date_histogram": {
                "field": "time",
                "interval": str(interval_minutes) + "m",
                "time_zone": "Europe/London",
                "min_doc_count": 1
              },
              "aggs": {
                "moisture_value": {
                  "avg": {
                    "field": "moisture"
                  }
                }
              }
            }
          }
        }
      },
      "size": 0,
      "_source": {
        "excludes": []
      },
      "stored_fields": [
        "*"
      ],
      "script_fields": {},
      "docvalue_fields": [
        {
          "field": "time",
          "format": "date_time"
        }
      ],
      "query": {
        "bool": {
          "must": [
            {
              "query_string": {
                "query": "owner: \"casabatata\"",
                "analyze_wildcard": True,
                "default_field": "*"
              }
            },
            {
              "range": {
                "time": {
                  "gte": low_timestamp_ms,
                  "lte": high_timestamp_ms,
                  "format": "epoch_millis"
                }
              }
            }
          ],
          "filter": [],
          "should": [],
          "must_not": []
        }
      }
    }
    res = es.search(index="irsys-moisture-1", body=es_query)
    print(json.dumps(es_query))
    # print(res)
    # print("Got %d Hits:" % res['hits']['total'])

    aggregations = res['aggregations']

    data = {}
    device_ids_obj = {}
    timestamps_obj = {}
    for _, moisture_data_bucket in enumerate(aggregations["moisture_data"]['buckets']):
        device_id = moisture_data_bucket['key']
        data[device_id] = {}
        for _, date_bucket in enumerate(moisture_data_bucket['date_buckets']['buckets']):
            timestamp = date_bucket['key']
            moisture_value = date_bucket['moisture_value']['value']

            timestamps_obj[timestamp] = {}
            device_ids_obj[device_id] = {}

            if timestamp not in data:
                data[timestamp] = {}
            data[timestamp][device_id] = moisture_value

    timestamps = list(timestamps_obj.keys())
    timestamps.sort()

    device_ids = list(device_ids_obj.keys())
    device_ids.sort()


    rows = []
    for timestamp in timestamps:
        # row = {
        #     "time": timestamp
        # }
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
            "device_ids": device_ids,
            "low_timestamp_ms": low_timestamp_ms,
            "high_timestamp_ms": high_timestamp_ms
        }
    }
    print("TIMESERIES")
    print(timeseries)
    template_context = dict(
        page="index",
        timeseries=json.dumps(timeseries)
    )

    return render_template('template.html', **template_context)

def debug(v):
    print("-------------> ")
    print(json.dumps(v))
