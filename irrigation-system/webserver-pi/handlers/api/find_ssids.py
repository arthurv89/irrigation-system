from access_points import get_scanner

from flask import Flask, request, render_template, jsonify
from Generator import Generator
import json

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    def _run():
        print("Create scanner")
        wifi_scanner = get_scanner()

        print("Get access points")
        arr = wifi_scanner.get_access_points()
        for x in arr:
            print("SSID", x)
            add_ssid({"ssid": x['ssid'], "quality": x['quality']})
        print("Done")

    def add_ssid(ssid):
        obj['networks'].append(ssid)
        return obj



    obj = {
        "networks": [],
    }

    _run()
    obj['networks'] = sorted(obj['networks'], key=lambda k: k['quality'], reverse=True)
    return obj
