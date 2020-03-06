# uwsgi --http 127.0.0.1:8000 --wsgi-file main.py --callable app
FLASK_APP=main.py FLASK_ENV=development flask run --port 8123 --host=0.0.0.0
