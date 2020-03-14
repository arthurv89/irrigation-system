import logging
import time
import json

high_timestamp = int(time.time())
low_timestamp = high_timestamp - 1 * one_day
time_bucket_size = 300

def handle():
    logging.debug("[Start] Water the plants")
    high_timestamp_ms = int(time.time() * 1000)
    minute_ms = 60 * 1000
    low_timestamp_ms = high_timestamp_ms - 15 * minute_ms

    open_valves = []

    res = db.get_moisture_values_per_device_per_timebucket(low_timestamp, high_timestamp, time_bucket_size)
    for _, moisture_data_bucket in enumerate(moisture_data_buckets):
        device_id = moisture_data_bucket["key"]
        date_data = moisture_data_bucket["date_buckets"]['buckets']
        last_datapoint = date_data[len(date_data)-1]
        moisture = last_datapoint['moisture_value']['value']

        open_valves.append(device_id)

    for device_id in open_valves:
        opening_time = 10
        turn_on_valve(device_id, opening_time)

        # This can be batched
        write_opening(device_id, opening_time)

        logging.debug(device_id, moisture)
        # logging.debug(moisture_data_bucket)

def turn_on_valve(device_id, opening_time):
    logging.debug("Turning on " + device_id + " for " + str(opening_time) + " seconds")
    # TODO: set pins on the PI

def write_opening(device_id, opening_time):
    # We could have multiple moisture sensors for a valve (or other way around) in the future
    db.write_opening(device_id, opening_time)
    logging.debug("Written open valve to ES")


handle()
