import logging

from flask import request

from utils import db
from utils.db import ValveBoxId, Valve


def handle():
    device_id = ValveBoxId(request.form.get('deviceId'))
    valve_position = int(request.form.get('valvePosition'))
    status = request.form.get('status') == 'true'
    opening_time = int(request.form.get('opening_time'))

    logging.debug(f"Setting valve_id of position {valve_position} to {status} for {opening_time} seconds")

    if status:
        logging.warning("Opening valve_id!")
        valves: list[Valve] = db.get_valves_for_device(device_id)
        logging.warning(valves)
        valve = valves[valve_position]
        logging.debug(valve)

        db.write_opening(valve.valve_id, opening_time)
        logging.debug(db.get_valve_openings())
    else:
        logging.warning("Can't close valve_id yet!")
    return {}
