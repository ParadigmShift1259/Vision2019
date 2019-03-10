v4l2-ctl -d /dev/video0 -cauto_exposure=1
v4l2-ctl -d /dev/video0 -cexposure_time_absolute=50
./VisionProcessing
