/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
 * 
 * Base class for image processing
*/

#include "ProcessingBase.h"

// 2018 competition cube settings
//Scalar ProcessingBase::m_upper = { 20, 280, 50 };                  
//Scalar ProcessingBase::m_lower = { 40, 255, 255 };

const char* c_testOutputPath = "C:/Users/Developer/Documents/TestData/Output/";

#ifdef BUILD_ON_WINDOWS
double ProcessingBase::m_degreesToRadians = PI / 180.0;
double ProcessingBase::m_radiansTodegrees = 180.0 / PI;
#endif

ProcessingBase::ProcessingBase(const Scalar& upper, const Scalar& lower)
    : m_upper(upper) 
    , m_lower(lower)
{
}

ProcessingBase::~ProcessingBase()
{
}

void ProcessingBase::Prepare(const Mat& image, bool bSkipHSVConvert /* = false */)
{
    if (!bSkipHSVConvert)
    {
	    cvtColor(image, m_imageHSV, COLOR_BGR2HSV);	// Convert BGR to HSV
    }
    
    if (c_bUseLastDiagImage)
	{
        // if (m_calibImageCount == 0)
        // {
        //     char buf[30];
        //     for (int i = 0; i < 11; i++)
        //     {
        //         m_lower = Scalar(i, 50, 30);
        //         //m_upper  = Scalar(16, 255, 255);
                 inRange(m_imageHSV, m_lower, m_upper, m_inrange);
        //         sprintf(buf, "inrange%d.bmp", i);
        //         imwrite(buf, m_inrange);
        //     }
        // }
        static int count = 0;
		char fileName[100];
#ifdef BUILD_ON_WINDOWS
		sprintf_s<sizeof(fileName)>(fileName, "%sinrange%d.jpg", c_testOutputPath, count % 34 + 1);
#else
		sprintf(fileName, "inrange%d.jpg", count % 34 + 1);
#endif
		count++;        
        imwrite(fileName, m_inrange);
    }
    else
    {    
        // Searching for color in the image that has a high of upper scaler and a low of lower scaler. Stores result in inrange
        inRange(m_imageHSV, m_lower, m_upper, m_inrange);	// Identify color per HSV image

        if (m_calibImageCount == 10)
        {
            // For manually calibrating the camera
            imwrite("inrange.jpg", m_inrange);
            imwrite("image.jpg", image);
        }
    }
}

void ProcessingBase::FindContour()
{
    findContours(m_inrange, m_contours, m_hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    // creates a drawing matrix with inrange size and fills it with zeroes
    m_drawing = Mat::zeros(m_inrange.size(), CV_8UC3);

    // Reset the contour values to zero
    m_biggestContour = 0;
    m_biggestContourLocation = 0;
}

void ProcessingBase::FindBiggestContour()
{
	int currentContourSize = 0;

    // Walk through each contour that we found looking for the biggest contour
    for (size_t i = 0; i < m_contours.size(); i++)
    {
        currentContourSize = (int)moments(m_contours[i],true).m00;
        cout << "Contour " << i << " vector size " << m_contours[i].size() << " moment size " << currentContourSize << endl;
        if (m_biggestContour < currentContourSize)
        {
            m_biggestContour = currentContourSize;
            m_biggestContourLocation = (int)i;
        }

        if (c_bDrawAllContours)
        {
            drawContours(m_drawing, m_contours, (int)i , c_contourColor, 2, 8, m_hierarchy, 0); // Line thickness 2, line type 8, offset 0
        }
    }
    if (m_contours.size() > 0)
    {
        // Actually draw the contour for the biggest contour we find 
        drawContours(m_drawing, m_contours, m_biggestContourLocation , c_contourColor, 2, 8, m_hierarchy, 0);
    }
    else
    {
        cout << "Did not find any contour " << endl;
    }

    if (m_calibImageCount == 10 || c_bUseLastDiagImage)
    {
        // For manually calibrating the camera
        static int count = 0;
		char fileName[100];
#ifdef BUILD_ON_WINDOWS
		sprintf_s<sizeof(fileName)>(fileName, "%sdrawing%d.jpg", c_testOutputPath, count % 34 + 1);
#else
		sprintf(fileName, "drawing%d.jpg", count % 34 + 1);
#endif
		count++;        
        imwrite(fileName, m_drawing);
        //imwrite("drawing.bmp", m_drawing);
    }
    m_calibImageCount++;
}

void ProcessingBase::RejectSmallContours()
{
    // Walk through each contour that we found looking for the biggest contour by point count
    size_t maxSize = 0;
    cout << "Finding max contour size out of " << m_contours.size() << " contours"<< endl;
	for (size_t i = 0; i < m_contours.size(); i++)
	{
		if (m_contours[i].size() < c_maxContourPoints)
		{
			maxSize = max(m_contours[i].size(), maxSize);
		}
    }

    vector<vector<Point>> contours;
    size_t threshold = (size_t)(c_smallContourPercentOfMax * maxSize);
    cout << "Max contour size " << maxSize << " threshold " << threshold << endl;
    for (size_t i = 0; i < m_contours.size(); i++)
    {
        if (m_contours[i].size() > threshold)
        {
#ifdef TEST_GAFFER_TAPE_ALIGNMENT_IMGS
			auto arcLen = arcLength(m_contours[i], true);
			auto unorientedArea = contourArea(m_contours[i], false);
			cout << "arcLen " << arcLen << " unorientedArea " << unorientedArea << endl;
			if (arcLen > c_arcLenThreshold && unorientedArea > c_areaThreshold)
#endif
			{
				cout << "Saving contour of size " << m_contours[i].size() << endl;
				contours.push_back(m_contours[i]);
			}
		}
        else
        {
            //cout << "Rejecting contour of size " << m_contours[i].size() << endl;
        }
    }

    m_contours.swap(contours);
}

void ProcessingBase::FindCornerCoordinates()
{
    Mat image(960, 1280, CV_8UC3, Scalar(0));
    vector<Point> vertVectOut;
	RotatedRect calculatedRect;

    cout << "Approximating polygons for " << m_contours.size() << " contours" << endl;
    for (size_t i = 0; i < m_contours.size(); i++)
    {
        approxPolyDP(m_contours[i], vertVectOut, 5.0, true);

		if (vertVectOut.size() >= 4)
		{
			for (int j = 0; j < 4; j++)
			{
				//line(image, vertices[i], vertices[(i+1)%4], Scalar(0, 255, 0));
				//line(image, vertVect[j] + Point2f(200, 0), vertVect[(i+1)%4], Scalar(0, 255, 0));
				line(image, vertVectOut[j], vertVectOut[(j + 1) % 4], Scalar(255, 255, 255));
			}
		}
    }

    m_minRect.resize(m_contours.size());
    float angle;
    Point2f vertices[4];
    for (size_t i = 0; i < m_contours.size(); i++)
    {
        m_minRect[i] = minAreaRect(m_contours[i]);
        if (m_minRect[i].size.width < m_minRect[i].size.height)
        {
            angle = m_minRect[i].angle + 180;
        }
        else
        {
            angle = m_minRect[i].angle + 90; //
        }
        cout << "Angle of minimum area rectangle " << angle << endl;

        m_minRect[i].points(vertices);
		//m_minRect[i].size.area;
		//m_minRect[i].size.height;
		//m_minRect[i].size.width;
		for (int j = 0; j < 4; j++)
		{
			line(image, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));
		}
    }

	static int count = 0;
	char fileName[100];
#ifdef BUILD_ON_WINDOWS
	sprintf_s<sizeof(fileName)>(fileName, "%strapezoid%d.jpg", c_testOutputPath, count % 34 + 1);
#else
	sprintf(fileName, "trapezoid%d.jpg", count % 34 + 1);
#endif
	count++;
	imwrite(fileName, image);
}

void ProcessingBase::FindCenter()
{
    if (m_contours.size() == 0)
    { 
        return;
    }

    //cout << "Biggest contour location: " << m_biggestContourLocation << "\n Size of contour: " << m_contours.size() << endl;
    // Finding the moment of the biggest contour
    Moments m = moments(m_contours[m_biggestContourLocation], true);		// TODO we should save the moments calculated in FindBiggestContour

    // Find cube center coordinate and draw a circle at that point
    m_cube_center_x = m.m10 / m.m00;
    m_cube_center_y = m.m01 / m.m00;
    circle(m_drawing, Point((int)m_cube_center_x, (int)m_cube_center_y), 16, c_centerColor, 2);

    // Find image center coordinate
    m_im_center_x = m_drawing.size().width / 2;
    m_im_center_y = m_drawing.size().height / 2;
    circle(m_drawing, Point((int)m_im_center_x, (int)m_im_center_y), 1, c_centerColor);    // Replacement for drawMarker since drawMarker didn't work.
}

void ProcessingBase::FindVerticalRange()
{
    if (m_contours.size() == 0)
    { 
        return;
    }

    // Find cube height per center coordinate of the cube and contour info
    m_cube_contour_max_y = 10000;   // assign a big value
    m_cube_contour_min_y = 10000;   // assign a big value
	double im_actual_dist;

    for (size_t i = 0; i < m_contours[m_biggestContourLocation].size(); i++)
    {
        im_actual_dist = abs(m_cube_center_x - m_contours[m_biggestContourLocation][i].x);
        if (m_cube_contour_max_y > im_actual_dist)
        {
            if (m_contours[m_biggestContourLocation][i].y > m_cube_center_y) // Find coordinate > object center
            {
                m_cube_contour_max_index = (int)i;
                m_cube_contour_max_y = im_actual_dist;
            }
        }

        if (m_cube_contour_min_y > im_actual_dist)
        {
            if (m_contours[m_biggestContourLocation][i].y <= m_cube_center_y) // Find coordinate <= object center
            {
                m_cube_contour_min_index = (int)i;
                m_cube_contour_min_y = im_actual_dist;
            }
        }
    }
}

void ProcessingBase::CalcCubeHeight()
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

void ProcessingBase::CalcOutputValues()
{
    if (m_contours.size() == 0)
    { 
        return;
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

    int quality = eYellowTrackingObjects; // TODO: Calculate this

    m_OutputValues.SetDistance(m_Forward_Distance_Inch);
    m_OutputValues.SetAngle(m_Horizontal_Angle_Degree);
    m_OutputValues.SetQuality(quality);
}

void ProcessingBase::PrintDebugValues()
{
    cout    << "Counter: " << m_calibImageCount 
        << " Horizontal_Distance_Inch: " << m_Horizontal_Distance_Inch
        << " Vertical_Distance_Inch " << m_Vertical_Distance_Inch
        << " Forward_Distance_Inch: " << m_Forward_Distance_Inch 
        << " Horizontal_Angle " << m_Horizontal_Angle_Degree 
        << endl;
	cout << endl;
}
