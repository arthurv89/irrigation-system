import json
from flask import Flask, request, render_template

from elasticsearch import Elasticsearch

es = Elasticsearch([{'host':'localhost','port':9200}])

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    es_query = {
      "aggs": {
        "moisture_data": {
          "terms": {
            "field": "deviceId.keyword",
            "size": 20,
            "order": {
              "_key": "desc"
            },
            "missing": "__missing__"
          },
          "aggs": {
            "date_buckets": {
              "date_histogram": {
                "field": "time",
                "interval": "1h",
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
                  "gte": 1582807897603,
                  "lte": 1582980697603,
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
    res = es.search(index="irsys", body=es_query)
    print()
    print()
    print()
    print()
    print(res)
    print("Got %d Hits:" % res['hits']['total'])

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

            if device_id not in data:
                data[device_id] = {}
            data[device_id][timestamp] = moisture_value
    debug(data)

    timestamps = list(timestamps_obj.keys())
    timestamps.sort()

    device_ids = list(device_ids_obj.keys())
    device_ids.sort()


    # debug(timestamps)
    # debug(device_ids_obj)

    device_lines = []
    for device_id in device_ids:
        rows = []
        for timestamp in timestamps:
            row = [timestamp]
            if timestamp in data[device_id]:
                v = data[device_id][timestamp]
                row.append(v)
            else:
                row.append(None)
            rows.append(row)
        device_lines.append(rows)

    # debug(lines)

    # datapoints = [[1582921800, 10], [1582921860, 100], [1582921920, 30]]
    template_context = dict(lines=json.dumps(device_lines))

    return render_template('index.html', **template_context)

def debug(v):
    print("-------------> ")
    print(json.dumps(v))
