from flask import Flask, request, render_template, jsonify
import json
import textwrap
import os

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    ssid = request.form.get('ssid')
    password = request.form.get('password')

    credentials = {
        "ssid": ssid or "",
        "password": password or ""
    }

    print("Saving wifi.txt")
    with open('wifi.txt', 'w') as file:
        file.write(json.dumps(credentials))

    wpa_supplicant_contents = textwrap.dedent("""\
        ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
        update_config=1
        country=PT

        network={{
               ssid="{ssid}"
               psk="{psk}"
               key_mgmt=WPA-PSK
        }}""").format(ssid=ssid, psk=password)

    print("Saving WPA supplicant")
    with open('/etc/wpa_supplicant/wpa_supplicant.conf', 'w') as file:
        file.write(wpa_supplicant_contents)

    print("Saved!")

    # Reboot after returning the request response
    os.system('sudo shutdown -r now')



    return {}
