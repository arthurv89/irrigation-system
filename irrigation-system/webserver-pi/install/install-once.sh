sudo mysql_secure_installation
sudo mysql -u root -p

```sql
CREATE DATABASE irsys;
CREATE USER 'irsys'@'localhost' IDENTIFIED BY 'Waterme1!@#';
GRANT ALL PRIVILEGES ON irsys.* TO 'irsys'@'localhost';

# GRANT ALL PRIVILEGES ON irsys.* TO 'phpmyadmin'@'localhost';

# Create tables
CREATE TABLE irsys.`sensor_values` (
  `id` binary(16) NOT NULL,
  `deviceId` varchar(50) NOT NULL,
  `time` timestamp NOT NULL,
  `owner` varchar(50) NOT NULL,
  `type` varchar(50) NOT NULL,
  `value` DECIMAL(6, 2) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE irsys.`sensors` (
  `id` binary(16) NOT NULL,
  `deviceId` text NOT NULL,
  `time` timestamp NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


CREATE TABLE irsys.`open_times` (
  `id` binary(16) NOT NULL,
  `hose_id` varchar(50) NOT NULL,
  `time` timestamp NOT NULL,
  `opening_time` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


CREATE TABLE irsys.`wifi` (
  `ssid` varchar(60) NOT NULL,
  `password` varchar(60) NOT NULL,
  PRIMARY KEY (`ssid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


DROP TABLE valve_connector;
CREATE TABLE irsys.`valve_connector` (
  `id` binary(16) NOT NULL,
  `valve_id` varchar(60) NOT NULL,
  `hose_id` varchar(60) NOT NULL,
  `hose_position` int(5) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE (valve_id, hose_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


```

# Go to URL in browser:
# http://192.168.1.11/phpmyadmin/
```
User: phpmyadmin
Password: [whatever you put in the install]

Go to the "SQL" tab.
```
