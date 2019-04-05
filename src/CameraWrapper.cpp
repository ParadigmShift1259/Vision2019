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
#ifdef TEST_CHECKERBOARD_CALIB
const char* c_testImagePath = "C:/Users/Developer/Documents/TestData/FishEyeCorrected/";
#else
#ifdef PORTRAIT_IMAGE
const char* c_testImagePath = "C:/Users/Developer/Documents/ImagingData/fisheye_camera/Portrait/";
#else
//const char* c_testImagePath = "C:/Users/Developer/Documents/ImagingData/fisheye_camera/";
const char* c_testImagePath = "C:/Users/Developer/Documents/ImagingData/cameraOnRobot/selected/";
//const char* c_testImagePath = "C:/Users/Developer/Documents/ImagingData/cameraOnRobot/";
//const char* c_testImagePath = "C:/Users/Developer/Documents/TestData/OutputTestMatrixMar9/";
//const char* c_testImagePath = "C:/Projects/";
//const char* c_testImagePath = "C:/Users/Developer/Documents/TestData/StLouisData/2019_StLouis_Test/images_30deg/";
//const char* c_testImagePath = "C:/Users/212036134/Documents/Personal/FIRST Robotics/ImagingData/FromCafeteriaMar2/";
#endif	// PORTRAIT_IMAGE
#endif	// TEST_CHECKERBOARD_CALIB
#endif	// TEST_FILES_WIDE

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
#ifdef TEST_CHECKERBOARD_CALIB
	  "cam6.jpg"
#else

#ifdef PORTRAIT_IMAGE
	  "im_12in_p_wlen.jpg"
	, "im_18in_p_wlen.jpg"
	, "im_30in_p_wlen.jpg"
	, "im_48in_p_wlen.jpg"
	, "im_72in_p_wlen.jpg"
	, "im_96in_p_wlen.jpg"
#else
	//  "im_12in_h_wlen_1.jpg"
	//, "im_18in_h_wlen_1.jpg"
	//, "im_18in_h_wlen_1.jpg"
	//, "im_18in_p_wlen.jpg"
	//, "im_30in_h_wlen_1.jpg"
	//, "im_30in_h_wlen_1.jpg"
	//, "im_30in_p_wlen.jpg"
	//, "im_48in_h_wlen_1.jpg"
	//, "im_72in_h_wlen_1.jpg"
	//, "im_96in_h_wlen_1.jpg"

	//, "im_36in_45deg_h_wlen.jpg"
	//, "im_36in_m45deg_h_wlen.jpg"
	//, "im_60in_45deg_h_wlen.jpg"
	//, "im_60in_m45deg_h_wlen.jpg"

	// "image3ft0.jpg"
	//,"image3ft20.jpg"
	//,"image1ft20.jpg"

	//"image50.jpg"
	//,"image125.jpg"
	//,"image200.jpg"

	//  "image1ft_0deg.jpg"
	//, "image2ft_0deg.jpg"
	//, "image3ft_0deg.jpg"
	//, "image4ft_0deg.jpg"
	//, "image5ft_0deg.jpg"
	//, "image6ft_0deg.jpg"
	//, "image6ft_10degLf.jpg"
	//, "image6ft_10degRt.jpg"
	//, "image6ft_20degLf.jpg"
	//, "image6ft_20degRt.jpg"

	//"image675.jpg"

	//  "image0.jpg"
	//  "image1.jpg"
	//, "image2.jpg"
	//, "image3.jpg"
	//, "imagecargo2ft.jpg"
	//, "imagecargo3ft.jpg"
	//, "imagecargo4ft.jpg"
	//, "imagecargo0ft.jpg"
	//, "imagecargo1ft.jpg"

	  "im_394_1p5ft_contactUpsideDown.jpg"
	, "im_394_1p5ft_contact.jpg"
	//, "im_405_1p5ft_contact.jpg"
	//, "im_301_4ft.jpg"
	//, "im_367_2ft.jpg"
	//, "im_298_error_handling.jpg"
	//, "im_296.jpg"
	//, "im_837_w_hatch.jpg"
	//, "im_840_w_hatch.jpg"
	//, "im_426_5ft.jpg"
	//, "im_429_5ft.jpg"
	//, "im_450_5ft.jpg"
	//, "im_455_5ft_angled.jpg"
	//, "im_479_5ft.jpg"
	//, "im_495_4ft.jpg"
	//, "im_508_3ft.jpg"
	//, "im_512_2ft.jpg"
	//, "im_520_1p5ft_contact.jpg"
	//, "im_527_1p5ft_contact.jpg"
	//, "im_577_5ft_2line_start.jpg"
	//, "im_586_5ft_2line_start_tilted.jpg"
	//, "im_603_5ft_2line_start_titled.jpg"
	//, "im_633_4ft_2line_start_titled.jpg"
	//, "im_347_3ft.jpg"
	//, "im_658_3ft_2line_start_titled.jpg"
	//, "im_667_2ft_2line_start_titled.jpg"
	//, "im_674_1ft_2line_start_titled.jpg.jpg"

#endif	// PORTRAIT_IMAGE
#endif	// TEST_CHECKERBOARD_CALIB
#endif	// TEST_FILES_WIDE

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

std::vector<double> testDist	// Keep in sync with testFile vector
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
	, 18.0
	, 24.0
	, 30.0
	, 36.0
	, 36.0
	, 48.0
	, 60.0
	, 60.0
	, 72.0
	, 96.0
	//, 40.0
	//, 60.0
	//, 40.0
	//, 40.0
	//, 60.0
	//, 60.0
#endif

#ifdef TEST_FILES_WIDE
#ifdef TEST_CHECKERBOARD_CALIB
	  12.0	//?
#else
#ifdef PORTRAIT_IMAGE
	// Portrait
	//  12.0
	//, 18.0
	//, 30.0
	//, 48.0
	//, 72.0
	//, 96.0

	// Horizontal (landscape)
#else
	// Horizontal (landscape)
	//  12.0
	//, 18.0
	//, 30.0
	//, 48.0
	//, 72.0
	//, 96.0

	//, 36.0	//, "im_36in_45deg_h_wlen.jpg"
	//, 36.0	//, "im_36in_m45deg_h_wlen.jpg"
	//, 60.0	//, "im_60in_45deg_h_wlen.jpg"
	//, 60.0	//, "im_60in_m45deg_h_wlen.jpg"

	// 36
	//,36

	//  12
	//, 72
	//, 24

	  12
	, 24
	, 36
	, 48
	, 60
	, 72
	, 72
	, 72
	, 72
	, 72

	//  18.0	//image0.jpg
	//, 18.0	//image1.jpg
	//, 18.0	//image2.jpg
	//, 18.0	//image3.jpg
	//, 24.0  //"imagecargo2ft.jpg"
	//, 36.0  //"imagecargo3ft.jpg"
	//, 48.0  //"imagecargo4ft.jpg"
	//, 0.0  //"imagecargo0ft.jpg"
	//, 12.0  //"imagecargo1ft.jpg"

	//  18.0	//im_394_1p5ft_contact.jpg
	//, 18.0	//im_405_1p5ft_contact.jpg
	//, 48.0	//im_301_4ft.jpg
	//, 36.0	//im_347_3ft.jpg
	//, 24.0	//im_367_2ft.jpg
	//, 48.0	//im_298_error_handling.jpg
	//, 48.0	//im_296.jpg
	//, 60.0	//im_837_w_hatch.jpg
	//, 60.0	//im_840_w_hatch.jpg
	//, 60.0	//im_426_5ft.jpg
	//, 60.0	//im_429_5ft.jpg
	//, 60.0	//im_450_5ft.jpg
	//, 60.0	//im_455_5ft_angled.jpg
	//, 60.0	//im_479_5ft.jpg
	//, 48.0	//im_495_4ft.jpg
	//, 36.0	//im_508_3ft.jpg
	//, 24.0	//im_512_2ft.jpg
	//, 18.0	//im_520_1p5ft_contact.jpg
	//, 18.0	//im_527_1p5ft_contact.jpg
	//, 60.0	//im_577_5ft_2line_start.jpg
	//, 60.0	//im_586_5ft_2line_start_tilted.jpg
	//, 60.0	//im_603_5ft_2line_start_titled.jpg
	//, 48.0	//im_633_4ft_2line_start_titled.jpg
	//, 36.0	//im_658_3ft_2line_start_titled.jpg
	//, 24.0	//im_667_2ft_2line_start_titled.jpg
	//, 12.0	//im_674_1ft_2line_start_titled.jpg.jpg
#endif

	//, 18.0	//, "testFishEye.jpg"
	//, 30.0	//, "test_horiz30gaff.jpg"
	//, 45.0	//, "test_horiz45gaff.jpg"
	//, 18.0	//, "test_vertical0gaff.jpg"
	//, 45.0	//, "test_vertical45gaff.jpg"
#endif
#endif

#ifdef TEST_FILES_LINE
	  1.0
	, 2.0
	, 3.0
	, 4.0
	, 5.0
	, 6.0
	, 7.0
	, 8.0
	, 9.0
	, 10.0
	, 11.0
	, 12.0
	, 13.0
	, 14.0
	, 15.0
	, 16.0
	, 17.0
	, 18.0
	, 19.0
	, 20.0
	, 21.0
	, 22.0
	, 23.0
	, 24.0
	, 25.0
	, 26.0
	, 27.0
	, 28.0
	, 29.0
	, 30.0
	, 31.0
	, 32.0
	, 33.0
	, 34.0
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
	//m_Camera.set(CV_CAP_PROP_EXPOSURE, 0.0001);			// Default 0.09991
	//cout << "CV_CAP_PROP_EXPOSURE after set to 0.0001: " << m_Camera.get(CV_CAP_PROP_EXPOSURE) << endl;
	//m_Camera.set(CV_CAP_PROP_BRIGHTNESS, 0.0);
	//m_Camera.set(CV_CAP_PROP_CONTRAST, 0.5);			// Default 0.5
	//m_Camera.set(CV_CAP_PROP_SATURATION, 0.5);			// Default 0.5
	//m_Camera.set(CV_CAP_PROP_HUE, 0.0);
	//m_Camera.set(CV_CAP_PROP_WHITE_BALANCE_BLUE_U, 1000.0);	// Default 1000
	//m_Camera.set(CV_CAP_PROP_WHITE_BALANCE_RED_V, 1000.0);		// Default 1000

	if (m_Camera.get(CV_CAP_PROP_FRAME_WIDTH) != c_imageWidthPixel)
	{
		cout << "Setting camera resolution width to " << c_imageWidthPixel  << " was " << m_Camera.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
		m_Camera.set(CV_CAP_PROP_FRAME_WIDTH, c_imageWidthPixel);
	}
	if (m_Camera.get(CV_CAP_PROP_FRAME_HEIGHT) != c_imageHeightPixel)
	{
		cout << "Setting camera resolution height to " << c_imageHeightPixel << " was " << m_Camera.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
		m_Camera.set(CV_CAP_PROP_FRAME_HEIGHT, c_imageHeightPixel);
	}	
	
	// Warm up camera for 60 frames to stabilize image brightness
	cout << "Warming up camera" << endl;
	int warmUpLoops = 60;
#ifdef BUILD_ON_WINDOWS
	warmUpLoops = 1;
#endif
	Mat image;
	for (int loop = 0; loop < warmUpLoops; loop++)
	{
		m_Camera.grab();
	}
	//int grabCount = 0;
	//while (!m_Camera.grab())
	//{
	//	grabCount++;
	//}
	//cout << "Camera warmup grab count " << grabCount << endl;
	m_Camera.retrieve(image);
	if (image.rows > 0 && image.cols > 0)
	{
		cvtColor(image, m_imageHSV, COLOR_BGR2HSV);	// Convert BGR to HSV
	}
	cout << "Camera is opened and warmed up" << endl;
}

CameraWrapper::~CameraWrapper()
{
    m_Camera.release();
}

void CameraWrapper::AcquireImage()
{
	ScopedTimer timer("CameraWrapper::AcquireImage ");
	Mat image;

	if (c_bUseLastDiagImage)
	{
		// Reread the previously saved image to consistently process the same image
#ifdef BUILD_ON_WINDOWS
		string fileName = c_testImagePath + testFiles[loopCounter % testFiles.size()];
#else
		string fileName = testFiles[loopCounter % (int)testFiles.size()];
#endif

		{
			ScopedTimer timer("Reading image from file ");
			image = imread(fileName, CV_LOAD_IMAGE_COLOR);
		}

		if (image.empty())
		{
			cout << "Cannot find the specified file name: " << fileName << endl;
		}
	}
	else
	{
		int grabCount = 0;
		while (!m_Camera.grab())
		{
			grabCount++;
		}
		//cout << "Camera grab count " << grabCount << endl;

		if (!m_Camera.retrieve(image))
		{
			cout << "Cannot retrieve the image from the camera" << endl;
		}
	}

	if (!image.empty())
	{
		if (image.cols != c_imageWidthPixel || image.rows != c_imageHeightPixel)
		{
			cout << "Image from the camera is wrong resolution cols " << image.cols << " rows " << image.rows << endl;
		}

#ifdef CORRECT_UPSIDE_DOWN_IMAGE
		{
			ScopedTimer timer("Rotating image ");
			rotate(image, image, ROTATE_180);	// Camera moved to center of robot, temporarily upside down
		}
#endif

		if (c_bUseLastDiagImage || bImageCaptureTrigger)
		{
			char fileName[255];
#ifdef BUILD_ON_WINDOWS
			int ndx = loopCounter % testFiles.size();
			sprintf_s<sizeof(fileName)>(fileName, "%s%dimage_%s", c_testOutputPath, ndx + 1, testFiles[ndx].c_str());
#else
			if (c_bUseLastDiagImage)
			{
				sprintf(fileName, "image%d.jpg", loopCounter % testFiles.size() + 1);
			}
			else
			{
				sprintf(fileName, "image%d.jpg", loopCounter);
			}
#endif
			//cout << "Capturing image " << fileName << endl;
			//imwrite(fileName, image);
			SaveFileInBackground(m_imageWriteTask, fileName, image);
		}

		{
			ScopedTimer timer("Convert image color scheme ");
			cvtColor(image, m_imageHSV, COLOR_BGR2HSV);	// Convert BGR to HSV
		}
	}
}

template <class Task>
void CameraWrapper::SaveFileInBackground(Task& writeTask, const std::string& fileName, const Mat& matrix)
{
	if (writeTask.valid())
	{
		ScopedTimer timer("Done waiting for previous write task to complete, elapsed ");
		writeTask.wait();
	}

	//thread::id this_id = this_thread::get_id();
	//cout << "Thread " << this_id << endl;

	// Do not pass the arguments as references; we need copies, otherwise both the foregroud and background threads will access the objects
	writeTask = async(launch::async, [fileName, matrix]()
	{
		//thread::id this_id = this_thread::get_id();
		//cout << "Thread " << this_id << endl;
		ScopedTimer timer("Write task complete, elapsed ");
		imwrite(fileName, matrix);
	});
}
