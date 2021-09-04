echo "Press [CTRL+C] to stop.."
endpoint="http://192.168.1.192:8123/api/submit"

while :
do
	r=$RANDOM
	json="{\"deviceId\": \"casabatata-tulips\", \"moisture\": $r}"
	# echo $json
	result=`curl -s \
	  --header "Content-Type: application/json" \
	 	-d "$json" \
		-X POST $endpoint`
	echo $result
	sleep 5
done
