import json
from flask import request
import properties
from datetime import datetime
from utils import db
import logging

def handle():
    store_data(request.get_json())
    return {}

def store_data(form):
    logging.debug(form)
    put_value("moisture", form)
    put_value("temperature", form)
    put_value("light", form)

def put_value(key, form):
    timestamp = int(datetime.utcnow().strftime("%s"))
    if key in form:
        db.put_sensor_value(
            deviceId=form["deviceId"],
            time=timestamp,
            owner=properties.owner,
            type=key,
            value=form[key]
        )
