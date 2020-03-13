from flask import Flask, request, render_template, jsonify
import json
import textwrap
import os
from utils import db, wifiUtil

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    ssid = request.form.get('ssid') or ""
    password = request.form.get('password') or ""

    print("Saving Wifi")
    db.put_wifi_credentials(ssid, password)

    wifiUtil.connect_main(ssid, password)



    return {}
