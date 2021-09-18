import typing

from dataclasses import dataclass
from datetime import timedelta

from flask import Flask, request

from handlers.api.get_valve_instructions import get_status, Status
from utils import varsUtil, db
from utils.db import ValveBoxId, LastOpened, Valve, ValveId

app = Flask(__name__, template_folder="jinja_templates")


@dataclass
class StatusResponse:
    valve_id: ValveId
    valve_position: int
    open: bool
    time_remaining: float
    can_open: bool


def handle():
    valve_box_id: ValveBoxId = ValveBoxId(varsUtil.not_null(request.args.get('deviceId'), "Request DeviceId"))
    valves: list[Valve] = db.get_valves_for_device(valve_box_id)
    last_opened_valves: dict[str, LastOpened] = db.get_last_opened(valves)

    statuses: list[StatusResponse] = []
    for valve in valves:
        last_opened: typing.Optional[LastOpened] = last_opened_valves[valve.valve_id.value] if valve.valve_id.value in last_opened_valves else None
        status: Status = get_status(valve.valve_id, valve.valve_position, last_opened)
        statuses.append(get_status_response(status))
    print(statuses)
    return statuses


def get_status_response(status: Status) -> StatusResponse:
    return StatusResponse(status.valve_id, status.valve_position, status.open, status.time_remaining.total_seconds(), status.can_open)
