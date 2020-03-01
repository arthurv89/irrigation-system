from flask import Flask, request, render_template, jsonify

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    return {"result": 1}
