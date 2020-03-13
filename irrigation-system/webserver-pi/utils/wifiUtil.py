import subprocess
from utils import db
from wireless import Wireless
import properties
import textwrap
import uuid

wire = Wireless()

def connect_main(main_ssid, main_password):
    return connect(main_ssid, main_password, None)

def connect_moisture_sensor(main_credentials, ssid):
    return connect(main_credentials['ssid'], main_credentials['password'], ssid)


def connect(main_ssid, main_password, ssid=None):
    wire.power(True)
    print("OS:", properties.os)
    if properties.os == "pi":
        wpa_supplicant_contents = textwrap.dedent("""\
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
country=PT

network={{
    ssid="{main_ssid}"
    psk="{main_password}"
    priority=1
    id_str="Main wifi"
}}
""").format(main_ssid=main_ssid, main_password=main_password)

        target_ssid = main_ssid if ssid is None else ssid
        if ssid is not None:
            wpa_supplicant_contents += textwrap.dedent("""\

network={{
    ssid="{ssid}"
    key_mgmt=NONE
    priority=2
    id_str="Moisture sensor"
}}""").format(ssid=ssid)

        print("Saving WPA supplicant")
        with open('/etc/wpa_supplicant/wpa_supplicant.conf', 'w') as file:
            file.write(wpa_supplicant_contents)
        print("Saved!")

        print("Connecting to SSID using Pi script")
        f = open("/tmp/reset_wifi_pi.log", "w")
        print("TARGET SSID", target_ssid)
        rc = subprocess.run([properties.working_directory + "/scripts/reset_wifi_pi.sh", target_ssid], stdout=f)
        print("Done")
        return True
    elif properties.os == "macosx":
        print("Connecting to SSID", ssid, "using Mac OS command")
        return wire.connect(ssid=ssid,password='')
        # for i in range(1, 10):
        #     cur = wire.current()
        #     print("Current: ", cur)
        #     if cur == ssid:
        #         print("GOT IT!")
        #         return True
        #     sleep(1)
        # return False
    else:
        raise "Could not determine OS."
