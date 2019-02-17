/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef PROCESSING_BASE_H
#define PROCESSING_BASE_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdint>
#include <string.h>
#include <stdint.h>

#include "Const.h"
#include "OutputValues.h"

using namespace cv;
using namespace std;

class ProcessingBase
{
public:
    ProcessingBase(const Scalar& upper, const Scalar& lower);
    ~ProcessingBase();

	virtual void ProcessImage(const Mat& image) = 0;						//!< Defer to the derived class to process the input image from the camera

    const OutputValues& GetOutputValues() const { return m_OutputValues; }	//!< Return the output values
    const Mat& GetImageHSV() const { return m_imageHSV; }					//!< Return the converted input image in HSV representation
    void SetImageHSV(const Mat& imageHSV) { m_imageHSV = imageHSV; }		//!< Set the converted input image in HSV representation for later retreival

protected:
	void Prepare(const Mat& image, bool bSkipHSVConvert = false);			//!< Convert input image to HSV and perform in-range filtering 
	void FindContour();														//!< Process the in-range image to get object contours (outlines)
	void FindVerticalRange();												//!< Old function for cube; finds the min and max Y coordinates of the biggest contour
	void RejectSmallContours();												//!< Process the contours to reject very small and very large contours by point count
	void FindCornerCoordinates();											//!< Find corner coordinates by finding minimum area rectange and/or approximating a polygon
	void FindCenter();														//!< Find center of biggest contour and center of drawing
	void CalcCubeHeight();													//!< Old function for cube; calculate Cube with/without Fish Eye correction based on vertical range
	void FindBiggestContour();												//!< Process the contours to find the largest contours by moment and draw the contour found TODO split
	void CalcOutputValues();												//!< Calculate the values that will be sent to the robot
	void PrintDebugValues();												//!< Print the output values to the Pi console
	
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

#ifdef BUILD_ON_WINDOWS
	static double m_degreesToRadians;							//!< Angle units converison factor
	static double m_radiansTodegrees;							//!< Angle units converison factor
#else
	static constexpr double m_degreesToRadians = PI / 180.0;	//!< Angle units converison factor
	static constexpr double m_radiansTodegrees = 180.0 / PI;	//!< Angle units converison factor
#endif

    static constexpr bool m_bFishEyeCorrection = false;			//!< Set to true if using a wide angle camera
    
    OutputValues m_OutputValues;                                //!< Values to send to the Robot

    Scalar m_upper;                                             //!< [HSV triplet] Upper color bound
    Scalar m_lower;                                             //!< [HSV triplet]Lower color bound

    double m_standard_height_p = 0.0;							//!< [dimensionless] Scaled height of cube
    double m_pixel_per_in = 0.0;								//!< [pixel/inch] calculated from scaled height
    // TODO not used needed? double m_pixel_per_degree = 0.0;							//!< [pixel/degree] 
    double m_Horizontal_Distance_Pixel = 0.0;					//!< [pixel] Estimated distance from camera to target
    double m_Vertical_Distance_Pixel = 0.0;						//!< [pixel] Estimated distance from floor to target
    double m_Horizontal_Distance_Inch = 0.0;					//!< [inch] Estimated distance from camera to target
	double m_Vertical_Distance_Inch = 0.0;						//!< [inch] Estimated distance from floor to target
    double m_Forward_Distance_Inch = 0.0;						//!< [inch] Estimated distance to command the robot forward
    double m_Total_Distance_Inch = 0.0;							//!< [inch] Estimated total distance as the crow flies from robot to target
    double m_Horizontal_Angle_Degree = 0.0;						//!< [inch] Estimated angle in the plane of the floor from robot to target
    double m_Vertical_Angle_Degree = 0.0;						//!< [inch] Estimated angle in the vertical plane from robot to target

    double m_im_center_x = 0.0;									//!< X coord for center of image (drawing)
    double m_im_center_y = 0.0;									//!< Y coord for center of image (drawing)

    double m_cube_contour_max_y;								//!< Max Y coord for contour TODO change cube
    double m_cube_contour_min_y;								//!< Min Y coord for contour TODO change cube

    double m_cube_center_x = 0.0;								//!< X coord for center of cube TODO change cube
    double m_cube_center_y = 0.0;								//!< Y coord for center of cube TODO change cube
    double m_cube_height = 0.0;    								//!< [units??] Vision target height on image 

	double m_dx = 0.0;											//!< X coord for 3D distance between camera and target
    double m_dy = 0.0;											//!< Y coord for 3D distance between camera and target
    double m_dz = 0.0;											//!< Z coord for 3D distance between camera and target

    // Contour variables to give contour location. Countour is outline points of the object you are trying to identify
	int m_biggestContour = 0;									//!< Size of biggest contour based on moments
	int m_biggestContourLocation = 0;							//!< Index into m_contours of the biggest contor
    int m_cube_contour_max_index = 0;							//!< Index into m_contours of the max Y coord
    int m_cube_contour_min_index = 0;							//!< Index into m_contours of the min Y coord

    vector<Vec4i> m_hierarchy;									//!< Output from OpenCV findCountours (not used; holds info on nested objects)
    vector<vector<Point>> m_contours;							//!< Output from OpenCV findCountours (as applied to in range image)
    vector<RotatedRect> m_minRect;                              //!< minimum area rectangle for contours

    Mat m_inrange;												//!< A black and white image that contains regions for the colors searched for
    Mat m_imageHSV;												//!< Converted input image BGR->HSV
    Mat m_drawing;												//!< An output image to draw contours
};


#endif  // PROCESSING_BASE_H