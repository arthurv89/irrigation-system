from access_points import get_scanner

from flask import Flask, request, render_template, jsonify
from Generator import Generator
import json

app = Flask(__name__, template_folder="jinja_templates")

def handle_stream():
    def _run():
        wifi_scanner = get_scanner()
        arr = wifi_scanner.get_access_points()
        for x in arr:
            yield add_ssid({"ssid": x['ssid'], "quality": x['quality']})

    def add_ssid(ssid):
        obj['networks'].append(ssid)
        obj['networks'] = sorted(obj['networks'], key=lambda k: k['quality'], reverse=True)
        return obj



    obj = {
        "networks": [],
    }

    for message in Generator(_run()):
        yield json.dumps(message) + '\n'
