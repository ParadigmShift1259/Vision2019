/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

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

const double PI = 4.0 * atan(1.0);
//const double PI = acos(-1.0);

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
    static constexpr double m_DEFAULT_FOV_ROW_NUM = 960.0;		//!< [pixel] Height in pixels of image captured by camera
    static constexpr double m_DEFAULT_HEIGHT_PIXEL = 510.0;	    //!< [pixel] Height in pixels of a cube placed 12 inches from the camer
    static constexpr double m_CAL_DISTANCE_INCH = 12.0;  		//!< [inch] Calibration distance from camera to cube
	static constexpr double m_MEAS_CUBE_HEIGHT = 12.3;  		//!< [inch] Height of cube in inches; used a tape measure in the real world
	static constexpr double m_DEFAULT_PIXEL_PER_INCH = m_DEFAULT_HEIGHT_PIXEL / m_MEAS_CUBE_HEIGHT;	// [pxiel/inch] ~41.463
	// Output value bounds
	static constexpr double m_ANGLE_THRESHOLD = 60.0;		    //!< [degrees] If we calculate an output angle more than this, do not send it
    static constexpr double m_FORWARD_DIST_THRESHOLD = 240.0;	//!< [inch] 20 feet; if we calculate an output distance more than this, do not send it
	static constexpr double m_CUBE_CONTOUR_THRESHOLD = 100.0; 	//!< [pixel] If the contour is smaller than this, do not process it

	static constexpr double m_degreesToRadians = PI / 180.0;	//!< Angle units converison factor
	static constexpr double m_radiansTodegrees = 180.0 / PI;	//!< Angle units converison factor

    static Scalar m_upper;                                      //!< [HSV triplet] Upper color bound
    static Scalar m_lower;                                      //!< [HSV triplet]Lower color bound
    static Scalar m_contourColor;                               //!< [HSV? triplet] We will draw a contour of color 124, 252, 0
    static Scalar m_centerColor;                                //!< [HSV triplet?] Color for center marking for both image and cube
    static constexpr bool m_bFishEyeCorrection = true;			//!< Set to true if using a wide angle camera

    double m_standard_height_p = 0.0;							//!< [dimensionless] Scaled height of cube
    double m_pixel_per_in = 0.0;								//!< [pixel/inch] calculated from scaled height
    double m_pixel_per_degree = 0.0;							//!< 
    double m_Horizontal_Distance_Pixel = 0.0;					//!< 
    double m_Vertical_Distance_Pixel = 0.0;						//!< 
    double m_Horizontal_Distance_Inch = 0.0;					//!< 
    double m_Vertical_Distance_Inch = 0.0;						//!< 
    double m_Forward_Distance_Inch = 0.0;						//!< 
    double m_Total_Distance_Inch = 0.0;							//!< 
    double m_Horizontal_Angle_Degree = 0.0;						//!< 
    double m_Vertical_Angle_Degree = 0.0;						//!< 
    double m_im_center_x = 0.0;									//!< 
    double m_im_center_y = 0.0;									//!< 
    double m_cube_contour_max_y;								//!< 
    double m_cube_contour_min_y;								//!< 
    double m_im_actual_dist;									//!< 


    double m_cube_center_x = 0.0;								//!< 
    double m_cube_center_y = 0.0;								//!< 
    double m_cube_height = 0.0;    								//!< cube height on image 
	double m_dx = 0.0;											//!< 
    double m_dy = 0.0;											//!< 
    double m_dz = 0.0;											//!< distance in X, Y, Z between camera and cube

    // Contour variables to give contour location. Countour is outline points of the object you are trying to identify
	int m_biggestContour = 0;									//!< 
	int m_biggestContourLocation = 0;							//!< 
	int m_currentContourSize = 0;								//!< 
    int m_cube_contour_max_index = 0;							//!< 
    int m_cube_contour_min_index = 0;							//!< 
    int m_calibImageCount = 0;									//!< 

    vector<Vec4i> m_hierarchy;									//!< 
    vector<vector<Point> > m_contours;							//!< 

    Mat m_inrange;												//!< 
    Mat m_imageHSV;												//!< 
    Mat m_drawing;												//!< 
};


#endif  // PROCESSING_H