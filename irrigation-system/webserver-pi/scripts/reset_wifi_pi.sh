#!/bin/bash
sensor_ssid=$1
echo "====== STARTING RESET WIFI SCRIPT ======"
echo "SENSOR SSID: $sensor_ssid"

for i in {0..10}
do
  echo "Stopping WIFI"
  sudo killall wpa_supplicant
  echo "Stopped WIFI"

  echo "Starting WIFI"
  output=$(sudo wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant.conf)
  echo "Output: $output"
  while [[ "$output" == *"Failed to initialize control interface"* ]]
  do
    echo "Trying again."
    output=$(sudo wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant.conf)
    echo "Output: $output"
  done
  echo "Started WIFI"

  echo "Waiting to connect to network"
  j=0
  max=$((200*$i))
  echo "MAX=$max, i=$i"

  current_ssid=$(iwgetid -r)
  for (( j=0; j<=$max; j++ ))
  do
    current_ssid=$(iwgetid -r)
    echo "Sensor SSID=$sensor_ssid, current_ssid=$current_ssid"
    if [[ "$current_ssid" == "$sensor_ssid" ]]; then
      echo "DONE. Connected to $sensor_ssid"
      exit 0
    fi
    sleep 0.1
  done

  echo "======= COULD NOT CONNECT YET TO NETWORK ======="
done
echo "======= FINISHED WITHOUT SUCCESS!!! ======="
