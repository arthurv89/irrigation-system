SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
WEBSERVER_PATH="$SCRIPT_DIR/../../../irrigation-system/webserver-pi/"

cd $WEBSERVER_PATH

export LC_ALL=C.UTF-8;
export LANG=C.UTF-8;

# IRSYS_OS=macosx
# IRSYS_OS=pi

IRSYS_RUN_DIR=WEBSERVER_PATH PYTHONDONTWRITEBYTECODE=1 FLASK_DEBUG=1 FLASK_APP=$(pwd)/main.py FLASK_ENV=development flask run --port 8123 --host=0.0.0.0
