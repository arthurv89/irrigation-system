import json
from flask import request
import properties
from elasticsearch import Elasticsearch
from datetime import datetime

es = Elasticsearch([{'host':'localhost','port':9200}])

def handle():
    store_data(request.get_json())

    ret = {}
    return ret

def store_data(form):
    timestamp = int(datetime.utcnow().strftime("%s"))
    id = form["deviceId"] + " @ " + str(timestamp)
    e1={
        "deviceId": form["deviceId"],
        "time": timestamp * 1000,
        "owner": properties.owner,
        "moisture": form["moisture"]
    }

    res = es.index(index='irsys', doc_type='moisture', id=id, body=e1)
