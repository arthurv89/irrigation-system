import json
from flask import request
import properties
from datetime import datetime

def handle():
    store_data(request.get_json())
    return {}

def store_data(form):
    timestamp = int(datetime.utcnow().strftime("%s"))
    id = form["deviceId"] + " @ " + str(timestamp)
    e1={
        "deviceId": form["deviceId"],
        "time": timestamp * 1000,
        "owner": properties.owner,
        "moisture": form["moisture"]
    }

    res = es.index(index="irsys-moisture-1", id=id, body=e1)
