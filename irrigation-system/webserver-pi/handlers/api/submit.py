import json
from flask import request
import properties
from datetime import datetime
from utils import db

def handle():
    store_data(request.get_json())
    return {}

def store_data(form):
    timestamp = int(datetime.utcnow().strftime("%s"))
    id = form["deviceId"] + " @ " + str(timestamp)
    db.put_moisture_value(
        deviceId=form["deviceId"],
        time=timestamp,
        owner=properties.owner,
        moisture=form["moisture"]
    )
