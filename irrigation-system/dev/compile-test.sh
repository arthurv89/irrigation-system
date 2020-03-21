set -x
set -e

cd ~/workspace/arduino/irrigation-system/midgets/controller-moisture
rm -rf build
"/Users/arthur/Downloads/Arduino 2.app/Contents/MacOS/Arduino" --pref build.path=build --verbose --board esp8266:esp8266:nodemcu --verify *.ino
mv build/controller-moisture.ino.bin ~/workspace/arduino/irrigation-system/webserver-pi/bin/sensor/moisture.bin


echo ""
echo ""
echo "Done!"
