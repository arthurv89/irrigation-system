import sys
sys.path.append("../src") # Adds higher directory to python modules path.
import logging
import time
from utils import db

minute = 60
period = 15 * minute

moisture_threshold = 100

def handle():
    logging.debug("[Start] Water the plants")
    high_timestamp = int(time.time())
    low_timestamp = high_timestamp - 2 * period

    open_valves = []

    moisture_values = db.get_sensor_data("moisture", low_timestamp, high_timestamp, period)
    print(moisture_values)

    devices = {}
    for _, row in enumerate(moisture_values):
        device_id = row["deviceId"]
        if not device_id in devices:
            devices[device_id] = []
        devices[device_id].append(row)

    for device_id in devices.keys():
        interesting_row = devices[device_id][1]
        device_id = interesting_row["deviceId"]
        date_data = interesting_row["timestamp_bucket"]
        moisture = interesting_row["moisture"]

        if moisture > moisture_threshold:
            open_valves.append(device_id)

    for device_id in open_valves:
        opening_time = 10
        turn_on_valve(device_id, opening_time)

        # This can be batched
        write_opening(device_id, opening_time)

def turn_on_valve(device_id, opening_time):
    logging.debug("Turning on " + device_id + " for " + str(opening_time) + " seconds")
    # TODO: set pins on the PI

def write_opening(device_id, opening_time):
    # We could have multiple moisture sensors for a valve (or other way around) in the future
    db.write_opening(device_id, opening_time)
    logging.debug("Written open valve to ES")

    # print(device_id)
    # logging.debug(device_id)


handle()
