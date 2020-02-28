import sys
import json
from flask import Flask
from flask_api import status
from handlers import index, health, submit

app = Flask(__name__)

@app.route('/', methods=['GET'])
def _index():
    return handle(index)

@app.route('/submit', methods=['POST'])
def _submit():
    return handle(submit)

@app.route('/health', methods=['GET'])
def _health():
    return handle(health)


def handle(handler):
    try:
        response = {
            "status": "OK",
            "response": handler.handle(),
        }
        return response, status.HTTP_200_OK
    except Exception as ex:
        template = "An exception of type {0} occurred. Arguments:\n{1!r}"
        message = template.format(type(ex).__name__, ex.args)
        print(message)

        response = {
            "status": "Failed"
        }
        return response, status.HTTP_500_INTERNAL_SERVER_ERROR
