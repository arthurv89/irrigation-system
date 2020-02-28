# import wificontrol
from access_points import get_scanner
from wireless import Wireless
from socket import AF_INET
import network
import netifaces
import requests
from time import sleep

wire = Wireless()


def run():
    while True:
        # print(network)
        # wifi = wificontrol.WiFiControl()
        # WiFiControl().turn_off_wifi()
        # WiFiControl().turn_on_wifi()
        try:
            loop()
        except e:
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
        sleep(1)

def loop():
    # current_wifi_ssid = get_current_wifi_ssid()
    # println("Current SSID: " + current_wifi_ssid)

    print("Scanning...")
    has_found = scan()
    if not has_found:
        print("No new sensors found")
        return

    print("Found IRSYS!")
    is_connected = connect()
    if not is_connected:
        print("Couldn't connect!")
        return

    print("Connected!")
    ip = get_gateway_ip()
    saved = save_credentials(ip)

    if not saved:
        print("Could not save the wifi credentials")

    print("Sensor has Wifi now")
    reconnect_to_original_wifi()

    print("Done")


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
