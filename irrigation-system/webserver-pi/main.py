import sys
import logging

from dataclasses import dataclass
from flask import Flask, request, send_from_directory, Response, jsonify
from flask_api import status
from handlers.pages import setup_wifi, connect_sensors, index
from handlers.pages import controller_actions
from handlers.api import find_ssids, submit, save_wifi, connect_sensor, get_connected_sensors, get_valve_instructions, set_valve, health, get_valve_status
from flask_log_request_id import RequestID, RequestIDLogFilter
import glob
import os
import properties

app = Flask(__name__, template_folder="jinja_templates")
# app.logger.setLevel(logging.INFO)

RequestID(app)
app.config['LOG_REQUEST_ID_GENERATE_IF_NOT_FOUND'] = True
# app.config['LOG_REQUEST_ID_LOG_ALL_REQUESTS'] = True
# app.config['LOG_REQUEST_ID_G_OBJECT_ATTRIBUTE'] = 'True'

# logging.basicConfig(
#     filename='/tmp/irsys.log',
#     level=logging.DEBUG,
#     format='[%(request_id)s] %(asctime)s [%(levelname)s]: %(message)s')
handler = logging.StreamHandler()
handler.setFormatter(logging.Formatter("%(asctime)s [%(levelname)s] %(request_id)s - %(message)s"))
handler.addFilter(RequestIDLogFilter())  # << Add request id contextual filter
logging.getLogger().addHandler(handler)
logging.getLogger().setLevel('DEBUG')

properties.os = os.getenv('IRSYS_OS')
if properties.os not in ["pi", "macosx"]:
    raise Exception("OS not correct: " + str(properties.os))

properties.working_directory = os.getenv('IRSYS_RUN_DIR') + "/.."


# Pages

@app.route('/', methods=['GET'])
def _index():
    return handle(index, "html")


@app.route('/connect-sensors', methods=['GET'])
def _connect_sensors():
    return handle(connect_sensors, "html")


@app.route('/setup-wifi', methods=['GET'])
def _setup_wifi():
    return handle(setup_wifi, "html")


@app.route('/controller-actions', methods=['GET'])
def _controller_actions():
    return handle(controller_actions, "html")


# API

@app.route('/api/valve-status', methods=['GET'])
def _get_valve_status():
    return handle(get_valve_status, "json")


@app.route('/api/submit', methods=['POST'])
def _submit():
    return handle(submit, "json")


@app.route('/api/health', methods=['GET'])
def _health():
    return handle(health, "json")


@app.route('/api/find-ssids', methods=['POST'])
def _find_ssids():
    return handle(find_ssids, "json")


@app.route('/api/save-wifi', methods=['POST'])
def _save_wifi():
    return handle(save_wifi, "json")


@app.route('/api/connect-sensor', methods=['POST'])
def _connect_sensor():
    return handle(connect_sensor, "json")


@app.route('/api/get-connected-sensors', methods=['POST'])
def _get_connected_sensors():
    return handle(get_connected_sensors, "json")


@app.route('/api/valveInstructions', methods=['GET'])
def _get_valve_instructions():
    return handle(get_valve_instructions, "json")


@app.route('/api/valve', methods=['POST'])
def _set_valve():
    return handle(set_valve, "json")


def get_sensor_bin(type):
    # logging.info(request.args.get('deviceId'));
    # if request.args.get('deviceId') == "72094380487927049551":
    #     logging.info("Redirecting sensor to moisture binary")
    #     return send_from_directory('bin/sensor', 'moisture.bin')

    return send_from_directory('bin/sensor', type + '.bin')


@app.route('/bin/blink', methods=['GET'])
def _get_blink_bin():
    speed = request.args.get('speed')
    get_bin()
    return send_from_directory('bin/blink', speed + '.bin')


@app.route('/bin/temperature', methods=['GET'])
def _get_temperature_bin():
    return get_sensor_bin('temperature')


@app.route('/bin/moisture', methods=['GET'])
def _get_moisture_bin():
    return get_sensor_bin('moisture')


@app.route('/bin/valve', methods=['GET'])
def _get_valve_bin():
    version = request.args.get('version')

    list_of_files = glob.glob('bin/valve/valve-*.bin')  # * means all if need specific format then *.csv
    latest_file = max(list_of_files, key=os.path.getctime).split("/")[-1]

    user_version = "valve-" + version + ".bin"
    logging.debug("Last file: " + latest_file)
    logging.debug("Version: " + user_version)
    if latest_file == user_version:
        return "", status.HTTP_404_NOT_FOUND
    else:
        return send_from_directory('bin/valve', latest_file)


# Static resources

@app.route('/css/<path:path>')
def send_css(path):
    return send_from_directory('static/css', path)


@app.route('/webfonts/<path:path>')
def send_webfonts(path):
    return send_from_directory('static/webfonts', path)


@app.route('/js/<path:path>')
def send_js(path):
    return send_from_directory('static/js', path)


@app.route('/img/<path:path>')
def send_img(path):
    return send_from_directory('static/img', path)


@app.route('/bower_components/<path:path>')
def send_bower_components(path):
    return send_from_directory('static/bower_components', path)


@app.route('/fonts/<path:path>')
def send_fonts(path):
    return send_from_directory('static/fonts', path)


def handle_stream(handler):
    return Response(handler.handle_stream(), content_type='text/event-stream')


def handle(handler, type):
    try:
        result = handler.handle()
        logging.info("Result")
        if type == "json":
            logging.info(result)
            response = jsonify({
                "status": "OK",
                "response": result
            })
        elif type == "html":
            response = result
        else:
            raise Exception("Could not determine type")
        return response, status.HTTP_200_OK
    except Exception as ex:
        try:
            exc_type, exc_obj, exc_tb = sys.exc_info()
            fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
            logging.error(exc_type, fname, exc_tb.tb_lineno)

            logging.debug(type(ex))
            logging.debug(type(ex).__name__)
            logging.debug(type(ex).args)

            template = "An exception of type {0} occurred. Arguments:\n{1!r}"
            message = template.format(type(ex).__name__, ex.args)
            logging.debug(message)
        except Exception as ex2:
            logging.debug(ex2)
        logging.debug("EXCEPTION!!!")
        logging.debug(ex)

        response = {
            "status": "Failed"
        }
        return response, status.HTTP_500_INTERNAL_SERVER_ERROR


if __name__ == "__main__":
    app.run(port=8123)
