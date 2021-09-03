set -x
set -e

arduinoPath="/Applications/Arduino.app/Contents/MacOS/Arduino"
binPath="~/workspace/arduino/irrigation-system/irrigation-system/webserver-pi/bin"

cd ~/workspace/arduino/irrigation-system/irrigation-system/midgets/controller-valve
rm -rf build
"$arduinoPath" --pref build.path=build --verbose --board esp8266:esp8266:nodemcuv2 --verify *.ino
mv build/controller-valve.ino.bin $binPath/valve/real/valve.bin

# cd ~/workspace/arduino/irrigation-system/irrigation-system/midgets/sensor/midget-moisture
# rm -rf build
# $arduinoPath --pref build.path=build --verbose --board esp8266:esp8266:nodemcuv2 --verify *.ino
# mv build/midget-moisture.ino.bin $binPath/sensor/real/moisture.bin
#
# cd ~/workspace/arduino/irrigation-system/irrigation-system/midgets/sensor/midget-temperature
# rm -rf build
# $arduinoPath --pref build.path=build --verbose --board esp8266:esp8266:nodemcuv2 --verify *.ino
# mv build/midget-temperature.ino.bin $binPath/sensor/real/temperature.bin


echo ""
echo ""
echo "Done!"
