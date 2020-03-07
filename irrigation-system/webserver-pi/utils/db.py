import mysql.connector as mariadb

mariadb_connection = mariadb.connect(user='irsys', password='waterme', database='irsys')
cursor = mariadb_connection.cursor()

def moisture_values_per_device_per_hour():
    return []
    # cursor.execute("SELECT first_name,last_name FROM employees WHERE first_name=%s", (some_name,))


def put_moisture_value():
    print("")
