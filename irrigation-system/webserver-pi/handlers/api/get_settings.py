import json
import boto3
from flask import request

def handle():
    ret = {
        "controller_ip": "192.168.1.3:8123"
    }
    return ret
