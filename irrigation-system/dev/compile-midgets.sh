set -x
set -e

base="/Users/swipecrowd/workspace/arduino/irrigation-system/irrigation-system"
arduinoPath="/Applications/Arduino.app/Contents/MacOS/Arduino"
binPath="$base/webserver-pi/bin"
echo $binPath

cd "$base/midgets/controller-valve"
rm -rf build
"$arduinoPath" --pref build.path=build --verbose --board esp8266:esp8266:nodemcuv2 --verify *.ino
mv build/controller-valve.ino.bin $binPath/valve/real/valve.bin

# cd $base/midgets/sensor/midget-moisture
# rm -rf build
# $arduinoPath --pref build.path=build --verbose --board esp8266:esp8266:nodemcuv2 --verify *.ino
# mv build/midget-moisture.ino.bin $binPath/sensor/real/moisture.bin
#
# cd $base/midgets/sensor/midget-temperature
# rm -rf build
# $arduinoPath --pref build.path=build --verbose --board esp8266:esp8266:nodemcuv2 --verify *.ino
# mv build/midget-temperature.ino.bin $binPath/sensor/real/temperature.bin


echo ""
echo ""
echo "Done!"
