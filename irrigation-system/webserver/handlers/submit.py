import json
from flask import request

def handle():
    x = request.form["input1"]

    ret = {
        "input": x
    }
    return json.dumps(ret)
