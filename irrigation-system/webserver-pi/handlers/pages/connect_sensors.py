from flask import Flask, request, render_template, jsonify

app = Flask(__name__, template_folder="jinja_templates")

def handle():
    template_context = dict(
        page="connect_sensors"
    )
    return render_template('template.html', **template_context)
