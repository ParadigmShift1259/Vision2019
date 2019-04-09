/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef CONST_H
#define CONST_H

//#define BUILD_ON_WINDOWS					// Uncomment to build on Windows; comment to build on RasPi
#define USE_OFFBOARD_COMMS						// Uncomment if you want to use it

//#define CAPTURE_EVERY_NTH_IMAGE
#ifdef CAPTURE_EVERY_NTH_IMAGE
constexpr int c_SaveEveryNthImage = 1;//25;
#endif

constexpr int c_warmUpLoops = 60;

//#define PORTRAIT_IMAGE
//#define CORRECT_UPSIDE_DOWN_IMAGE
//#define CALC_LEFT_RIGHT_TARGETS				// Uncomment to calculate the distance to each retro target individually
constexpr double c_camera_offset_x0 = 0.0;					//!< [inch] Camera offset from center of the robot
//constexpr double c_camera_offset_x0 = 8.5;					//!< [inch] Camera offset from center of the robot

// Output value bounds
constexpr double c_maxAngleChangePerLoop = 40.0;			//!< [degrees] If the difference from the last known good output angle is more than this, do not send it
constexpr double c_maxAngle = 60.0;							//!< [degrees] If we calculate an output angle more than this, do not send it
constexpr double c_maxActualDist = 10.0 * 12.0;				//!< [inch] 8 feet; if we calculate an output distance more than this, do not send it

//#define TRACK_TO_LAST_COORD
constexpr double c_trackToLastCoordDist = 30.0;	//!< [inch] Distance under which we track to the last object center instead of image center

//-------------------------------------------------------------------------------------------------------------------------
//#define TEST_CHECKERBOARD_CALIB

#ifndef TEST_CHECKERBOARD_CALIB
#define DRAW_OPENCV_FIT_LINE
#endif

/// If true, read a previously saved image than getting one from camera
#ifdef BUILD_ON_WINDOWS
constexpr bool c_bUseLastDiagImage = true;
constexpr bool c_bDrawAllContours = true;                   //!< If true, draw every countour found
#define WRITE_OPENCV_TEXT_ON_IMAGES
#else
constexpr bool c_bUseLastDiagImage = false;
constexpr bool c_bDrawAllContours = false;                   //!< If true, draw every countour found
//constexpr bool c_bDrawAllContours = true;                   //!< If true, draw every countour found
#endif

// Define these one at a time
//#define TEST_FILES_LINE					// Uncomment to test image files of the alignment (gaffer's tape) lines
//#define TEST_FILES_ROCKET					// Uncomment to test image files of the rocket
//#define TEST_FILES_CARGO					// Uncomment to test image files of the cargo ship
//#define TEST_FILES_NARROW					// Uncomment to test image files of the cargo ship with the narrow field of view camera
#define TEST_FILES_WIDE					// Uncomment to test image files of the cargo ship with the wide field of view (fish eye) camera

#include <math.h>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <vector>
#include <map>
#include <chrono>

#define USE_SCOPED_TIMER
class ScopedTimer
{
public:
	ScopedTimer(const char* logText) : m_logText(logText)
	{
#ifdef USE_SCOPED_TIMER
		m_start = std::chrono::system_clock::now();
#endif
	}

	~ScopedTimer()
	{
#ifdef USE_SCOPED_TIMER
		auto now = std::chrono::system_clock::now();
		std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start);
		if (elapsed.count() > 0)
		{
			//std::cout << std::endl;
			std::cout << m_logText << elapsed.count() << " milliseconds" << std::endl;
		}
#endif
	}

private:
	std::chrono::system_clock::time_point m_start;
	const char* m_logText;
};

#ifdef BUILD_ON_WINDOWS
const double PI = 4.0 * atan(1.0);
#else
constexpr double PI = 4.0 * atan(1.0);
#endif

constexpr double c_smallContourPercentOfMax = 0.30;         //!< Reject contours that are less than this % of max countour point count
constexpr size_t c_minContourPoints = 60;					//!< Reject contours that have less points than this
constexpr size_t c_maxContourPoints = 700;					//!< Reject contours that have more points than this

constexpr float c_occludedAspectRatio = 0.43f;			    //!< [ratio] Aspect ratio greater than this might be an occluded retroreflective target

constexpr int c_upperCornerIgnore = 300;					//!< [pixel] Number of pixels to ignore in the upper left and right
constexpr int c_lowerImageIgnore = 200;						//!< [pixel] Number of pixels to ignore at the bottom of the image

#ifdef PORTRAIT_IMAGE
constexpr int c_imageHeightPixel = 1280;					//!< [pixel] Height in pixels of image captured by camera
constexpr int c_imageWidthPixel = 960;						//!< [pixel] Height in pixels of image captured by camera
#else
constexpr int c_imageHeightPixel = 960;						//!< [pixel] Height in pixels of image captured by camera
constexpr int c_imageWidthPixel = 1280;						//!< [pixel] Height in pixels of image captured by camera
#endif
constexpr int c_im_center_x = c_imageWidthPixel / 2;		//!< X coord for center of image (drawing)
constexpr int c_im_center_y = c_imageHeightPixel / 2;		//!< Y coord for center of image (drawing)

static cv::Scalar c_contourColor = { 231, 96, 97 };         //!< [HSV triplet] We will draw a contour of specific color
static cv::Scalar c_fishEyeContourColor = { 30, 130, 250 };	//!< [HSV triplet] Color for drawing uncorrected contours
static cv::Scalar c_biggestContourColor = { 183, 183, 255 };//!< [HSV triplet] Color for drawing biggest contour
static cv::Scalar c_centerColor = { 231, 96, 97 };          //!< [HSV triplet] Color for marking center of image and object
static cv::Scalar c_lineColor = { 0, 255, 0 };				//!< [HSV triplet] Color for drawing best fit lines
static cv::Scalar c_upperCheckerBoard = { 180, 255, 255 };	//!< [HSV triplet] Upper color range for fisheye calibration checker board
static cv::Scalar c_lowerCheckerBoard = { 1, 1, 50 };		//!< [HSV triplet] Lower color range for fisheye calibration checker board
static cv::Scalar c_black = { 0, 0, 0 };					//!< [HSV triplet] Black
//static cv::Scalar c_outOfRange = { 40, 153, 0 };

extern bool bImageCaptureTrigger;

extern int loopCounter;
extern std::vector<std::string> testFiles;
extern std::vector<double> testDist;

extern const char* c_testOutputPath;

#endif // CONST_H
