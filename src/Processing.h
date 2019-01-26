/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#define PI 3.1415926535897932384626433832795

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdint>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <cmath>

#ifndef PROCESSING_H
#define PROCESSING_H

using namespace cv;
using namespace std;

class Processing
{
public:
    Processing();
    ~Processing();

    void Prepare(const Mat& image);
    void FindContour();
    void FindCenter();
    void FindBiggestContour();
    void CalcCubeHeight();
    double CalcOutputValues();
    void PrintDebugValues();

private:
    // Following settings is for camera calibrated value
    static constexpr double m_DEFAULT_FOV_ROW_NUM = 960.0;		    // default = 960
    static constexpr double m_DEFAULT_HEIGHT_PIXEL = 510.0;	        // default = 400
    static constexpr double m_DEFAULT_PIXEL_PER_INCH = 41.5;   	    // default = 42
    static constexpr double m_CAL_DISTANCE_INCH = 12.0;  		    // default = 12
    static constexpr double m_ANGLE_THRESHOLD = 60.0;		        // in degree
    static constexpr double m_FORWARD_DIST_THRESHOLD = 240.0;	    // in inch = 20 feet
    static constexpr double m_CUBE_CONTOUR_THRESHOLD = 100.0; 	    // in pixel
    static Scalar m_upper;                                          // Upper color bound
    static Scalar m_lower;                                          // Lower color bound
    static Scalar m_contourColor;                                   // We will draw a contour of color 124, 252, 0
    static Scalar m_centerColor;                                    // Color for center marking for both image and cube
    static constexpr bool m_bFishEyeCorrection = true;

    double m_standard_height_p = 0.0;
    double m_pixel_per_in = 0.0;
    double m_pixel_per_degree = 0.0;
    double m_Horizontal_Distance_Pixel = 0.0;
    double m_Vertical_Distance_Pixel = 0.0;
    double m_Horizontal_Distance_Inch = 0.0;
    double m_Vertical_Distance_Inch = 0.0;
    double m_Forward_Distance_Inch = 0.0;
    double m_Total_Distance_Inch = 0.0;
    double m_Horizontal_Angle_Degree = 0.0;
    double m_Vertical_Angle_Degree = 0.0;
    double m_im_center_x = 0.0;
    double m_im_center_y = 0.0;
    double m_cube_contour_max_y;
    double m_cube_contour_min_y;
    double m_im_actual_dist;


    double m_cube_center_x = 0.0;
    double m_cube_center_y = 0.0;
    double m_cube_height = 0.0;    	// cube height on image 
	double m_dx = 0.0;
    double m_dy = 0.0;
    double m_dz = 0.0;		// distance in X, Y, Z between camera and cube

    // Contour variables to give contour location. Countour is outline points of the object you are trying to identify
	int m_biggestContour = 0;
	int m_biggestContourLocation = 0;
	int m_currentContourSize = 0;
    int m_cube_contour_max_index = 0;
    int m_cube_contour_min_index = 0;
    int m_calibImageCount = 0;

    vector<Vec4i> m_hierarchy;
    vector<vector<Point> > m_contours;

    Mat m_inrange;
    Mat m_imageHSV;
    Mat m_drawing;
};


#endif  // PROCESSING_H