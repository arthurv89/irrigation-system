sudo mysql_secure_installation
sudo mysql -u root -p

```sql
CREATE DATABASE irsys;
CREATE USER 'irsys'@'localhost' IDENTIFIED BY 'waterme';
GRANT ALL PRIVILEGES ON irsys.* TO 'irsys'@'localhost';

GRANT ALL PRIVILEGES ON irsys.* TO 'phpmyadmin'@'localhost';

# Create tables
CREATE TABLE `moisture` (
  `id` binary(16) NOT NULL,
  `deviceId` text NOT NULL,
  `time` datetime NOT NULL,
  `owner` varchar(50) NOT NULL,
  `moisture` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


```

# Go to URL in browser:
# http://192.168.1.11/phpmyadmin/
```
User: phpmyadmin
Password: [whatever you put in the install]

Go to the "SQL" tab.
```
