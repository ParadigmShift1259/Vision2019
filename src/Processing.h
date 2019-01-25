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

#ifndef PROCESSING_H
#define PROCESSING_H

class Processing
{
public:
    Processing();
    ~Processing();

    void Prepare(const Mat& image);
    void Contour();

private:

    // Following settings is for camera calibrated value
    static const double m_DEFAULT_FOV_ROW_NUM = 960.0;		// default = 960
    static const double m_DEFAULT_HEIGHT_PIXEL = 510.0;	    // default = 400
    static const double m_DEFAULT_PIXEL_PER_INCH = 41.5;   	// default = 42
    static const double m_CAL_DISTANCE_INCH = 12.0;  		// default = 12
    static const double m_ANGLE_THRESHOLD = 60.0;		    // in degree
    static const double m_FORWARD_DIST_THRESHOLD = 240.0;	// in inch = 20 feet
    static const double m_CUBE_CONTOUR_THRESHOLD = 100.0; 	// in pixel
    static const Scalar m_upper = Scalar(20, 280, 50);      // Upper color bound
    static const Scalar m_lower = Scalar(40, 255, 255);     // Lower color bound
    static const Scalar m_contourColor = Scalar(124, 252, 0)// We will draw a contour of color 124, 252, 0

    double m_standard_height_p = 0.0;
    double m_pixel_per_in = 0.0;
    double m_pixel_per_degree = 0.0;
    double m_Horizontal_Distance_Pixel = 0.0;
    double m_Vertical_Distance_Pixel = 0.0;
    double m_Horizontal_Distance_Inch = 0.0;
    double m_Vertical_Distance_Inch = 0.0;
    double m_Forward_Distance_Inch = 0.0;


    double m_cube_center_x = 0;
    double m_cube_center_y = 0;
    double m_cube_height = 0;    	// cube height on image 
	double m_dx, m_dy, m_dz;		// distance in X, Y, Z between camera and cube

    // Contour variables to give contour location. Countour is outline points of the object you are trying to identify
	int m_biggestContour = 0;
	int m_biggestContourLocation = 0;
	int m_currentContourSize = 0;

    vector<Vec4i> m_hierarchy;
    vector<vector<Point> > m_contours;

    Mat m_inrange;
    Mat m_imageHSV;
    Mat m_drawing;
};


#endif  // PROCESSING_H