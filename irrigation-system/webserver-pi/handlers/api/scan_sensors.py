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
from wifi import Cell, Scheme
import textwrap

wire = Wireless()

prefix = "IRSYS-M-"

def handle_stream():
    def _run():
        print("Run method")
        try:
            with open('wifi.txt', 'r') as fh:
                credentials = json.loads(fh.readlines()[0])
                gen = Generator(scan_and_connect(credentials))
                for messages in gen:
                    print(messages)
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

    def scan_and_connect(credentials):
        yield addMessage("Scanning...", "in-progress")

        cell = find_cell()
        if cell is None:
            yield addMessage("No new sensors found", "error")
            return False


        yield addMessage("Found sensor with name " + cell.ssid, "in-progress")
        is_connected = connect(cell)
        if not is_connected:
            yield addMessage("Couldn't connect to sensor", "error")
            return False

        yield addMessage("Connected to sensor!", "in-progress")
        ip = get_gateway_ip()
        saved = save_credentials(ip, credentials)

        if not saved:
            yield addMessage("Could not save the wifi credentials", "error")
            return False

        yield addMessage("Sensor has Wifi now. It's added to you list of sensors.", "done")

        reconnect_to_original_wifi()

        association_to_es(ssid)

        return True


    def find_cell():
        for i in range(0, 5):
            cells = scan()
            cell = filter(cells)
            if cell is not None:
                return cell
            print("Couldn't find it yet, trying again in 3 seconds...")
            sleep(3)
        return None

    def association_to_es(device_id):
        # We could have multiple moisture sensors for a valve (or other way around) in the future
        db.write_sensor_association(device_id, int(time.time()))
        print("Written assocation of sensor " + device_id + " to the database")



    def reconnect_to_original_wifi():
        wire.power(False)
        wire.power(True)


    def scan():
        print("SCANNNNN")
        wifi_scanner = get_scanner()
        print(wifi_scanner)
        arr = wifi_scanner.get_access_points()
        print(arr)

        cells = Cell.all('wlan0')
        print(list(cells))

        print(next(iter(list(cells) or []), None))
        wifi_list = [cell for cell in Cell.all('wlan0')]
        cells = [cell for cell in wifi_list]
        # wifi_to_connect = [cell for cell in wifi_list if cell.ssid.startswith(prefix)][0]
        # print(wifi_to_connect)


        # for x in arr:
        #     if x.ssid.startswith(prefix):
        #         return x.ssid

        return cells

    def filter(cells):
        wifi_to_connect = [cell for cell in cells if cell.ssid.startswith(prefix)]
        if len(wifi_to_connect) > 0:
            return wifi_to_connect[0]
        else:
            return None


    def connect(cell):
        print("Connecting to " + cell.ssid)

        wire.power(True)

        ssid = cell.ssid
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

        # is_connected = wire.connect(ssid=ssid,password='')
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
        gws = netifaces.gateways()
        for i in range(0, 10):
            if AF_INET in gws:
                print(gws)
                return gws[AF_INET][0][0]
            print("Sleeping...")
            sleep(1)

    def addMessage(message, status):
        obj["messages"].append({
            "message": message,
            "status": status})
        return obj

    obj = {
        "messages": []
    }

    print("Starting...s")
    for obj in Generator(_run()):
        print("Ran the generator")
        yield json.dumps(obj) + '\n'
