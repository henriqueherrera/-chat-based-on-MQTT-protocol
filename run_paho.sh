#!/bin/bash    

if ! [ $(id -u) = 0 ]; then
   echo "The script need to be run as root." >&2
   exit 1
fi

if [ $SUDO_USER ]; then
    real_user=$SUDO_USER
else
    real_user=$(whoami)
fi

sudo apt update
sudo apt  install cmake 
sudo apt install mosquitto
sudo apt install mosquitto-clients

sudo apt-get install libssl-dev

git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
make
sudo make install
