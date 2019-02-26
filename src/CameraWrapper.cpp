/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#include "CameraWrapper.h"
#include "Const.h"

#ifdef TEST_FILES_LINE
const char* c_testImagePath = "C:/Users/Developer/Documents/TestData/Input/";
#endif
#ifdef TEST_FILES_ROCKET
const char* c_testImagePath = "C:/Users/Developer/Documents/ImagingData/";
#endif
#ifdef TEST_FILES_CARGO
const char* c_testImagePath = "C:/Users/Developer/Documents/ImagingData/";
#endif
#ifdef TEST_FILES_NARROW
const char* c_testImagePath = "C:/Users/Developer/Documents/ImagingData/narrow_camera_at_top/";
#endif
#ifdef TEST_FILES_WIDE
#ifdef TEST_FISHEYE_CORRECTION_BY_LUT
const char* c_testImagePath = "C:/Users/Developer/Documents/TestData/FishEyeCorrected/";
#else
const char* c_testImagePath = "C:/Users/Developer/Documents/ImagingData/fisheye_camera/";
#endif
#endif

std::vector<std::string> testFiles
{
#ifdef TEST_FILES_ROCKET
	  "RocketPanelStraightDark16in.jpg"
	, "RocketPanelStraightDark24in.jpg"
	, "RocketPanelStraightDark36in.jpg"
	, "RocketPanelStraightDark48in.jpg"
	, "RocketPanelStraightDark60in.jpg"
	, "RocketPanelStraightDark72in.jpg"
	, "RocketPanelStraightDark96in.jpg"
	, "RocketStraightDark96in.jpg"
	, "RocketPanelStraightDark12in.jpg"
	, "RocketPanelStraightDark24inLeftOnly.jpg"
	, "RocketPanelStraightDark24inRightOnly.jpg"
#endif

#ifdef TEST_FILES_CARGO
	, "CargoSideStraightDark36in.jpg"
	, "CargoSideStraightDark60in.jpg"
	, "CargoSideStraightDark72in.jpg"
	, "CargoSideStraightPanelDark36in.jpg"
	, "CargoStraightDark19in.jpg"
	, "CargoStraightDark24in.jpg"
	, "CargoStraightDark48in.jpg"
	, "CargoStraightDark72in.jpg"
	, "CargoStraightDark90in.jpg"
#endif

#ifdef TEST_FILES_NARROW
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
#endif

#ifdef TEST_FILES_WIDE
#ifdef TEST_FISHEYE_CORRECTION_BY_LUT
	  "cam6.jpg"
#else
	  "im_96in_p_wlen.jpg"
	, "im_12in_h_wlen_1.jpg"
	, "im_12in_p_wlen.jpg"
	, "im_18in_h_wlen_1.jpg"
	, "im_18in_p_wlen.jpg"
	, "im_30in_h_wlen_1.jpg"
	, "im_30in_p_wlen.jpg"
	, "im_36in_45deg_h_wlen.jpg"
	, "im_36in_m45deg_h_wlen.jpg"
	, "im_48in_h_wlen_1.jpg"
	, "im_48in_p_wlen.jpg"
	, "im_60in_45deg_h_wlen.jpg"
	, "im_60in_m45deg_h_wlen.jpg"
	, "im_72in_h_wlen_1.jpg"
	, "im_72in_p_wlen.jpg"
	, "im_96in_h_wlen_1.jpg"

	, "testFishEye.jpg"
	, "test_horiz30gaff.jpg"
	, "test_horiz45gaff.jpg"
	, "test_vertical0gaff.jpg"
	, "test_vertical45gaff.jpg"
#endif
#endif

#ifdef TEST_FILES_LINE
	  "alignment_line1.jpg"
	, "alignment_line2.jpg"
	, "alignment_line3.jpg"
	, "alignment_line4.jpg"
	, "alignment_line5.jpg"
	, "alignment_line6.jpg"
	, "alignment_line7.jpg"
	, "alignment_line8.jpg"
	, "alignment_line9.jpg"
	, "alignment_line10.jpg"
	, "alignment_line11.jpg"
	, "alignment_line12.jpg"
	, "alignment_line13.jpg"
	, "alignment_line14.jpg"
	, "alignment_line15.jpg"
	, "alignment_line16.jpg"
	, "alignment_line17.jpg"
	, "alignment_line18.jpg"
	, "alignment_line19.jpg"
	, "alignment_line20.jpg"
	, "alignment_line21.jpg"
	, "alignment_line22.jpg"
	, "alignment_line23.jpg"
	, "alignment_line24.jpg"
	, "alignment_line25.jpg"
	, "alignment_line26.jpg"
	, "alignment_line27.jpg"
	, "alignment_line28.jpg"
	, "alignment_line29.jpg"
	, "alignment_line30.jpg"
	, "alignment_line31.jpg"
	, "alignment_line32.jpg"
	, "alignment_line33.jpg"
	, "alignment_line34.jpg"
#endif
};

std::vector<float> testDist	// Keep in sync with testFile vector
{
#ifdef TEST_FILES_ROCKET
	  16.0
	, 24.0
	, 36.0
	, 48.0
	, 60.0
	, 72.0
	, 96.0
	, 96.0
	, 12.0
	, 24.0
	, 24.0
#endif

#ifdef TEST_FILES_CARGO
	  36.0
	, 60.0
	, 72.0
	, 36.0
	, 19.0
	, 24.0
	, 48.0
	, 72.0
	, 90.0
#endif

#ifdef TEST_FILES_NARROW
	  12.0
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
#endif

#ifdef TEST_FILES_WIDE
#ifdef TEST_FISHEYE_CORRECTION_BY_LUT
	  12	//?
#else
	  96
	, 12
	, 12
	, 18
	, 18
	, 30
	, 30
	, 36
	, 36
	, 48
	, 48
	, 60
	, 60
	, 72
	, 72
	, 96
	// These are not right, probably angles
	, 18	//? maybe
	, 30
	, 45
	, 18	// 0
	, 45
#endif
#endif

#ifdef TEST_FILES_LINE
	  1
	, 2
	, 3
	, 4
	, 5
	, 6
	, 7
	, 8
	, 9
	, 10
	, 11
	, 12
	, 13
	, 14
	, 15
	, 16
	, 17
	, 18
	, 19
	, 20
	, 21
	, 22
	, 23
	, 24
	, 25
	, 26
	, 27
	, 28
	, 29
	, 30
	, 31
	, 32
	, 33
	, 34
#endif
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
#ifdef BUILD_ON_WINDOWS
		string fileName = c_testImagePath + testFiles[loopCounter % testFiles.size()];
#else
		string fileName = testFiles[loopCounter % (int)testFiles.size()];
#endif

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
