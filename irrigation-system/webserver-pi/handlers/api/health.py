from flask import request

def handle():
    ret = {
        "health": "green"
    }
    return ret