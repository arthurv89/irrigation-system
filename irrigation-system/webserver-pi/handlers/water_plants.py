import time
import json

def handle():
    print("[Start] Water the plants")
    high_timestamp_ms = int(time.time() * 1000)
    minute_ms = 60 * 1000
    low_timestamp_ms = high_timestamp_ms - 15 * minute_ms

    body = {
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
                "interval": "15m",
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
    # print(json.dumps(body))

    open_valves = []

    res = es.search(index="irsys-moisture-1", body=body)
    agg = res["aggregations"]['moisture_data']
    moisture_data_buckets = agg['buckets']
    for _, moisture_data_bucket in enumerate(moisture_data_buckets):
        device_id = moisture_data_bucket["key"]
        date_data = moisture_data_bucket["date_buckets"]['buckets']
        last_datapoint = date_data[len(date_data)-1]
        moisture = last_datapoint['moisture_value']['value']

        open_valves.append(device_id)

    for device_id in open_valves:
        opening_time = 10
        turn_on_valve(device_id, opening_time)

        # This can be batched
        write_opening_to_es(device_id, opening_time)

        print(device_id, moisture)
        # print(moisture_data_bucket)

def turn_on_valve(device_id, opening_time):
    print("Turning on " + device_id + " for " + str(opening_time) + " seconds")
    # TODO: set pins on the PI

def write_opening_to_es(device_id, opening_time):
    # We could have multiple moisture sensors for a valve (or other way around) in the future
    e1 = {
      "valve": device_id,
      "time": int(time.time() * 1000),
      "opening-time": opening_time
    }
    res = es.index(index='irsys-open_valve-1', body=e1)
    print("Written open valve to ES")


handle()
