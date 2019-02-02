/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#include "ProcessingCargo.h"

// 2018 competition cube settings
//Scalar ProcessingCargo::m_upper = { 20, 280, 50 };                  
//Scalar ProcessingCargo::m_lower = { 40, 255, 255 };

// Fluorescent lighting - working but not perfect
Scalar ProcessingCargo::m_lower = { 10, 50, 30 };
Scalar ProcessingCargo::m_upper  = { 16, 255, 255 }; 

Scalar ProcessingCargo::m_contourColor = { 231, 96, 97 };           
Scalar ProcessingCargo::m_centerColor = { 231, 96, 97 }; 

ProcessingCargo::ProcessingCargo()
{
}

ProcessingCargo::~ProcessingCargo()
{
}

void ProcessingCargo::Prepare(const Mat& image)
{
#ifdef GRAB_DIAG_IMAGE
	imwrite("CameraGrab.bmp", image);
#endif

	cvtColor(image, m_imageHSV, COLOR_BGR2HSV);	// Convert BGR to HSV

#ifdef GRAB_DIAG_IMAGE
	imwrite("CamerGrabHsv.bmp", m_imageHSV);
#endif

    if (c_bUseLastDiagImage)
	{
        if (m_calibImageCount == 0)
        {
            char buf[30];
            for (int i = 0; i < 11; i++)
            {
                m_lower = Scalar(i, 50, 30);
                //m_upper  = Scalar(16, 255, 255);
                inRange(m_imageHSV, m_lower, m_upper, m_inrange);
                sprintf(buf, "inrange%d.bmp", i);
                imwrite(buf, m_inrange);
            }
        }
    }
    else
    {    
        // Searching for color in the image that has a high of upper scaler and a low of lower scaler. Stores result in inrange
        inRange(m_imageHSV, m_lower, m_upper, m_inrange);	// Identify color per HSV image

        if (m_calibImageCount == 10)
        {
            // For manually calibrating the camera
            imwrite("inrange.bmp", m_inrange);
            imwrite("image.bmp", image);
        }
    }
}

void ProcessingCargo::FindContour()
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
    if (m_contours.size() > 0)
    {
        // Actually draw the contour for the biggest contour we find 
        drawContours(m_drawing, m_contours, m_biggestContourLocation , m_contourColor, 2, 8, m_hierarchy, 0);
    }
    else
    {
        cout << "Did not find any contour " << endl;
    }

    if (m_calibImageCount == 10)
    {
        // For manually calibrating the camera
        imwrite("drawing.bmp", m_drawing);
    }
    m_calibImageCount++;
}

void ProcessingCargo::FindCenter()
{
    if (m_contours.size() == 0)
    { 
        return;
    }

    //cout << "Biggest contour location: " << m_biggestContourLocation << "\n Size of contour: " << m_contours.size() << endl;
    // Finding the moment of the biggest contour
    Moments m = moments(m_contours[m_biggestContourLocation], true);
    cout << "Past establishing moment" << endl;

    // Find cube center coordinate and draw a circle at that point
    m_cube_center_x = m.m10 / m.m00;
    m_cube_center_y = m.m01 / m.m00;
    cout << "Cube center moments established" << endl;
    circle(m_drawing, Point(m_cube_center_x, m_cube_center_y), 16, m_centerColor, 2);
    cout << "Center of the cube found" << endl;

    // Find image center coordinate
    m_im_center_x = m_drawing.size().width / 2;
    m_im_center_y = m_drawing.size().height / 2;
    cout << "Center coordinates of image" << endl;
    circle(m_drawing, Point(m_im_center_x, m_im_center_y), 1, m_centerColor);    // Replacement for drawMarker since drawMarker didn't work.
    cout << "Circle for the center of image canvas" << endl;
}

void ProcessingCargo::FindBiggestContour()
{
    if (m_contours.size() == 0)
    { 
        return;
    }

    // Find cube height per center coordinate of the cube and contour info
    m_cube_contour_max_y = 10000;   // assign a big value
    m_cube_contour_min_y = 10000;   // assign a big value

    for (int i = 0; i < m_contours[m_biggestContourLocation].size(); i++)
    {
        m_im_actual_dist = abs(m_cube_center_x - m_contours[m_biggestContourLocation][i].x);
        if (m_cube_contour_max_y > m_im_actual_dist)
        {
            if (m_contours[m_biggestContourLocation][i].y > m_cube_center_y) // Find coordinate > object center
            {
                m_cube_contour_max_index = i;
                m_cube_contour_max_y = m_im_actual_dist;
            }
        }

        if (m_cube_contour_min_y > m_im_actual_dist)
        {
            if (m_contours[m_biggestContourLocation][i].y <= m_cube_center_y) // Find coordinate <= object center
            {
                m_cube_contour_min_index = i;
                m_cube_contour_min_y = m_im_actual_dist;
            }
        }
    }
}

void ProcessingCargo::CalcCubeHeight()
{
    if (m_contours.size() == 0)
    { 
        return;
    }
    
    if (m_bFishEyeCorrection) 
    {
        // Apply distortion correction for fisheye camera to three sets of coordinates only to speed up calculation
        double x_original[3];
        double y_original[3];
        double x_corrected[3];
        double y_corrected[3];
        double xt[3];
        double yt[3];
        double ut[3];
        double vt[3];
        double r;
        double R;
        double theta;
        double s;
        double s2;
        double border_corr;
        const double k = -0.46;     		// a constant value for fisheye lens used by Raspberry pi
        
        x_original[0] = m_cube_center_x;
        x_original[1] = m_cube_center_x;
        x_original[2] = m_cube_center_x;
    
        y_original[0] = m_contours[m_biggestContourLocation][m_cube_contour_min_index].y;
        y_original[1] = m_cube_center_y;
        y_original[2] = m_contours[m_biggestContourLocation][m_cube_contour_max_index].y;

        R = sqrt(m_im_center_x * m_im_center_x + m_im_center_y * m_im_center_y);
        border_corr = 1/(1 + k*pow( min(m_im_center_x, m_im_center_y)/R, 2.0) ); // Scaling factor per border

        for (int i = 0; i < 3; i++)
        {
            xt[i] = x_original[i] - m_im_center_x;
            yt[i] = y_original[i] - m_im_center_y; 
    
            r = sqrt(xt[i]*xt[i] + yt[i]*yt[i]);	// Find radius

            theta = atan(yt[i]/xt[i])*180/PI; 	// Find theta for the angle

            if ((yt[i] > 0) && (xt[i] >= 0))
            {
                theta = theta;
            }
            else if ((yt[i] < 0) && (xt[i] >= 0))
            {
                theta = 360 + theta;
            }
            else if ((yt[i] >= 0) && (xt[i] <= 0))
            {
                theta = 180 + theta;
            }
            else if ((yt[i] < 0) && (xt[i] <= 0))
            {
                theta = 180 + theta;
            }

            r = r / R;				// Normalize the polar coordinate r
    
            s = r * (1 + k * r);				// Apply r-based transform with k

            s2 = s * R;				// Un-normalize s

            s2 = s2 * border_corr;			// Scale radius 

            ut[i] = s2*cos(theta*PI/180);		// Converted back to cartesian coordinates
            vt[i] = s2*sin(theta*PI/180);

            x_corrected[i] = ut[i] + m_im_center_x;	// Add image center back 
            y_corrected[i] = vt[i] + m_im_center_y;
        }

        m_cube_height = abs(y_corrected[2] -  y_corrected[0]);
        m_cube_center_x = x_corrected[1];
        m_cube_center_y = y_corrected[1];
    }
    else   
    {
        // Return the cube height with Fish Eye correction
        m_cube_height = 	abs(m_contours[m_biggestContourLocation][m_cube_contour_max_index].y -  
                            m_contours[m_biggestContourLocation][m_cube_contour_min_index].y);
    }
}

double ProcessingCargo::CalcOutputValues()
{
    if (m_contours.size() == 0)
    { 
        return 0.0;
    }
    
    m_standard_height_p = m_DEFAULT_HEIGHT_PIXEL / (m_DEFAULT_FOV_ROW_NUM / m_drawing.size().height);
    m_pixel_per_in = m_DEFAULT_PIXEL_PER_INCH/(m_DEFAULT_FOV_ROW_NUM / m_drawing.size().height);

    m_Total_Distance_Inch = ((m_standard_height_p/m_cube_height) * m_CAL_DISTANCE_INCH);
    m_Horizontal_Distance_Pixel = m_cube_center_x - m_im_center_x;
    m_Vertical_Distance_Pixel = m_im_center_y - m_cube_center_y;	// Switch due to (0,0) at the top left to convert to bottom left
    m_Horizontal_Angle_Degree = atan(m_Horizontal_Distance_Pixel/(m_pixel_per_in*m_CAL_DISTANCE_INCH))*180/PI;
    m_Vertical_Angle_Degree = atan(m_Vertical_Distance_Pixel/(m_pixel_per_in*m_CAL_DISTANCE_INCH))*180/PI;

    // Output values given to Network Table
    m_Horizontal_Distance_Inch = m_Total_Distance_Inch * sin(m_Horizontal_Angle_Degree*PI/180);
    m_Vertical_Distance_Inch = m_Total_Distance_Inch * sin(m_Vertical_Angle_Degree*PI/180);
    m_Forward_Distance_Inch = m_Total_Distance_Inch*cos(m_Vertical_Angle_Degree*PI/180)*cos(m_Horizontal_Angle_Degree*PI/180);

    // Add constraints for cube distance and angle
    if ((abs(m_Horizontal_Angle_Degree ) > m_ANGLE_THRESHOLD ) ||
        (abs(m_Vertical_Angle_Degree) > m_ANGLE_THRESHOLD ) ||
        (m_Forward_Distance_Inch < 0) || (m_Forward_Distance_Inch > m_FORWARD_DIST_THRESHOLD ) ||
        (m_contours[m_biggestContourLocation].size() < m_CUBE_CONTOUR_THRESHOLD ))
    {
        m_Horizontal_Distance_Inch = 0;
        m_Vertical_Distance_Inch=0;
        m_Forward_Distance_Inch =0;
    }

    return m_Horizontal_Angle_Degree;
}

void ProcessingCargo::PrintDebugValues()
{
    cout    << "Counter: " << m_calibImageCount 
        << " Horizontal_Distance_Inch: " << m_Horizontal_Distance_Inch
        << " Vertical_Distance_Inch " << m_Vertical_Distance_Inch
        << " Forward_Distance_Inch: " << m_Forward_Distance_Inch 
        << " Horizontal_Angle " << m_Horizontal_Angle_Degree 
        << endl;
}
