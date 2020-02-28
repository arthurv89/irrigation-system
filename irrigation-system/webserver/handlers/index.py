import json
from flask import Flask, request, render_template

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    template_context = dict(name="World")
    return render_template('index.html', **template_context)
