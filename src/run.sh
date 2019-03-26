#!/bin/bash
sleep 30
cd /home/pi/Vision2019/src
while :
do
  echo "changing camera settings"
v4l2-ctl -d /dev/video0 -cauto_exposure=1
v4l2-ctl -d /dev/video0 -cexposure_time_absolute=40
  echo "starting program"
  /home/pi/Vision2019/src/VisionProcessing
done
