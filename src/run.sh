#!/bin/bash
cd /home/pi/Vision2019/src
while :
do
  echo "changin camera settings"
v4l2-ctl -d /dev/video0 -cauto_exposure=1
v4l2-ctl -d /dev/video0 -cexposure_time_absolute=50
  echo "starting program"
  /home/pi/Vision2019/src/VisionProcessing
done
