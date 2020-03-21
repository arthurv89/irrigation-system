set -x
set -e

cd ~/workspace/arduino/irrigation-system/midgets/controller-moisture
if [ -d "build" ]; then
  rm -rf build
fi
mkdir build
arduino-cli compile  -b esp8266:esp8266:nodemcu --output build/
# mv build/build.elf ~/workspace/arduino/irrigation-system/webserver-pi/bin/real/moisture.bin

cd ~/workspace/arduino/irrigation-system/midgets/controller-temperature
if [ -d "build" ]; then
  rm -rf build
fi
mkdir build
arduino-cli compile  -b esp8266:esp8266:nodemcu --output build/
# mv build/build.elf ~/workspace/arduino/irrigation-system/webserver-pi/bin/real/temperature.bin


echo ""
echo ""
echo "Done!"
