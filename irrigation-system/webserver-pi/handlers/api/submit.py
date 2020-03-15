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

    if "moisture" in form:
        db.put_sensor_value(
            deviceId=form["deviceId"],
            time=timestamp,
            owner=properties.owner,
            type="moisture",
            value=form["moisture"]
        )

    if "temperature" in form:
        db.put_sensor_value(
            deviceId=form["deviceId"],
            time=timestamp,
            owner=properties.owner,
            type="temperature",
            value=form["temperature"]
        )
