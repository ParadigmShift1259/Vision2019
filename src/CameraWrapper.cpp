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
    m_Camera.set(CV_CAP_PROP_GAIN, 30);				// TODO unsupported with VideoCapture?
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
		static int count = 0;

#ifdef TEST_GAFFER_TAPE_ALIGNMENT_IMGS
		char fileName[100];
#ifdef BUILD_ON_WINDOWS
		const char* c_testImagePath = "C:/Users/Developer/Documents/TestData/Input/";
		sprintf_s<sizeof(fileName)>(fileName, "%salignment_line%d.jpg", c_testImagePath, count % 34 + 1);
#else
		sprintf(fileName, "alignment_line%d.jpg", count % 34 + 1);
#endif

#else	// if TEST_GAFFER_TAPE_ALIGNMENT_IMGS

		vector<string> files
		{
			  "RocketPanelStraightDark16in.jpg"
			, "RocketPanelStraightDark24in.jpg"
			, "RocketPanelStraightDark36in.jpg"
			, "RocketPanelStraightDark48in.jpg"
			, "RocketPanelStraightDark60in.jpg"
			, "RocketPanelStraightDark72in.jpg"
			, "RocketPanelStraightDark96in.jpg"
			, "RocketStraightDark96in.jpg"
			, "RocketPanelStraightDark12in.jpg"

			, "CargoSideStraightDark36in.jpg"
			, "CargoSideStraightDark60in.jpg"
			, "CargoSideStraightDark72in.jpg"
			, "CargoSideStraightPanelDark36in.jpg"
			, "CargoStraightDark19in.jpg"
			, "CargoStraightDark24in.jpg"
			, "CargoStraightDark48in.jpg"
			, "CargoStraightDark72in.jpg"
			, "CargoStraightDark90in.jpg"
		};

#ifdef BUILD_ON_WINDOWS
		const char* c_testImagePath = "C:/Users/Developer/Documents/TestData/Input/";
		string fileName = c_testImagePath + files[count % files.size()];
#else
		string fileName = files[count % files.size()];
#endif

#endif	// if TEST_GAFFER_TAPE_ALIGNMENT_IMGS

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


