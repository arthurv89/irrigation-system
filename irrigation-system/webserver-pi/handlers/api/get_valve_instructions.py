from datetime import datetime
from utils import db
import logging
import time
import json
from flask import request

minute = 60
period = 15 * minute

moisture_threshold = 10
open_time = 5000

def handle():
    hour = datetime.now().hour
    if hour < 6 or hour > 19:
        logging.debug("Not the right time to open.")
        return {
            "open": to_open
        }

    high_timestamp = int(time.time())
    low_timestamp = high_timestamp - 2 * period

    deviceId = request.args.get('deviceId')

    recently_opened_hoses_rows = db.get_recently_opened_hoses()
    # logging.debug("recently_opened_hoses_rows")
    # logging.debug(recently_opened_hoses_rows)
    recently_opened_hoses = {row['hose_id']: row['hose_id'] for row in recently_opened_hoses_rows}
    # logging.debug("recently_opened_hoses")
    # logging.debug(recently_opened_hoses)

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
        if hose_id in moisture and moisture[hose_id] > moisture_threshold and hose_id not in recently_opened_hoses:
            obj = open_obj(hose_position, open_time)
            to_open.append(obj)

            db.write_opening(hose_id, open_time)
            logging.debug("Written open valve to DB")


    return {
        "open": to_open
    }

def open_obj(hose_position, time):
    return {
        "valve": hose_position,
        "time": time
    }
