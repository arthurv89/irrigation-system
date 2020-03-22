from flask import request

def get_params():
    return request.get_json()['params']
