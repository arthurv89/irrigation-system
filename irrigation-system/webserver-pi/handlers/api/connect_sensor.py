from access_points import get_scanner

from flask import Flask, request, render_template, jsonify
import json
import sys
import traceback
from wifi import Cell
from time import sleep, time
from wireless import Wireless
import textwrap
import netifaces
from socket import AF_INET
import requests
from utils import db

wire = Wireless()

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    def _run():
        print("Run method")
        try:
            with open('wifi.txt', 'r') as fh:
                credentials = json.loads(fh.readlines()[0])
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

    def scan_and_connect(credentials):
        print(request.form['ssid'])
        ssid = request.form['ssid']
        hasSsid = find_cell(ssid)

        if not hasSsid:
            print("Could not find network with SSID " + ssid, "in-progress")
            return False


        print("Found sensor with name " + ssid, "in-progress")
        is_connected = connect(ssid)
        if not is_connected:
            print("Couldn't connect to sensor", "error")
            return False

        print("Connected to sensor!", "in-progress")
        ip = get_gateway_ip()
        if ip is None:
            print("Coulnd't find IP Gateway", "error")
            return False

        saved = save_credentials(ip, credentials)

        if not saved:
            print("Could not save the wifi credentials", "error")
            return False

        print("Sensor has Wifi now. It's added to you list of sensors.", "done")

        reconnect_to_original_wifi()

        association_to_db(ssid)

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



    def reconnect_to_original_wifi():
        wire.power(False)
        wire.power(True)


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



    def connect(ssid):
        print("Connecting to " + ssid)

        wire.power(True)

        password = ""

        wpa_supplicant_contents = textwrap.dedent("""\
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
country=PT

network={{
    ssid="CasaBatata"
    psk="nopassword"
    priority=1
    id_str="Main wifi"
}}

network={{
    ssid="{ssid}"
    key_mgmt=NONE
    priority=2
    id_str="Moisture sensor"
}}
""").format(ssid=ssid)


        print("Saving WPA supplicant")
        with open('/etc/wpa_supplicant/wpa_supplicant.conf', 'w') as file:
            file.write(wpa_supplicant_contents)

        wire.power(False)
        wire.power(True)
        is_connected = wire.connect(ssid=ssid,password='')

        print("Saved!")

        # Reboot after returning the request response
        # os.system('sudo shutdown -r now')



        # cell = Cell.all('wlan0')[0]
        # try:
        #     scheme = Scheme.for_cell('wlan0', 'home', cell, "")
        #     scheme.save()
        # except Exception as e:
        #     scheme = Scheme.find('wlan0', 'home')
        # scheme.activate()

        return True

    def save_credentials(ip, credentials):
        url = "http://" + ip + "/wifisave?s=" + credentials['ssid'] + "&p=" + credentials['password']
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
