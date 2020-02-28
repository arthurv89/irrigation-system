echo "Press [CTRL+C] to stop.."
endpoint="http://192.168.1.3:8123/submit"

while :
do
	r=$RANDOM
	json="{\"id\": 1, \"deviceId\": \"casabatata-roses\", \"moisture\": $r}"
	# echo $json
	result=`curl -s \
	  --header "Content-Type: application/json" \
	 	-d "$json" \
		-X POST $endpoint`
	echo $result
	sleep 5
done
