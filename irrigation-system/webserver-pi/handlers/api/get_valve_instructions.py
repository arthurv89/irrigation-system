import logging
import typing
from dataclasses import dataclass
from datetime import datetime

import pytz
from flask import request

from utils import db
from utils.db import ValveBoxId, Valve, LastOpened, ValveId

open_time = 3
time_between_opening = 10


@dataclass
class Instruction:
    valve_id: int
    time: int


@dataclass
class Status:
    valve_id: ValveId
    valve_position: int
    open: bool
    time_remaining: float
    can_open: bool


def handle():
    device_id: ValveBoxId = ValveBoxId(request.args.get('deviceId'))

    # instructions = open_all_valves()
    instructions: list[Instruction] = smart_open_valves(device_id)
    logging.debug("")
    return instructions


def smart_open_valves(device_id) -> list[Instruction]:
    # hour = datetime.now().hour
    # if hour < 6 or hour > 19:
    #     logging.debug("Not the right time to open.")
    #     return {
    #         "open": []
    #     }

    # high_timestamp = int(time.time())
    # low_timestamp = high_timestamp - 2 * period

    valves: list[Valve] = db.get_valves_for_device(device_id)
    # valves = valves[0:1]
    # valve_ids = [row.valve_id for row in valves]

    logging.debug("device_id")
    logging.debug(device_id)

    # logging.debug("valves")
    # logging.debug(valves)

    # valves = {valve.valve_id: valve.valve_position for valve in valves}
    # logging.debug("valves")
    # logging.debug(valves)

    last_opened_valves: dict[str, LastOpened] = db.get_last_opened(valves)
    logging.debug("valves")
    logging.debug(valves)

    instructions: list[Instruction] = []
    for valve in valves:
        last_opened: typing.Optional[LastOpened] = last_opened_valves[valve.valve_id.value] if valve.valve_id.value in last_opened_valves else None
        instruction: Instruction = get_instruction(valve.valve_id, valve.valve_position, last_opened)
        if instruction is not None:
            instructions.append(instruction)

    return instructions


def get_instruction(valve_id: ValveId, valve_position: int, last_opened: typing.Optional[LastOpened]) -> typing.Optional[Instruction]:
    status: Status = get_status(valve_id, valve_position, last_opened)

    if status.can_open:
        obj: Instruction = open_obj(valve_position, status.time_remaining)
        db.write_opening(valve_id, status.time_remaining)
        return obj


def get_status(valve_id: ValveId, valve_position: int, last_opened: typing.Optional[LastOpened]) -> Status:
    if last_opened is None:
        logging.debug("Never opened. Open now for " + str(open_time) + " seconds")
        return Status(valve_id=valve_id, valve_position=valve_position, open=False, can_open=True, time_remaining=open_time)
    else:
        last_closed: datetime = last_opened.last_closed

        time_diff = datetime.now().astimezone(pytz.utc) - last_closed
        time_since_last_closed = time_diff.total_seconds()

        if time_since_last_closed < 0:
            time_left = -time_since_last_closed
            logging.debug("Still open. " + str(time_left) + " more seconds")
            return Status(valve_id=valve_id, valve_position=valve_position, open=True, can_open=True, time_remaining=time_left)
        elif time_since_last_closed < time_between_opening:
            logging.debug("Too soon to open again. Try again in " + str(time_between_opening - time_since_last_closed) + " seconds")
            return Status(valve_id=valve_id, valve_position=valve_position, open=False, can_open=False, time_remaining=time_since_last_closed)
        else:
            logging.debug("Time to open again for " + str(open_time) + " seconds")
            return Status(valve_id=valve_id, valve_position=valve_position, open=False, can_open=True, time_remaining=open_time)


# def open_all_valves():
#     device_id = ValveBoxId(request.args.get('device_id'))
#
#     valve_rows = db.get_valves_for_device(device_id)
#     # valve_rows = valve_rows[0:1]
#     valve_ids = [row.valve_id for row in valve_rows]
#     valves = {row.valve_id: row.valve_position for row in valve_rows}
#
#     instructions = []
#     for valve_position, valve_id in enumerate(valves):
#         obj = open_obj(valve_position, open_time)
#         instructions.append(obj)
#     return instructions


def open_obj(valve_position, time) -> Instruction:
    return Instruction(valve_position, time)
