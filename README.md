# Vision2019
Destination Deep Space
Vision target tracking code for the Raspberry Pi

Prerequisites:

    Install latest version of FRCVision
		https://github.com/wpilibsuite/FRCVision-pi-gen/releases
		This Raspbian-based Raspberry Pi image includes C++, Java, and Python libraries required for vision coprocessor development for FRC (e.g. opencv, cscore, ntcore, robotpy-cscore, Java 11, etc).
		Step by step instructions here:
		https://wpilib.screenstepslive.com/s/currentCS/m/85074/l/1027260-installing-the-image-to-your-microsd-card
		https://wpilib.screenstepslive.com/s/currentCS/m/85074/l/1027253-what-you-need-to-get-the-pi-image-running

	Boot the Pi and connect to the laptop
	On the laptop do the following:
		Download and install Putty (https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html)
		Download and install WinSCP (https://winscp.net/eng/index.php)
		Open command prompt
		run "ping frcvision.local"
		If you get response in packets then that means the Pi is connected properly
		Log into Putty using frcvision.local port 22 using SSH (not Telnet)
		If warning for data breach shows up, answer yes
		Log into the pi with username pi and password raspberry 
		Run WinScp
		Connect to frcvision.local
		
	In at the frcvision.local site created by the pi do the following:
		Open Vision Settings
		Set the team number to 1259
		Delete the Pi Cam from the execution routine
		
	In the Putty SSH shell execute the following:
		sudo raspi-config:
			Change keyboard settings to U.S
			Change local and time zone to central
			Locate and enable ssh and camera
			Exit out of configuration and reboot using sudo shutdown -r now
		Set the file system to read/write if the promt ends with (ro):
			become the super user withsudo -s
			cd etc
			remount the file system with mount -o remount rw root
			edit the fstab file with nano fstab
				change ...defaults.ro.noatime
				to           ...defaults.rw.noatime
			leave /boot as read only
			reboot using sudo shutdown -r now
			the promt should now end with (rw):
			
	Avoid corrupting the SD card; before powering down the Pi, halt the OS with sudo shutdown -h now
		

