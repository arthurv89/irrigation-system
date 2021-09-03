from datetime import datetime
from utils import db
import logging
import time
import json
from flask import request
import pytz

minute = 60
period = 15 * minute

moisture_threshold = 10
open_time = 10

time_between_opening = 10

def handle():
    hour = datetime.now().hour
    # if hour < 6 or hour > 19:
    #     logging.debug("Not the right time to open.")
    #     return {
    #         "open": []
    #     }

    high_timestamp = int(time.time())
    low_timestamp = high_timestamp - 2 * period

    deviceId = request.args.get('deviceId')

    hose_rows = db.get_hoses_for_valve(deviceId)
    hose_ids = [row['hose_id'] for row in hose_rows]

    # logging.debug("hose_rows")
    # logging.debug(hose_rows)
    hoses = {row['hose_id']: row['hose_position'] for row in hose_rows}
    # logging.debug("hoses")
    # logging.debug(hoses)

    hose_rows = db.get_last_opened(hose_ids)
    # logging.debug("hose_rows")
    # logging.debug(hose_rows)
    # recently_opened_hoses = {row['hose_id']: row['hose_id'] for row in recently_opened_hoses_rows}
    # logging.debug("recently_opened_hoses")
    # logging.debug(recently_opened_hoses)

    # sensor_data_rows = db.average_moisture(hoses.keys())
    # moisture = {row['deviceId']: row['avg_value'] for row in sensor_data_rows}
    # logging.debug("moisture")
    # logging.debug(moisture)

    to_open = []
    for hose_position, hose_id in enumerate(hoses):
        # if moisture[hose_id] > moisture_threshold and hose_id not in recently_opened_hoses:
        hose_row = hose_rows[hose_id]
        if hose_row is not None:
            last_closed = hose_row["last_closed"]

            time_diff = datetime.now().astimezone(pytz.utc) - last_closed
            time_since_last_closed = time_diff.total_seconds()

            if time_since_last_closed < 0:
                logging.debug("Still open. " + str(-time_since_last_closed) + " more seconds")
                obj = open_obj(hose_position, open_time)
                to_open.append(obj)
            elif time_since_last_closed < time_between_opening:
                logging.debug("Too soon to open again. Try again in " + str(time_between_opening - time_since_last_closed) + " seconds")
            else:
                logging.debug("Time to open again for " + str(open_time) + " seconds")
                db.write_opening(hose_id, open_time)
        else:
            logging.debug("Never opened. Open now for " + str(open_time) + " seconds")
            db.write_opening(hose_id, open_time)
    logging.debug("")


    return {
        "open": to_open
    }

def open_obj(hose_position, time):
    return {
        "valve": hose_position,
        "time": time
    }
