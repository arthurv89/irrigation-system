# Readme

## Folders

Folders: controller-2 is the code that runs on the Arduino.

webserver is code that you can run on a mac (or Raspberry Pi) that can receive data from the Arduino.


## How to run

### Setup Elasticsearch

https://github.com/elastic/homebrew-tap

Elasticsearch: http://localhost:9200/
(make sure you have version >= 7.x)

Kibana: http://localhost:5601/status#?_g=()



### Setup server & client

cd webserver-pi/ && bash run-server.sh
cd webserver-pi/ && bash run-client.sh
