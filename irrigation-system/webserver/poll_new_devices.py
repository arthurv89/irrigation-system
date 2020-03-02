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



wire = Wireless()

class Generator:
    def __init__(self, gen):
        self.gen = gen

    def __iter__(self):
        self.value = yield from self.gen


def run():
    for message in _run():
        print(json.dumps(message))
        time.sleep(.3)
        yield json.dumps(message) + '\n'

def _run():
    try:
        gen = Generator(loop())
        for messages in gen:
            yield messages
        if gen.value:
            return
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

def loop():
    obj = {}
    setAttr(obj, "message", "Scanning...")
    setAttr(obj, "status", "in-progress")
    yield obj

    has_found = scan()
    if not has_found:
        setAttr(obj, "message", "No new sensors found")
        setAttr(obj, "status", "error")
        yield obj
        return True

    setAttr(obj, "message", "Found IRSYS!")
    setAttr(obj, "status", "in-progress")
    yield obj
    is_connected = connect()
    if not is_connected:
        setAttr(obj, "message", "Couldn't connect!")
        setAttr(obj, "status", "error")
        yield obj
        return False

    setAttr(obj, "message", "Connected to sensor!")
    setAttr(obj, "status", "in-progress")
    ip = get_gateway_ip()
    saved = save_credentials(ip)

    if not saved:
        setAttr(obj, "message", "Could not save the wifi credentials")
        setAttr(obj, "status", "error")
        yield obj

    setAttr(obj, "message", "Sensor has Wifi now")
    setAttr(obj, "status", "done")
    reconnect_to_original_wifi()

    return True


def setAttr(obj, key, value):
    obj[key] = value

def reconnect_to_original_wifi():
    power = wire.power(False)
    print(is_connected)

    power = wire.power(True)
    print(is_connected)


def scan():
    wifi_scanner = get_scanner()
    arr = wifi_scanner.get_access_points()

    for x in arr:
        if(x.ssid == "IRSYS"):
            return True

    return False

def connect():
    print("Connecting to IRSYS")

    is_connected = wire.connect(ssid='IRSYS',password='')
    return is_connected

# def get_current_wifi_ssid():
#     sta = WLAN(network.STA_IF)
#     return sta.config('essid')

def save_credentials(ip):
    url = "http://" + ip + "/wifisave?s=CasaBatata&p=nopassword"
    print(url)
    response = requests.get(url)
    html = response.content.decode("utf-8")
    print(type(html))
    print(html)
    return "Credentials Saved" in html


def get_gateway_ip():
    gws = netifaces.gateways()
    return gws[AF_INET][0][0]

run()
