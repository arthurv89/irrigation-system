set -x
set -e

datetime=`date '+%d-%m-%Y_%H-%M-%S'`
version="$datetime"


base="/Users/swipecrowd/workspace/arduino/irrigation-system/irrigation-system"
arduinoPath="/Applications/Arduino.app/Contents/MacOS/Arduino"
binPath="$base/webserver-pi/bin"
target="$binPath/valve/valve-$version.bin"


echo "#include \"Version.h\"

String getVersion() {
  return \"$version\";
}
" > $base/midgets/controller-valve/Version.cpp

cd "$base/midgets/controller-valve"
"$arduinoPath" --pref build.path=build --board esp8266:esp8266:nodemcuv2 --verify *.ino
cp build/controller-valve.ino.bin $target

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
