def handle():
    # Pins connected: D1, D2, D3 = 5, 4, 0
    ret = {
        "open": [
            open_obj(0, 10000),
            open_obj(1, 10000),
            open_obj(2, 10000),
            # open_obj(1, 1000)
        ]
    }
    return ret

def open_obj(valve, time):
    return {
        "valve": valve,
        "time": time
    }
