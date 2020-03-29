set -x
set -e

cd ~/workspace/arduino/irrigation-system/midgets/controller-valve
rm -rf build
"/Users/arthur/Downloads/Arduino 2.app/Contents/MacOS/Arduino" --pref build.path=build --verbose --board esp8266:esp8266:nodemcu --verify *.ino
mv build/controller-valve.ino.bin ~/workspace/arduino/irrigation-system/webserver-pi/bin/valve/real/valve.bin

cd ~/workspace/arduino/irrigation-system/midgets/sensor/midget-moisture
rm -rf build
"/Users/arthur/Downloads/Arduino 2.app/Contents/MacOS/Arduino" --pref build.path=build --verbose --board esp8266:esp8266:nodemcu --verify *.ino
mv build/midget-moisture.ino.bin ~/workspace/arduino/irrigation-system/webserver-pi/bin/sensor/real/moisture.bin

cd ~/workspace/arduino/irrigation-system/midgets/sensor/midget-temperature
rm -rf build
"/Users/arthur/Downloads/Arduino 2.app/Contents/MacOS/Arduino" --pref build.path=build --verbose --board esp8266:esp8266:nodemcu --verify *.ino
mv build/midget-temperature.ino.bin ~/workspace/arduino/irrigation-system/webserver-pi/bin/sensor/real/temperature.bin


echo ""
echo ""
echo "Done!"
