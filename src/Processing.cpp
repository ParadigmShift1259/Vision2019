/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#include "Processing.h"
#include "CameraWrapper.h"



Processing::Processing()
{
}

Processing::~Processing()
{
}

void Processing::Prepare(const Mat& image)
{
#ifdef GRAB_DIAG_IMAGE
	imwrite("CameraGrab.bmp", image);
#endif

	cvtColor(image, m_imageHSV, COLOR_BGR2HSV);	// Convert BGR to HSV

#ifdef GRAB_DIAG_IMAGE
	imwrite("CamerGrabHsv.bmp", m_imageHSV);
#endif

	// Searching for color in the image that has a high of upper scaler and a low of lower scaler. Stores result in inrange
	inRange(m_imageHSV, m_lower, m_upper, m_inrange);	// Identify color per HSV image
}

void Processing::Contour()
{
    findContours(m_inrange, m_contours, m_hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    // creates a drawing matrix with inrange size and fills it with zeroes
    m_drawing = Mat::zeros(m_inrange.size(), CV_8UC3);

    // Reset the contour values to zero
    m_biggestContour = 0;
    m_biggestContourLocation = 0;
    m_currentContourSize = 0;

    // Walk through each contour that we found looking for the biggest contour
    for (int i = 0; i < m_contours.size(); i++)
    {
        m_currentContourSize = moments(m_contours[i],true).m00;
        if (m_biggestContour < m_currentContourSize)
        {
            m_biggestContour = m_currentContourSize;
            m_biggestContourLocation = i;
        }
    }

    // Actually draw the contour for the biggest contour we find 
    drawContours(m_drawing, m_contours, m_biggestContourLocation , Scalar(124, 252, 0), 2, 8,hierarchy, 0);
}