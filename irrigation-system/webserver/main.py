import sys
import json
from flask import Flask, send_from_directory, Response
from flask_api import status
from handlers import index, scan
from handlers.api import scan as scanNow, health, submit
import time
import poll_new_devices

app = Flask(__name__, template_folder="jinja_templates")


# Dynamic

@app.route('/', methods=['GET'])
def _index():
    return handle(index, "html")

@app.route('/scan', methods=['GET'])
def _scan():
    return handle(scan, "html")

@app.route('/api/submit', methods=['POST'])
def _submit():
    return handle(submit, "json")

@app.route('/api/health', methods=['GET'])
def _health():
    return handle(health, "json")

@app.route('/api/scan', methods=['POST'])
def _scanNow():
    return Response(poll_new_devices.run(), content_type='text/event-stream')


# Static resources

@app.route('/css/<path:path>')
def send_css(path):
    return send_from_directory('static/css', path)

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




def handle(handler, type):
    try:
        result = handler.handle()
        if type == "json":
            response = {
                "status": "OK",
                "response": result
            }
        elif type == "html":
            response = result
        else:
            raise Exception("Could not determine type")
        return response, status.HTTP_200_OK
    except Exception as ex:
        template = "An exception of type {0} occurred. Arguments:\n{1!r}"
        message = template.format(type(ex).__name__, ex.args)
        print(message)

        response = {
            "status": "Failed"
        }
        return response, status.HTTP_500_INTERNAL_SERVER_ERROR
