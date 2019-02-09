/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#include "CameraWrapper.h"
#include "Const.h"

CameraWrapper::CameraWrapper()
{
    m_Camera.set(CV_CAP_PROP_GAIN, 30);
	m_Camera.set(CV_CAP_PROP_EXPOSURE, 200);
	cout << "Vision has Started" << endl;
	m_Camera.open();
	cout << "Camera is opened" << endl;
	
	// Warm up camera for 60 frames to stabilize image brightness
	for (int loop = 0; loop < 60 ; loop++)
	{
		m_Camera.grab();
	}
	m_Camera.retrieve(m_image);
}

CameraWrapper::~CameraWrapper()
{
    m_Camera.release();
}

void CameraWrapper::AcquireImage()
{
	if (c_bUseLastDiagImage)
	{
		// Reread the previously saved image to consistently process the same image
		m_image = imread("image.bmp", CV_LOAD_IMAGE_COLOR);
	}
	else
	{
		m_Camera.grab();
    	m_Camera.retrieve(m_image);
	}
}


