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
		//string fileName = "image.jpg";
		//string fileName = "alignment_line3.jpg";
		static int count = 1;
		char fileName[100];
        sprintf(fileName, "alignment_line%d.jpg", count % 34 + 1);
		count++;
		
		m_image = imread(fileName, CV_LOAD_IMAGE_COLOR);

		if (m_image.empty())
		{
			cout << "Cannot find the specified file name: " << fileName << endl;
		}
	}
	else
	{
		m_Camera.grab();
    	m_Camera.retrieve(m_image);
	}
}


