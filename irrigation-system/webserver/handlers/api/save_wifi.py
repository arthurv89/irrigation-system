from flask import Flask, request, render_template, jsonify
import json

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    ssid = request.form.get('ssid')
    username = request.form.get('username')
    password = request.form.get('password')

    credentials = {
        "ssid": ssid or "",
        "username": username or "",
        "password": password or ""
    }

    with open('wifi.txt', 'w') as file:
        file.write(json.dumps(credentials))
        return {}
