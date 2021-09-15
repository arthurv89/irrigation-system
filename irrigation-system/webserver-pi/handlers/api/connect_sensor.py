from access_points import get_scanner
import logging
from flask import Flask, request
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
        logging.debug("Run method")
        try:
            credentials = db.get_wifi_credentials()
            scan_and_connect(credentials)
        except:
            exc_type, exc_obj, exc_tb = sys.exc_info()
            fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
            logging.error(exc_type, fname, exc_tb.tb_lineno)

            exc_type, exc_value, exc_traceback = sys.exc_info()
            logging.debug("*** print_tb:")
            traceback.print_tb(exc_traceback, limit=1, file=sys.stdout)
            logging.debug("*** print_exception:")
            # exc_type below is ignored on 3.5 and later
            traceback.print_exception(exc_type, exc_value, exc_traceback,
                                      limit=2, file=sys.stdout)
            logging.debug("*** print_exc:")
            traceback.print_exc(limit=2, file=sys.stdout)
            logging.debug("*** format_exc, first and last line:")
            formatted_lines = traceback.format_exc().splitlines()
            logging.debug(formatted_lines[0])
            logging.debug(formatted_lines[-1])
            logging.debug("*** format_exception:")
            # exc_type below is ignored on 3.5 and later
            logging.debug(repr(traceback.format_exception(exc_type, exc_value,
                                                  exc_traceback)))
            logging.debug("*** extract_tb:")
            logging.debug(repr(traceback.extract_tb(exc_traceback)))
            logging.debug("*** format_tb:")
            logging.debug(repr(traceback.format_tb(exc_traceback)))
            logging.debug("*** tb_lineno:", exc_traceback.tb_lineno)
            logging.debug("Error occurred. Please try again")

    def scan_and_connect(main_wifi_credentials):
        logging.debug(request.form['ssid'])
        logging.debug("main_wifi_credentials: " + json.dumps(main_wifi_credentials))
        new_ssid = request.form['ssid']
        hasSsid = find_cell(new_ssid)

        if not hasSsid:
            logging.debug("Could not find network with SSID " + new_ssid)
            return False


        logging.debug("Found sensor with name " + new_ssid)
        is_connected = wifiUtil.connect_moisture_sensor(main_wifi_credentials, new_ssid)
        logging.debug(is_connected)
        if not is_connected:
            logging.debug("Couldn't connect to sensor")
            return False

        logging.debug("Connected to sensor!")
        ip = get_gateway_ip()
        if ip is None:
            logging.debug("Coulnd't find IP Gateway")
            return False

        saved = save_credentials(ip, main_wifi_credentials)

        if not saved:
            logging.debug("Could not save the wifi credentials")
            return False

        logging.debug("Sensor has Wifi now. It's added to you list of sensors.")

        wifiUtil.connect_main(main_wifi_credentials['ssid'], main_wifi_credentials['password'])

        association_to_db(new_ssid)

        return True


    def find_cell(ssid):
        # logging.debug("FINDING CELL")
        for i in range(0, 5):
            hasSsid = scan(ssid)
            if hasSsid:
                return True
            logging.debug("Couldn't find it yet, trying again in 3 seconds...")
            sleep(3)
        return False

    def association_to_db(device_id):
        # We could have multiple moisture sensors for a valve (or other way around) in the future
        db.write_sensor_association(device_id, int(time()))
        logging.debug("Written assocation of sensor " + device_id + " to the database")




    def scan(ssid):
        # logging.debug("SCANNNNN")
        wifi_scanner = get_scanner()
        # logging.debug(wifi_scanner)
        arr = wifi_scanner.get_access_points()
        logging.debug(arr)
        for x in arr:
            if x.ssid == ssid:
                return True

        return False


    def save_credentials(ip, main_wifi_credentials):
        url = "http://" + ip + "/wifisave?s=" + main_wifi_credentials['ssid'] + "&p=" + main_wifi_credentials['password']
        logging.debug(url)
        response = requests.get(url)
        html = response.content.decode("utf-8")
        logging.debug(type(html))
        logging.debug(html)
        return "Credentials Saved" in html


    def get_gateway_ip():
        for i in range(0, 10):
            gws = netifaces.gateways()
            logging.debug(gws)
            if AF_INET in gws:
                return gws[AF_INET][0][0]
            logging.debug("Sleeping...")
            sleep(1)
        return None

    _run()
    return {}
