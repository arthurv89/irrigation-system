# import wificontrol
from access_points import get_scanner
from wireless import Wireless
from socket import AF_INET
import network
import netifaces
import requests
from time import sleep
import sys
import traceback
import json
import time
from Generator import Generator


wire = Wireless()

def handle_stream():
    def _run():
        try:
            with open('wifi.txt', 'r') as fh:
                credentials = json.loads(fh.readlines()[0])
                gen = Generator(scan_and_connect(credentials))
                for messages in gen:
                    yield messages
                return gen.value
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
            yield addMessage("Error occurred. Please try again", "error")

    def addMessage(message, status):
        obj["messages"].append(message)
        obj["status"] = status
        return obj

    def scan_and_connect(credentials):
        obj = {}
        yield addMessage("Scanning...", "in-progress")

        ssid = scan()
        if ssid is None:
            yield addMessage("No new sensors found", "error")
            return False

        yield addMessage("Found IRSYS!", "in-progress")
        is_connected = connect(ssid)
        if not is_connected:
            yield addMessage("Couldn't connect!", "error")
            return False

        yield addMessage("Connected to sensor!", "in-progress")
        ip = get_gateway_ip()
        saved = save_credentials(ip, credentials)

        if not saved:
            yield addMessage("Could not save the wifi credentials", "error")
            return False

        yield addMessage("Sensor has Wifi now. It's added to you list of sensors.", "done")

        reconnect_to_original_wifi()

        return True



    def reconnect_to_original_wifi():
        wire.power(False)
        wire.power(True)


    def scan():
        wifi_scanner = get_scanner()
        arr = wifi_scanner.get_access_points()

        for x in arr:
            if x.ssid.startswith("IRSYS-M-"):
                return x.ssid

        return None

    def connect(ssid):
        print("Connecting to " + ssid)

        wire.power(True)
        is_connected = wire.connect(ssid=ssid,password='')
        return is_connected

    def save_credentials(ip, credentials):
        url = "http://" + ip + "/wifisave?s=" + credentials['ssid'] + "&p=" + credentials['password']
        print(url)
        response = requests.get(url)
        html = response.content.decode("utf-8")
        print(type(html))
        print(html)
        return "Credentials Saved" in html


    def get_gateway_ip():
        gws = netifaces.gateways()
        print(gws)
        return gws[AF_INET][0][0]

    obj = {
        "messages": [],
        "status": "in-progress"
    }

    for message in Generator(_run()):
        yield json.dumps(message) + '\n'
