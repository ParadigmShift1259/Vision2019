/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#include "CameraWrapper.h"

CameraWrapper::CameraWrapper()
{
    Camera.set(CV_CAP_PROP_GAIN, 10);
	Camera.set(CV_CAP_PROP_EXPOSURE, 200);
	cout<<"Vision has Started..."<<endl;
	Camera.open();
	cout<<"Camera is opened...\n";
	
	// Warm up camera for 60 frames to stabilize image brightness
	for (int loop = 0; loop<60 ; loop++)
	{
		Camera.grab();
	}
	//Camera.retrieve(m_image);
}

CameraWrapper::~CameraWrapper()
{
    Camera.release();
}

void CameraWrapper::Loop()
{
    Camera.grab();
    //counter += 3;
    Camera.retrieve(m_image);
    
}


