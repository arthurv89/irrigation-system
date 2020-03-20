import json
from flask import request

def handle():
    ret = {
        "controller_addr": {
            "ip": "192.168.1.3",
            "port": 8123
        }
    }
    return ret
