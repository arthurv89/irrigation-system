import json
from flask import Flask, request, render_template

from elasticsearch import Elasticsearch

es = Elasticsearch([{'host':'localhost','port':9200}])

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    es_query = {
       "aggs":{
          "moisture_buckets":{
             "date_histogram":{
                "field":"time",
                "interval":"1h",
                "time_zone":"Europe/London",
                "min_doc_count":1
             },
             "aggs":{
                "data":{
                   "filter":{
                      "bool":{
                         "must":[
                            {
                               "query_string":{
                                  "query":"owner: \"casabatata\"",
                                  "analyze_wildcard":True,
                                  "default_field":"*"
                               }
                            },
                            {
                               "range":{
                                  "time":{
                                     "gte":1582843110263,
                                     "lte":1582929510263,
                                     "format":"epoch_millis"
                                  }
                               }
                            }
                         ]
                      }
                   },
                   "aggs":{
                      "average_moisture":{
                         "avg":{
                            "field":"moisture"
                         }
                      }
                   }
                }
             }
          }
       },
       "size":0,
       "_source":{
          "excludes":[

          ]
       },
       "stored_fields":[
          "*"
       ]
    }
    res = es.search(index="irsys", body=es_query)
    print(res)
    print("Got %d Hits:" % res['hits']['total'])

    arr = res['aggregations']['moisture_buckets']["buckets"]
    arr.sort(key=lambda x: x['key'])

    print("ordered_hits")
    print(res)

    datapoints = []
    for bucket in arr:
        timestamp = bucket['key']
        moisture_value = bucket['data']['average_moisture']['value']
        datapoint = [timestamp, moisture_value]
        print("Datapoint: " + str(datapoint))
        if moisture_value is not None:
            datapoints.append(datapoint)

    # datapoints = [[1582921800, 10], [1582921860, 100], [1582921920, 30]]
    template_context = dict(datapoints=datapoints)

    return render_template('index.html', **template_context)
