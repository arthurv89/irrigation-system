import typing

from flask import Flask, request

from handlers.api.get_valve_instructions import get_status, Status
from utils import varsUtil, db
from utils.db import ValveBoxId, LastOpened, Valve

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    valve_box_id: ValveBoxId = ValveBoxId(varsUtil.not_null(request.args.get('deviceId'), "Request DeviceId"))
    valves: list[Valve] = db.get_valves_for_device(valve_box_id)
    last_opened_valves: dict[str, LastOpened] = db.get_last_opened(valves)

    statuses: list[Status] = []
    for valve in valves:
        last_opened: typing.Optional[LastOpened] = last_opened_valves[valve.valve_id.value] if valve.valve_id.value in last_opened_valves else None
        status: Status = get_status(valve.valve_id, valve.valve_position, last_opened)
        statuses.append(status)
    print(statuses)
    return statuses
