from access_points import get_scanner
import logging

from flask import Flask, request, render_template, jsonify
import json

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    def _run():
        logging.debug("Create scanner")
        wifi_scanner = get_scanner()

        logging.debug("Get access points")
        arr = wifi_scanner.get_access_points()
        for x in arr:
            logging.debug("SSID: " + json.dumps(x))
            add_ssid({"ssid": x['ssid'], "quality": x['quality']})
        logging.debug("Done")

    def add_ssid(ssid):
        obj['networks'].append(ssid)
        return obj



    obj = {
        "networks": [],
    }

    _run()
    obj['networks'] = sorted(obj['networks'], key=lambda k: k['quality'], reverse=True)
    return obj
