echo "Press [CTRL+C] to stop.."
while :
do
	result=`curl -s -d "input1=value1" -X POST http://192.168.1.3:8000/submit`
	echo $result
	sleep 1
done
