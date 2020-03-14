sudo chmod +x ~/workspace/irrigation-system/webserver-pi/scripts/reset_wifi_pi.sh && \
sudo chmod 777 /etc/wpa_supplicant/wpa_supplicant.conf && \
cd /home/pi/workspace/irrigation-system/webserver-pi/scripts/ && \
nohup bash run-server-fg.sh > /dev/null 2>&1 & disown
