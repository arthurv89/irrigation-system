from access_points import get_scanner

from flask import Flask, request, render_template, jsonify
import json
import sys
import traceback
from time import sleep, time
import netifaces
from socket import AF_INET
import requests
from utils import db, wifiUtil

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    def _run():
        print("Run method")
        try:
            credentials = db.get_wifi_credentials()
            scan_and_connect(credentials)
        except:
            exc_type, exc_value, exc_traceback = sys.exc_info()
            print("*** print_tb:")
            traceback.print_tb(exc_traceback, limit=1, file=sys.stdout)
            print("*** print_exception:")
            # exc_type below is ignored on 3.5 and later
            traceback.print_exception(exc_type, exc_value, exc_traceback,
                                      limit=2, file=sys.stdout)
            print("*** print_exc:")
            traceback.print_exc(limit=2, file=sys.stdout)
            print("*** format_exc, first and last line:")
            formatted_lines = traceback.format_exc().splitlines()
            print(formatted_lines[0])
            print(formatted_lines[-1])
            print("*** format_exception:")
            # exc_type below is ignored on 3.5 and later
            print(repr(traceback.format_exception(exc_type, exc_value,
                                                  exc_traceback)))
            print("*** extract_tb:")
            print(repr(traceback.extract_tb(exc_traceback)))
            print("*** format_tb:")
            print(repr(traceback.format_tb(exc_traceback)))
            print("*** tb_lineno:", exc_traceback.tb_lineno)
            print("Error occurred. Please try again", "error")

    def scan_and_connect(main_wifi_credentials):
        print(request.form['ssid'])
        print("main_wifi_credentials", main_wifi_credentials)
        new_ssid = request.form['ssid']
        hasSsid = find_cell(new_ssid)

        if not hasSsid:
            print("Could not find network with SSID " + new_ssid, "in-progress")
            return False


        print("Found sensor with name " + new_ssid, "in-progress")
        is_connected = wifiUtil.connect_moisture_sensor(main_wifi_credentials, new_ssid)
        print(is_connected)
        if not is_connected:
            print("Couldn't connect to sensor", "error")
            return False

        print("Connected to sensor!", "in-progress")
        ip = get_gateway_ip()
        if ip is None:
            print("Coulnd't find IP Gateway", "error")
            return False

        saved = save_credentials(ip, main_wifi_credentials)

        if not saved:
            print("Could not save the wifi credentials", "error")
            return False

        print("Sensor has Wifi now. It's added to you list of sensors.", "done")

        wifiUtil.connect_main(main_wifi_credentials['ssid'], main_wifi_credentials['password'])

        association_to_db(new_ssid)

        return True


    def find_cell(ssid):
        # print("FINDING CELL")
        for i in range(0, 5):
            hasSsid = scan(ssid)
            if hasSsid:
                return True
            print("Couldn't find it yet, trying again in 3 seconds...")
            sleep(3)
        return False

    def association_to_db(device_id):
        # We could have multiple moisture sensors for a valve (or other way around) in the future
        db.write_sensor_association(device_id, int(time()))
        print("Written assocation of sensor " + device_id + " to the database")




    def scan(ssid):
        # print("SCANNNNN")
        wifi_scanner = get_scanner()
        # print(wifi_scanner)
        arr = wifi_scanner.get_access_points()
        print(arr)
        for x in arr:
            if x.ssid == ssid:
                return True

        return False


    def save_credentials(ip, main_wifi_credentials):
        url = "http://" + ip + "/wifisave?s=" + main_wifi_credentials['ssid'] + "&p=" + main_wifi_credentials['password']
        print(url)
        response = requests.get(url)
        html = response.content.decode("utf-8")
        print(type(html))
        print(html)
        return "Credentials Saved" in html


    def get_gateway_ip():
        for i in range(0, 10):
            gws = netifaces.gateways()
            print(gws)
            if AF_INET in gws:
                return gws[AF_INET][0][0]
            print("Sleeping...")
            sleep(1)
        return None

    _run()
    return {}
