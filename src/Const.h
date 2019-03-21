/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef CONST_H
#define CONST_H

//#define BUILD_ON_WINDOWS					// Uncomment to build on Windows; comment to build on RasPi
//#define PI_TIMING
#define USE_OFFBOARD_COMMS						// Uncomment if you want to use it
//#define PORTRAIT_IMAGE
//#define CAPTURE_EVERY_NTH_IMAGE
constexpr bool c_bUseLastDiagImage = false;                  //!< If true, read a previously saved image than getting one from camera
constexpr int c_loopCountToSaveDiagImage = 1;
constexpr double c_camera_offset_x0 = 0.0; //8.5;					//!< [inch] Camera offset from center of the robot

//-------------------------------------------------------------------------------------------------------------------------
//#define TEST_GAFFER_TAPE_ALIGNMENT_IMGS		// Use this when testing hatch vision targets made with white gaffer's tape
//#define TEST_FISHEYE_CORRECTION_BY_LUT
//#define TEST_CHECKERBOARD_CALIB

#ifndef TEST_CHECKERBOARD_CALIB
#define DRAW_OPENCV_FIT_LINE
#endif

//#define WRITE_OPENCV_TEXT_ON_IMAGES

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

#ifdef BUILD_ON_WINDOWS
const double PI = 4.0 * atan(1.0);
#else
constexpr double PI = 4.0 * atan(1.0);
#endif

constexpr double c_smallContourPercentOfMax = 0.30;         //!< Reject contours that are less than this % of max countour point count
constexpr double c_arcLenThreshold = 750;				    //!< Reject contours that have an arc length less than this
constexpr double c_areaThreshold = 4500;                    //!< Reject contours that have an controur area less than this
constexpr size_t c_minContourPoints = 100;					//!< Reject contours that have less points than this
constexpr size_t c_maxContourPoints = 2000;                 //!< Reject contours that have more points than this

constexpr bool c_bDrawAllContours = false;                   //!< If true, draw every countour found

constexpr float c_areaThresholdPercent = 0.30f;				//!< Reject rectangles that are less than this % of maximum area found (per image) 
constexpr float c_occludedAspectRatio = 0.43f;			    //!< [ratio] Aspect ratio greater than this might be an occluded retroreflective target
constexpr float c_minLeftAngle = 8.0f;						//!< [degrees] Lower bound for determining left leaning retroreflective target
constexpr float c_maxLeftAngle = 30.0f;						//!< [degrees] Upper bound for determining left leaning retroreflective target
constexpr float c_minRightAngle = 155.0f;					//!< [degrees] Lower bound for determining right leaning retroreflective target
constexpr float c_maxRightAngle = 177.0f;					//!< [degrees] Upper bound for determining right leaning retroreflective target

#ifdef PORTRAIT_IMAGE
constexpr int c_imageHeightPixel = 1280;					//!< [pixel] Height in pixels of image captured by camera
constexpr int c_imageWidthPixel = 960;						//!< [pixel] Height in pixels of image captured by camera
#else
constexpr int c_imageHeightPixel = 960;						//!< [pixel] Height in pixels of image captured by camera
constexpr int c_imageWidthPixel = 1280;						//!< [pixel] Height in pixels of image captured by camera
#endif

static cv::Scalar c_contourColor = { 231, 96, 97 };         //!< [HSV triplet] We will draw a contour of specific color
static cv::Scalar c_centerColor = { 231, 96, 97 };          //!< [HSV triplet] Color for marking center of image and object
static cv::Scalar c_lineColor = { 0, 255, 0 };				//!< [HSV triplet] Color for drawing best fit lines
static cv::Scalar c_biggestContourColor = { 183, 183, 255 };//!< [HSV triplet] Color for drawing biggest contour
static cv::Scalar c_upperCheckerBoard = { 180, 255, 255 };	//!< [HSV triplet] Upper color range for fisheye calibration checker board
static cv::Scalar c_lowerCheckerBoard = { 1, 1, 50 };		//!< [HSV triplet] Lower color range for fisheye calibration checker board

extern bool bImageCaptureTrigger;

extern int loopCounter;
extern std::vector<std::string> testFiles;
extern std::vector<double> testDist;

extern const char* c_testOutputPath;

#endif // CONST_H
