/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef CONST_H
#define CONST_H

#include <math.h>
#include <opencv2/opencv.hpp>

constexpr double PI = 4.0 * atan(1.0);

constexpr bool c_bUseLastDiagImage = false;     //!< If true read a previously saved image than getting one from camera

static cv::Scalar c_contourColor = { 231, 96, 97 };        //!< [HSV triplet] We will draw a contour of specific color   
static cv::Scalar c_centerColor = { 231, 96, 97 };         //!< [HSV triplet] Color for center marking for both image and cube                                

#endif // CONST_H