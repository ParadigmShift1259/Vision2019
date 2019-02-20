/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef CONST_H
#define CONST_H

#define BUILD_ON_WINDOWS					// Uncomment to build on Windows; comment to build on RasPi
//#define USE_OFFBOARD_COMMS				// Uncomment if you want to use it
//#define TEST_GAFFER_TAPE_ALIGNMENT_IMGS		// Use this when testing hatch vision targets made with white gaffer's tape

#include <math.h>
#include <opencv2/opencv.hpp>
#include <vector>

#ifdef BUILD_ON_WINDOWS
const double PI = 4.0 * atan(1.0);
#else
constexpr double PI = 4.0 * atan(1.0);
#endif

constexpr double c_smallContourPercentOfMax = 0.30;         //!< Reject contours that are less than this % of max countour point count
constexpr double c_arcLenThreshold = 750;				    //!< Reject contours that have an arc length less than this
constexpr double c_areaThreshold = 4500;                    //!< Reject contours that have an controur area less than this
constexpr size_t c_maxContourPoints = 2000;                 //!< Reject contours that have more points than this

constexpr bool c_bUseLastDiagImage = true;                  //!< If true, read a previously saved image than getting one from camera
constexpr bool c_bDrawAllContours = true;                   //!< If true, draw every countour found

constexpr float c_areaThresholdPercent = 0.30f;				//!< Reject rectangles that are less than this % of maximum area found (per image) 
constexpr float c_occludedAspectRatio = 0.43f;			    //!< [ratio] Aspect ratio greater than this might be an occluded retroreflective target
constexpr float c_minLeftAngle = 8.0f;						//!< [degrees] Lower bound for determining left leaning retroreflective target
constexpr float c_maxLeftAngle = 30.0f;						//!< [degrees] Upper bound for determining left leaning retroreflective target
constexpr float c_minRightAngle = 155.0f;					//!< [degrees] Lower bound for determining right leaning retroreflective target
constexpr float c_maxRightAngle = 177.0f;					//!< [degrees] Upper bound for determining right leaning retroreflective target

constexpr double m_DEFAULT_FOV_ROW_NUM = 960.0;			    //!< [pixel] Height in pixels of image captured by camera

static cv::Scalar c_contourColor = { 231, 96, 97 };         //!< [HSV triplet] We will draw a contour of specific color
static cv::Scalar c_centerColor = { 231, 96, 97 };          //!< [HSV triplet] Color for center marking for both image and cube

constexpr int c_loopCountToSaveDiagImage = 0;//10;

extern int loopCounter;
extern std::vector<std::string> testFiles;
extern std::vector<float> testDist;

#endif // CONST_H