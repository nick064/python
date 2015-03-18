#!/bin/bash  
TZ=
# Define a timestamp function
timestamp() {
  date +"%d.%m.%y - %T"
}

ping -c 1 -W 1 "77.70.12.217:66" > /dev/null

if [ "$?" -eq 0 ] ; then
    echo "Dbad1@Sofia is up and running."
else
    echo "$(timestamp): Dbad1@Sofia is down from here." >> /var/log/hosts_status
fi

ping -c 1 -W 1 "212.233.156.56" > /dev/null

if [ "$?" -eq 0 ] ; then
    echo "Home@Pleven is up and running."
else
    echo "$(timestamp): Home@Pleven is down from here." >> /var/log/hosts_status
fi




#77.70.12.217 66
#81.209.165.58 80
