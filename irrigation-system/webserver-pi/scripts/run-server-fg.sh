export LC_ALL=C.UTF-8;
export LANG=C.UTF-8;

# IRSYS_OS=macosx
# IRSYS_OS=pi

IRSYS_RUN_DIR=$(pwd) PYTHONDONTWRITEBYTECODE=1 FLASK_DEBUG=1 FLASK_APP=../main.py FLASK_ENV=development flask run --port 8123 --host=0.0.0.0
