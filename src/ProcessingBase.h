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

enum ESide
{
	  eUnknownSide
	, eLeft
	, eRight
};

struct RectDescr
{
	size_t m_originalContourIndex;
	RotatedRect m_minRect;
	float m_slope;
	float m_angle;
	ESide m_side;
	float m_yIntercept;
};

struct LineDescr
{
	float m_slope;
	Point m_point1;
	Point m_point2;
	float m_yIntercept;
};

class ProcessingBase
{
public:
    ProcessingBase(const Scalar& upper, const Scalar& lower);
    ~ProcessingBase();

	virtual void ProcessImage(const Mat& image) = 0;						//!< Defer to the derived class to process the input image from the camera

    const OutputValues& GetOutputValues() const { return m_OutputValues; }	//!< Return the output values
	virtual const OutputValues& GetLeftTargetOutputValues() const { return m_OutputValues; }	//!< The derived class should override
	virtual const OutputValues& GetRightTargetOutputValues() const { return m_OutputValues; }	//!< The derived class should override

protected:
	void Prepare(const Mat& image);											//!< Perform in-range filtering 
	void FindContours();													//!< Process the in-range image to get object contours (outlines)
	void FitLinesToContours();												//!< Best fit of a straight line through all contours
	LineDescr FitLineToContour(const vector<Point>& contour);				//!< Fit line to single contour and return slope
	void ApproximatePolygons();												//!< Fit polygons to all contours
	void FindVerticalRange();												//!< Finds the min and max Y coordinates of the biggest contour
	void RejectSmallContours();												//!< Process the contours to reject very small and very large contours by point count
	void FishEyeCorrectContours();											//!< Iterate through all contours and correct each point
	void FishEyeCorrectContour(size_t index);								//!< Iterate through all points in specified contour and correct each point
	void FishEyeCorrectPoint(int xIn, int yIn, int& xOut, int& yOut);		//!< Correct the distortion from the wide angle (fisheye) camera
	void FishEyeCorrectPoint(double xIn, double yIn, double& xOut, double& yOut);//!< Correct the distortion from the wide angle (fisheye) camera
	void FindCornerCoordinates();											//!< Find corner coordinates by finding minimum area rectange and/or approximating a polygon
	void FindCenter();														//!< Find center of biggest contour and center of drawing
	void CalcObjectHeight();												//!< Calculate object height with/without Fish Eye correction based on vertical range
	void FindBiggestContour();												//!< Process the contours to find the largest contours by moment and draw the contour found TODO split
	void CalcOutputValues(const char* objType);								//!< Calculate the values that will be sent to the robot
	EQuality CalcOutputValues(const char* objType, double objHeight, double objCenterX, double objCenterY, double& actualDistInch, double& horzAngleDegree);
	void PrintDebugValues(const char* objType, double horzDistInch, double vertDistInch, double objHeight, double actualDistInch, double horzAngleDegree);		//!< Print the output values to the Pi console
	void CalibrateHSV(const Mat& imageHsv, int hue1, int hue2, int span, int satValueIncrement);	//!< Loop through hue, saturation, and values to find the best upper and lower bound
	virtual const char* GetTargetName() = 0;								//!< Derived class name to get unique filenames

	// Following settings is for camera calibrated value
    //static constexpr double m_calibTargetSizePixel = 104.0;					//!< [pixel] Height in pixels of a target placed m_calibCameraDistInch from the camera
	static constexpr double m_calibTargetSizePixel = 93.0;					//!< [pixel] Height in pixels of a target placed m_calibCameraDistInch from the camera
	static constexpr double m_calibCameraDistInch = 28.0;  					//!< [inch] Calibration distance from camera to object
	static constexpr double m_measuredObjectHeight = 6.0;  					//!< [inch] Height of object in inches; used a tape measure in the real world
	static constexpr double m_defaultPixelPerInch = m_calibTargetSizePixel / m_measuredObjectHeight;	// [pixel/inch] ~25
	static constexpr double m_cameraToFrontOfRobotDistInch = 18.0;  		//!< [inch] 
	// Output value bounds
	static constexpr double m_maxAngle = 60.0;								//!< [degrees] If we calculate an output angle more than this, do not send it
    static constexpr double m_maxActualDist = 10.0 * 12.0;					//!< [inch] 10 feet; if we calculate an output distance more than this, do not send it
	static constexpr const double m_k = -0.46;     							//!< Constant value for fisheye lens used by Raspberry pi (determined empirically)

#ifdef BUILD_ON_WINDOWS
	static double m_degreesToRadians;							//!< Angle units converison factor
	static double m_radiansToDegrees;							//!< Angle units converison factor
#else
	static constexpr double m_degreesToRadians = PI / 180.0;	//!< Angle units converison factor
	static constexpr double m_radiansToDegrees = 180.0 / PI;	//!< Angle units converison factor
#endif

    static constexpr bool m_bFishEyeCorrection = true;			//!< Set to true if using a wide angle camera
    
    OutputValues m_OutputValues;                                //!< Values to send to the Robot

    Scalar m_upper;                                             //!< [HSV triplet] Upper color bound
    Scalar m_lower;                                             //!< [HSV triplet]Lower color bound

    double m_Horizontal_Angle_Degree = 0.0;						//!< [inch] Estimated angle in the plane of the floor from robot to target
	double m_Actual_Distance_Inch = 0.0;						//!< [inch] Estimated total distance as the crow flies from robot to target with compensation for offset camera

    double m_im_center_x = 0.0;									//!< X coord for center of image (drawing)
    double m_im_center_y = 0.0;									//!< Y coord for center of image (drawing)
	double m_R;													//!< Radius to center of image for fisheye correction
	double m_borderCorr;										//!< Scaling factor per border for fisheye correction

    double m_object_contour_max_y;								//!< Max Y coord for contour of object
    double m_object_contour_min_y;								//!< Min Y coord for contour of object

    double m_object_center_x = 0.0;								//!< X coord for center of object
    double m_object_center_y = 0.0;								//!< Y coord for center of object 
    double m_object_height = 0.0;    							//!< [units??] Vision target height on image 

    // Contour variables to give contour location. Countour is outline points of the object you are trying to identify
	int m_biggestContour = 0;									//!< Size of biggest contour based on moments
	int m_biggestContourLocation = 0;							//!< Index into m_contours of the biggest contor
    int m_object_contour_max_index = 0;							//!< Index into m_contours of the max Y coord
    int m_object_contour_min_index = 0;							//!< Index into m_contours of the min Y coord

    vector<Vec4i> m_hierarchy;									//!< Output from OpenCV findCountours (not used; holds info on nested objects)
    vector<vector<Point>> m_contours;							//!< Output from OpenCV findCountours (as applied to in range image)
	vector<RectDescr> m_rectDescr;								//!< Minimum area rectangle for contours, line slope and y intercept
	Vec4f m_lineOutput;											//!< Collection of lines fit to all contours
	vector<pair<Point, Point>> m_linePoints;					//!< End points of lines fit to all contours
	RectDescr m_leftTarget;
	RectDescr m_rightTarget;

    Mat m_inrange;												//!< A black and white image that contains regions for the colors searched for
    Mat m_drawing;												//!< An output image to draw contours

	uint16_t* m_pXfisheyeData;//[c_imageWidthPixel * c_imageHeightPixel];
	uint16_t* m_pYfisheyeData;//[c_imageWidthPixel * c_imageHeightPixel];
};


#endif  // PROCESSING_BASE_H