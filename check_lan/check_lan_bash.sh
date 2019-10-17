#! /bin/sh
while true
	do
		if [[ $(wget -q -t 1 --spider --dns-timeout 3 --connect-timeout 10 109.73.14.200:40001; echo $?) -eq 0 ]];
		then echo "green" > /dev/ttyS0
		else echo "red" > /dev/ttyS0
		fi
	sleep 20
done
