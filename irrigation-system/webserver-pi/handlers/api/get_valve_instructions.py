from utils import db
import logging
import time
import json
from flask import request

minute = 60
period = 15 * minute

moisture_threshold = 800
open_time = 5000

def handle():
    high_timestamp = int(time.time())
    low_timestamp = high_timestamp - 2 * period

    deviceId = request.args.get('deviceId')

    not_recently_opened_valves = db.get_not_recently_opened_valves()
    # logging.debug("not_recently_opened_valves")
    # logging.debug(not_recently_opened_valves)

    hose_rows = db.get_hoses_for_valve(deviceId)
    # logging.debug("hose_rows")
    # logging.debug(hose_rows)
    hoses = {row['hose_id']: row['hose_position'] for row in hose_rows}
    # logging.debug("hoses")
    # logging.debug(hoses)

    sensor_data_rows = db.average_moisture(hoses.keys())
    moisture = {row['deviceId']: row['avg_value'] for row in sensor_data_rows}
    # logging.debug("moisture")
    # logging.debug(moisture)

    to_open = []
    for hose_position, hose_id in enumerate(hoses):
        if hose_id in moisture and moisture[hose_id] < moisture_threshold:
            obj = open_obj(hose_position, 4000)
            to_open.append(obj)

    return {
        "open": to_open
    }

def open_obj(hose_position, time):
    return {
        "valve": hose_position,
        "time": time
    }
