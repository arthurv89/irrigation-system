from utils import db
import logging
import time
from flask import request

minute = 60
period = 15 * minute

moisture_threshold = 100
open_time = 5000

def handle():
    deviceId = request.args.get('deviceId')

    not_recently_opened_valves = db.get_not_recently_opened_valves()

    rows = db.get_hoses_for_valve(deviceId)
    hoses = {row['hose_id']: row['hose_position'] for row in rows}
    hose_ids = hoses.keys()
    logging.debug("Rows")
    logging.debug(hoses)

    high_timestamp = int(time.time())
    low_timestamp = high_timestamp - 2 * period

    sensor_data = db.average_moisture(hose_ids)

    to_open_device_ids = [open_obj(hoses[x['deviceId']], open_time) for x in sensor_data if x['avg_value'] > moisture_threshold]

    return {
        "open": to_open_device_ids
    }

def open_obj(valve, time):
    return {
        "valve": valve,
        "time": time
    }
