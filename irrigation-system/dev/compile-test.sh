set -x
set -e

cd ~/workspace/arduino/test/HTTPServerOTA
rm -rf build
"/Users/arthur/Downloads/Arduino 2.app/Contents/MacOS/Arduino" --pref build.path=build --verbose --board esp8266:esp8266:nodemcu --verify *.ino
# mv build/HTTPServerOTA.ino.bin ~/workspace/arduino/irrigation-system/webserver-pi/bin/blink/real/fast.bin
mv build/HTTPServerOTA.ino.bin ~/workspace/arduino/irrigation-system/webserver-pi/bin/blink/real/slow.bin


echo ""
echo ""
echo "Done!"
