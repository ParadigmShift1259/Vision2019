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
	cout << "Loop,"
		<< "Dist,"
		<< "Area,"
		<< "ShortSide,"
		<< "LongSide,"
		<< "AspectRatio,"
		<< "Angle"
		<< endl;
}

ProcessingBase::~ProcessingBase()
{
}

void ProcessingBase::Prepare(const Mat& image, bool bSkipHSVConvert /* = false */)
{
    if (!bSkipHSVConvert && image.rows > 0 && image.cols > 0)
    {
	    cvtColor(image, m_imageHSV, COLOR_BGR2HSV);	// Convert BGR to HSV
    }
    
    if (c_bUseLastDiagImage)
	{
        // if (loopCounter == 0)
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
		if (!bSkipHSVConvert)
		{
			char fileName[255];
#ifdef BUILD_ON_WINDOWS
		int ndx = loopCounter % testFiles.size();
		sprintf_s<sizeof(fileName)>(fileName, "%s%dinrange_%s", c_testOutputPath, ndx + 1, testFiles[ndx].c_str());
#else
		sprintf(fileName, "inrange%d.jpg", loopCounter % testFiles.size() + 1);
#endif
			imwrite(fileName, m_inrange);
			imwrite("image.jpg", image);
		}
    }
    else
    {    
        // Searching for color in the image that has a high of upper scaler and a low of lower scaler. Stores result in inrange
        inRange(m_imageHSV, m_lower, m_upper, m_inrange);	// Identify color per HSV image

        if (loopCounter == c_loopCountToSaveDiagImage)
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
        //cout << "Contour " << i << " vector size " << m_contours[i].size() << " moment size " << currentContourSize << endl;
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

    if (loopCounter == c_loopCountToSaveDiagImage || c_bUseLastDiagImage)
    {
        // For manually calibrating the camera
		char fileName[255];
#ifdef BUILD_ON_WINDOWS
		int ndx = loopCounter % testFiles.size();
		sprintf_s<sizeof(fileName)>(fileName, "%s%ddrawing_%s", c_testOutputPath, ndx + 1, testFiles[ndx].c_str());
#else
		sprintf(fileName, "drawing%d.jpg", loopCounter % testFiles.size() + 1);
#endif
        imwrite(fileName, m_drawing);
        //imwrite("drawing.bmp", m_drawing);
    }
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
			//cout << "arcLen " << arcLen << " unorientedArea " << unorientedArea << endl;
			if (arcLen > c_arcLenThreshold && unorientedArea > c_areaThreshold)
#endif
			{
				cout << "Saving contour of size " << m_contours[i].size() << endl;
				contours.push_back(m_contours[i]);
			}
		}
  //      else
  //      {
  //          cout << "Rejecting contour of size " << m_contours[i].size() << endl;
  //      }
    }

    m_contours.swap(contours);
}

void ProcessingBase::FindCornerCoordinates()
{
	if (m_contours.size() == 0)
	{
		return;
	}

	Mat image(m_inrange.rows, m_inrange.cols, CV_8UC3, Scalar(0));
    //vector<Point> vertVectOut;

  //  cout << "Approximating polygons for " << m_contours.size() << " contours" << endl;
  //  for (size_t i = 0; i < m_contours.size(); i++)
  //  {
  //      approxPolyDP(m_contours[i], vertVectOut, 5.0, true);

		//if (vertVectOut.size() >= 4)
		//{
		//	for (int j = 0; j < 4; j++)
		//	{
		//		//line(image, vertices[i], vertices[(i+1)%4], Scalar(0, 255, 0));
		//		//line(image, vertVect[j] + Point2f(200, 0), vertVect[(i+1)%4], Scalar(0, 255, 0));
		//		line(image, vertVectOut[j], vertVectOut[(j + 1) % 4], Scalar(255, 255, 255));
		//	}
		//}
  //  }

	//cout << "Finding minimum area rotated rectangles for " << m_contours.size() << " contours" << endl;
	m_minRect.resize(m_contours.size());
	vector<float> angles(m_contours.size());

	enum ESide
	{
		  eUnknown
		, eLeft
		, eRight
	};
	vector<string> sideStr =
	{
		  "Unknown"
		, "Left"
		, "Right"
	};
	vector<ESide> side(m_contours.size());

	const vector<Scalar> colors =
	{
		  {   0,   0, 255 }
		, {   0, 255,   0 }
		, { 255,   0,   0 }
		, { 255, 255,   0 }
		, { 255,   0, 255 }
		, {   0, 255, 255 }
		, { 128, 128, 128 }
		, { 128,   0,   0 }
		, {   0, 128,   0 }
		, {   0,   0, 128 }
	};

	float maxArea = FLT_MIN;
	for (size_t i = 0; i < m_contours.size(); i++)
	{
		m_minRect[i] = minAreaRect(m_contours[i]);
		maxArea = max(maxArea, m_minRect[i].size.area());

		if (m_minRect[i].size.width < m_minRect[i].size.height)
		{
			angles[i] = m_minRect[i].angle + 180;
		}
		else
		{
			angles[i] = m_minRect[i].angle + 90;
		}
		
		// Is this a left or right leaning retroreflective target 
		if (angles[i] > c_minLeftAngle && angles[i] < c_maxLeftAngle)
		{
			side[i] = eLeft;
		}
		else if (angles[i] > c_minRightAngle && angles[i] < c_maxRightAngle)
		{
			side[i] = eRight;
		}
		else
		{
			side[i] = eUnknown;
		}
		cout << "Minimum area " << m_minRect[i].size.area()
			<< " width " << m_minRect[i].size.width
			<< " height " << m_minRect[i].size.height
			<< " angle " << angles[i]
			<< " side " << sideStr[side[i]]
			<< " aspect ratio 1 " << m_minRect[i].size.width / m_minRect[i].size.height
			<< " aspect ratio 2 " << m_minRect[i].size.height / m_minRect[i].size.width
			<< endl;
	}

	Point2f vertices[4];
	char text[255];
	const float areaThreshold = c_areaThresholdPercent * maxArea;
	for (size_t i = 0; i < m_contours.size(); i++)
	{
		if (m_minRect[i].size.area() < areaThreshold || side[i] == eUnknown)
		{
			if (side[i] == eUnknown)
			{
				cout << "Rejecting rectangle of unknown angle " << angles[i] << endl;
			}
			else
			{
				cout << "Rejecting rectangle with small area " << m_minRect[i].size.area() << endl;
			}
			continue;
		}

		float longSide = max(m_minRect[i].size.width, m_minRect[i].size.height);
		float shortSide = min(m_minRect[i].size.width, m_minRect[i].size.height);
		float aspectRatio = shortSide / longSide;
		//if (aspectRatio > c_occludedAspectRatio)
		//{
		//	cout << " aspect ratio greater than 0.4 may be occluded by hatch " << aspectRatio << endl;
		//}

		auto color = colors[i % colors.size()];
        m_minRect[i].points(vertices);
		for (int j = 0; j < 4; j++)
		{
			line(image, vertices[j], vertices[(j + 1) % 4], color, 2, 8);
		}

		//void putText(InputOutputArray img, const String& text, Point org, int fontFace, double fontScale, Scalar color, int thickness = 1, int lineType = LINE_8, bool bottomLeftOrigin = false)
		const double fontScale = 0.37;
		const int textThickness = 1;
		float offset = side[i] == eRight ? 50.f : 0.0f;
		float y = i * 10.0f;
		Point2f areaLoc(vertices[0].x - offset, y + 10.0f);
		Point2f whLoc(vertices[0].x - offset, y + 30.0f);
		Point2f angleLoc(vertices[0].x - offset, y + 50.0f);
		Point2f sideLoc(vertices[0].x - offset, y + 70.0f);
#ifdef BUILD_ON_WINDOWS
		sprintf_s<sizeof(text)>(text, "Area: %.2f", m_minRect[i].size.area());
		cv::putText(image, text, areaLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
		sprintf_s<sizeof(text)>(text, "width: %.2f height: %.2f", m_minRect[i].size.width, m_minRect[i].size.height);
		cv::putText(image, text, whLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
		sprintf_s<sizeof(text)>(text, "Angle %.2f", angles[i]);
		cv::putText(image, text, angleLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
		sprintf_s<sizeof(text)>(text, "%s", sideStr[side[i]].c_str());
		cv::putText(image, text, sideLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
		if (aspectRatio > c_occludedAspectRatio)
		{
			Point2f loc(vertices[0].x - offset, y + 90.0f);
			sprintf_s<sizeof(text)>(text, "Occluded - Aspect ratio %.2f", aspectRatio);
			cv::putText(image, text, loc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
		}
#else
		sprintf(text, "Area: %.2f", m_minRect[i].size.area());
		cv::putText(image, text, vertices[0] - offset, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
		sprintf_s<sizeof(text)>(text, "width: %.2f height: %.2f", m_minRect[i].size.width, m_minRect[i].size.height);
		cv::putText(image, text, whLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
		sprintf(text, "Angle %.2f", angle);
		cv::putText(image, text, vertices[0] - offset, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
		sprintf(text, "%s", sideStr[side[i]].c_str());
		cv::putText(image, text, vertices[0] - offset, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
		if (aspectRatio > c_occludedAspectRatio)
		{
			Point2f loc(vertices[0].x - offset, y + 90.0f);
			sprintf(text, "Occluded - Aspect ratio %.2f", aspectRatio);
			cv::putText(image, text, loc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
		}
#endif

		cout << loopCounter << ","
			<< testDist[loopCounter % testFiles.size()] << ","
			<< m_minRect[i].size.area() << ","
			<< shortSide << ","
			<< longSide << ","
			<< aspectRatio << ","
			<< angles[i]
			<< endl;
	}

	char fileName[255];
#ifdef BUILD_ON_WINDOWS
	int ndx = loopCounter % testFiles.size();
	sprintf_s<sizeof(fileName)>(fileName, "%s%d_trapezoid_%s", c_testOutputPath, ndx + 1, testFiles[ndx].c_str());
#else
	sprintf(fileName, "trapezoid%d.jpg", loopCounter % testFiles.size() + 1);
#endif
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
	int ndx = loopCounter % testFiles.size();
#ifdef BUILD_ON_WINDOWS
	cout << testFiles[ndx] << endl;
#else
	cout << testFiles[ndx] << endl;
#endif

	cout    << "Counter: " << loopCounter
			<< " Horizontal_Distance_Inch: " << m_Horizontal_Distance_Inch
			<< " Vertical_Distance_Inch " << m_Vertical_Distance_Inch
			<< " Forward_Distance_Inch: " << m_Forward_Distance_Inch 
			<< " Horizontal_Angle " << m_Horizontal_Angle_Degree 
			<< endl;

	cout << endl;
}
