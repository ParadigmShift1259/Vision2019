/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#include "CameraWrapper.h"
#include "Const.h"

std::vector<std::string> testFiles
{
	//  "RocketPanelStraightDark16in.jpg"
	//, "RocketPanelStraightDark24in.jpg"
	//, "RocketPanelStraightDark36in.jpg"
	//, "RocketPanelStraightDark48in.jpg"
	//, "RocketPanelStraightDark60in.jpg"
	//, "RocketPanelStraightDark72in.jpg"
	//, "RocketPanelStraightDark96in.jpg"
	//, "RocketStraightDark96in.jpg"
	//, "RocketPanelStraightDark12in.jpg"
	//, "RocketPanelStraightDark24inLeftOnly.jpg"
	//, "RocketPanelStraightDark24inRightOnly.jpg"

	//, "CargoSideStraightDark36in.jpg"
	//, "CargoSideStraightDark60in.jpg"
	//, "CargoSideStraightDark72in.jpg"
	//, "CargoSideStraightPanelDark36in.jpg"
	//, "CargoStraightDark19in.jpg"
	//, "CargoStraightDark24in.jpg"
	//, "CargoStraightDark48in.jpg"
	//, "CargoStraightDark72in.jpg"
	//, "CargoStraightDark90in.jpg"

	  "im_12in_l_nlen.jpg"
	, "im_18in_l_nlen.jpg"
	, "im_24in_l_nlen.jpg"
	, "im_30in_l_nlen.jpg"
	, "im_36in_45deg_l_nlen.jpg"
	, "im_36in_m45deg_l_nlen.jpg"
	, "im_48in_l_nlen.jpg"
	, "im_60in_45deg_l_nlen.jpg"
	, "im_60in_m45deg_l_nlen.jpg"
	, "im_72in_l_nlen.jpg"
	, "im_96in_l_nlen.jpg"
	//, "im_ball40in_l_nlen.jpg"
	//, "im_ball60in_l_nlen.jpg"
	//, "im_hatch40in_1_l_nlen.jpg"
	//, "im_hatch40in_2_l_nlen.jpg"
	//, "im_hatch60in_1_l_nlen.jpg"
	//, "im_hatch60in_2_l_nlen.jpg"
};

std::vector<float> testDist	// Keep in sync with testFile vector
{
	//  16.0
	//, 24.0
	//, 36.0
	//, 48.0
	//, 60.0
	//, 72.0
	//, 96.0
	//, 96.0
	//, 12.0
	//, 24.0
	//, 24.0

	//, 36.0
	//, 60.0
	//, 72.0
	//, 36.0
	//, 19.0
	//, 24.0
	//, 48.0
	//, 72.0
	//, 90.0

	  12
	, 18
	, 24
	, 30
	, 36
	, 36
	, 48
	, 60
	, 60
	, 72
	, 96
	//, 40
	//, 60
	//, 40
	//, 40
	//, 60
	//, 60
};

CameraWrapper::CameraWrapper()
{
	cout << "CV_CAP_PROP_FORMAT: " << m_Camera.get(CV_CAP_PROP_FORMAT) << endl;
	//cout << "CV_CAP_PROP_GAIN: " << m_Camera.get(CV_CAP_PROP_GAIN) << endl;
	cout << "CV_CAP_PROP_EXPOSURE: " << m_Camera.get(CV_CAP_PROP_EXPOSURE) << endl;
	cout << "CV_CAP_PROP_BRIGHTNESS: " << m_Camera.get(CV_CAP_PROP_BRIGHTNESS) << endl;
	cout << "CV_CAP_PROP_CONTRAST: " << m_Camera.get(CV_CAP_PROP_CONTRAST) << endl;
	cout << "CV_CAP_PROP_SATURATION: " << m_Camera.get(CV_CAP_PROP_SATURATION) << endl;
	//cout << "CV_CAP_PROP_HUE: " << m_Camera.get(CV_CAP_PROP_HUE) << endl;	// VIDEOIO ERROR : V4L2: property hue is not supported
	cout << "CV_CAP_PROP_WHITE_BALANCE_BLUE_U: " << m_Camera.get(CV_CAP_PROP_WHITE_BALANCE_BLUE_U) << endl;
	cout << "CV_CAP_PROP_WHITE_BALANCE_RED_V: " << m_Camera.get(CV_CAP_PROP_WHITE_BALANCE_RED_V) << endl;

    //m_Camera.set(CV_CAP_PROP_GAIN, 30.0);				// TODO unsupported with VideoCapture?
	m_Camera.set(CV_CAP_PROP_EXPOSURE, 200.0);			// Default 0.09991
	//m_Camera.set(CV_CAP_PROP_BRIGHTNESS, 0.0);
	m_Camera.set(CV_CAP_PROP_CONTRAST, 0.5);			// Default 0.5
	m_Camera.set(CV_CAP_PROP_SATURATION, 0.5);			// Default 0.5
	//m_Camera.set(CV_CAP_PROP_HUE, 0.0);
	m_Camera.set(CV_CAP_PROP_WHITE_BALANCE_BLUE_U, 1000.0);	// Default 1000
	m_Camera.set(CV_CAP_PROP_WHITE_BALANCE_RED_V, 1000.0);		// Default 1000

	// Warm up camera for 60 frames to stabilize image brightness
	for (int loop = 0; loop < 60 ; loop++)
	{
		m_Camera.grab();
	}
	m_Camera.retrieve(m_image);
	cout << "Camera is opened and warmed up" << endl;
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
#ifdef TEST_GAFFER_TAPE_ALIGNMENT_IMGS
		char fileName[100];
#ifdef BUILD_ON_WINDOWS
		const char* c_testImagePath = "C:/Users/Developer/Documents/TestData/Input/";
		sprintf_s<sizeof(fileName)>(fileName, "%salignment_line%d.jpg", c_testImagePath, loopCounter % testFiles.size() + 1);
#else
		sprintf(fileName, "alignment_line%d.jpg", loopCounter % testFiles.size() + 1);
#endif

#else	// if TEST_GAFFER_TAPE_ALIGNMENT_IMGS

#ifdef BUILD_ON_WINDOWS
		//const char* c_testImagePath = "C:/Users/Developer/Documents/TestData/Input/";
		const char* c_testImagePath = "C:/Users/Developer/Documents/ImagingData/narrow_camera_at_top/";
		string fileName = c_testImagePath + testFiles[loopCounter % testFiles.size()];
#else
		string fileName = testFiles[loopCounter % (int)testFiles.size()];
#endif

#endif	// if TEST_GAFFER_TAPE_ALIGNMENT_IMGS

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
