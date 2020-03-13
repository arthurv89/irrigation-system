from utils import db

def handle():
    networks = db.get_connected_sensors()
    # print("NETWORKS ", networks)
    ret = {
        "networks": networks
    }
    return ret
