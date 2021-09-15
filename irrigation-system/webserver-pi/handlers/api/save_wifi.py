from flask import Flask, request
from utils import db, wifiUtil
import logging

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    ssid = request.form.get('ssid') or ""
    password = request.form.get('password') or ""

    logging.debug("Saving Wifi")
    db.put_wifi_credentials(ssid, password)

    wifiUtil.connect_main(ssid, password)



    return {}
