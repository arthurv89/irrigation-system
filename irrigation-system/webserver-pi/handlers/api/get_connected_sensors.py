from utils import db

def handle():
    networks = db.get_connected_sensors()
    ret = {
        "networks": networks
    }
    return ret
