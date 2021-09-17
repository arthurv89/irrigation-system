def not_null(var, error):
    if var is None:
        raise Exception("Variable is None. Error: " + error)
    return var
