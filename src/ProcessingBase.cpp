/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
 * 
 * Base class for image processing
*/

#include "ProcessingBase.h"

#include <time.h>	// For clock_gettime()

// 2018 competition cube settings
//Scalar ProcessingBase::m_upper = { 20, 280, 50 };                  
//Scalar ProcessingBase::m_lower = { 40, 255, 255 };

const char* c_testOutputPath = "C:/Users/Developer/Documents/TestData/Output/";

#ifdef BUILD_ON_WINDOWS
double ProcessingBase::m_degreesToRadians = PI / 180.0;
double ProcessingBase::m_radiansToDegrees = 180.0 / PI;
#endif

ProcessingBase::ProcessingBase(const Scalar& upper, const Scalar& lower)
	: m_upper(upper)
	, m_lower(lower)
{
#ifdef TEST_CHECKERBOARD_CALIB
	m_upper = c_upperCheckerBoard;
	m_lower = c_lowerCheckerBoard;
#endif

	cout << "Loop,"
		<< "Dist,"
		<< "Area,"
		<< "ShortSide,"
		<< "LongSide,"
		<< "AspectRatio,"
		<< "Angle"
		<< endl;

#ifdef BUILD_ON_WINDOWS
#ifdef TEST_FISHEYE_CORRECTION_BY_LUT
	string fileName = "C:/Users/Developer/Documents/TestData/FishEyeCorrected/x_index_960x1280_uint16.img";
#else
	string fileName = "C:/Users/Developer/Documents/TestData/FishEyeCorrected/c_index_960x1280_uint16.img";
#endif
#else
#ifdef TEST_FISHEYE_CORRECTION_BY_LUT
	string fileName = "x_index_960x1280_uint16.img";
#else
	string fileName = "c_index_960x1280_uint16.img";
#endif
#endif
	m_pXfisheyeData = new uint16_t[c_imageWidthPixel * c_imageHeightPixel];
	m_pYfisheyeData = new uint16_t[c_imageWidthPixel * c_imageHeightPixel];
	memset(m_pXfisheyeData, 0, sizeof(uint16_t) * c_imageWidthPixel * c_imageHeightPixel);
	memset(m_pYfisheyeData, 0, sizeof(uint16_t) * c_imageWidthPixel * c_imageHeightPixel);
	ifstream in(fileName, ios::in || ios::binary);
	while (!in.eof())
	{
		in.read((char *)m_pXfisheyeData, sizeof(uint16_t) * c_imageWidthPixel * c_imageHeightPixel);
	}
	in.close();

#ifdef BUILD_ON_WINDOWS
#ifdef TEST_FISHEYE_CORRECTION_BY_LUT
	fileName = "C:/Users/Developer/Documents/TestData/FishEyeCorrected/y_index_960x1280_uint16.img";
#else
	fileName = "C:/Users/Developer/Documents/TestData/FishEyeCorrected/r_index_960x1280_uint16.img";
#endif
#else
#ifdef TEST_FISHEYE_CORRECTION_BY_LUT
	fileName = "y_index_960x1280_uint16.img";
#else
	fileName = "r_index_960x1280_uint16.img";
#endif
#endif
	in.open(fileName, ios::in || ios::binary);
	while (!in.eof())
	{
		in.read((char *)m_pYfisheyeData, sizeof(uint16_t) * c_imageWidthPixel * c_imageHeightPixel);
	}
	in.close();
}

ProcessingBase::~ProcessingBase()
{
	if (m_pXfisheyeData)
		delete [] m_pXfisheyeData;
	if (m_pYfisheyeData)
		delete [] m_pYfisheyeData;
}

void ProcessingBase::Prepare(const Mat& image, bool bSkipHSVConvert /* = false */)
{
	if (!bSkipHSVConvert && image.rows > 0 && image.cols > 0)
    {
	    cvtColor(image, m_imageHSV, COLOR_BGR2HSV);	// Convert BGR to HSV
		imwrite("OriginalImage.jpg", image);
    }
    
    if (c_bUseLastDiagImage)
	{
#ifdef TEST_FISHEYE_CORRECTION_BY_LUT
		Mat inrangeTemp;
		cv::inRange(m_imageHSV, m_lower, m_upper, inrangeTemp);	// Identify color per HSV image

		m_inrange = inrangeTemp.clone();	// Get the output size right
		imwrite("inrangeTemp.jpg", inrangeTemp);

		for (int row = 0; row < c_imageHeightPixel; row++)
		{
			for (int col = 0; col < c_imageWidthPixel; col++)
			{
				int ndx = (int)((c_imageWidthPixel * row) + col);		// [y][x] if 2D array
				uint16_t c = 0;
				uint16_t r = 0;
				if (ndx < c_imageWidthPixel * c_imageHeightPixel)
				{
					c = m_pXfisheyeData[ndx];
					r = m_pYfisheyeData[ndx];
				}

				if (r != 0)
				{
					r--;
				}

				if (c != 0)
				{
					c--;
				}

				uchar color = inrangeTemp.at<uchar>(Point(c, r));
				m_inrange.at<uchar>(Point(col, row)) = color;
			}
		}
		imwrite("imageInRange.jpg", inrangeTemp);
		imwrite("imageCorrected.jpg", m_inrange);
#else
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
#endif

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

	// Find image center coordinate
	m_im_center_x = m_drawing.size().width / 2;
	m_im_center_y = m_drawing.size().height / 2;
	m_R = sqrt(m_im_center_x * m_im_center_x + m_im_center_y * m_im_center_y);	// For fisheye correction
	m_borderCorr = 1 / (1 + m_k * pow(min(m_im_center_x, m_im_center_y) / m_R, 2.0)); // Scaling factor per border

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

#ifndef BUILD_ON_WINDOWS
		long int start_time;
		long int time_difference;
		struct timespec gettime_now;
	
		clock_gettime(CLOCK_REALTIME, &gettime_now);
		start_time = gettime_now.tv_nsec;		//Get nS value
	
		int count = 0;
		vector<vector<Point>> corrected(m_contours.size());
		for (size_t i = 0; i < m_contours.size(); i++)
		{
			auto& vIn = m_contours[i];
			auto& vOut = corrected[i];
			vOut.resize(vIn.size());
			for (size_t j = 0; j < vIn.size(); j++)
			{
				double xCorrected;
				double yCorrected;
				count++;
				FishEyeCorrect(vIn[j].x, vIn[j].y, xCorrected, yCorrected);
				vOut[j].x = xCorrected;
				vOut[j].y = yCorrected;
			}
		}
	
		clock_gettime(CLOCK_REALTIME, &gettime_now);
		time_difference = gettime_now.tv_nsec - start_time;
		if (time_difference < 0)
			time_difference += 1000000000;				//(Rolls over every 1 second)
		cout << "Fisheye correction of " << count << " points took " << time_difference << " nanoseconds " << time_difference / count << " nanosec/pt" << endl;
	#endif
}

void ProcessingBase::FishEyeCorrectContours()
{
	for (size_t i = 0; i < m_contours.size(); i++)
	{
		for (size_t j = 0; j < m_contours[i].size(); j++)
		{
			double uncorrectedX = m_contours[i][j].x;
			double uncorrectedY = m_contours[i][j].y;
			double correctedX;
			double correctedY;

			FishEyeCorrect(uncorrectedX, uncorrectedY, correctedX, correctedY);

			m_contours[i][j].x = (int)correctedX;
			m_contours[i][j].y = (int)correctedY;
		}
	}
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
	
	Vec4f lineOutput;

#ifdef USE_OPENCV_FIT_LINE
	for (int i = 0; i < m_contours.size(); i++)
	{
		
		fitLine(m_contours[i], lineOutput, 2, 0, 0.01, 0.01);	// Third arg: DIST_L2 = 2 is the fastest least squares method
		auto vx = lineOutput[0];
		auto vy = lineOutput[1];
		auto x = lineOutput[2];
		auto y = lineOutput[3];
		auto lefty = round((-x * vy / vx) + y);
		auto righty = round(((m_drawing.cols - x) * vy / vx) + y);
		Point point1(m_drawing.cols - 1, (int)righty);
		Point point2(0, (int)lefty);
		Scalar lineColor(0, 255, 0);
		line(m_drawing, point1, point2, lineColor, 2, 4, 0);	// Sixth arg LINE_4 = 4 px wide line
	}
#endif

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

#ifdef BUILD_ON_WINDOWS
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
#else
	vector<Scalar> colors;
	colors.push_back({ 0,   0, 255 });
	colors.push_back({   0, 255,   0 });
	colors.push_back({ 255,   0,   0 });
	colors.push_back({ 255, 255,   0 });
	colors.push_back({ 255,   0, 255 });
	colors.push_back({   0, 255, 255 });
	colors.push_back({ 128, 128, 128 });
	colors.push_back({ 128,   0,   0 });
	colors.push_back({   0, 128,   0 });
	colors.push_back({   0,   0, 128 });
#endif
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
	m_object_height = 0.0;
	for (size_t i = 0; i < m_contours.size(); i++)
	{
#ifndef TEST_FILES_WIDE
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
#endif

		float longSide = max(m_minRect[i].size.width, m_minRect[i].size.height);

		if (m_object_height < (double)longSide && side[i] == eLeft)	// TODO choose only left?
		{
			m_object_height = (double)longSide;
			m_object_center_x = m_minRect[i].center.x;
			m_object_center_y = m_minRect[i].center.y;
		}

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
		cv::putText(image, text, areaLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
		sprintf(text, "width: %.2f height: %.2f", m_minRect[i].size.width, m_minRect[i].size.height);
		cv::putText(image, text, whLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
		sprintf(text, "Angle %.2f", angles[i]);
		cv::putText(image, text, angleLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
		sprintf(text, "%s", sideStr[side[i]].c_str());
		cv::putText(image, text, sideLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
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
    m_object_center_x = m.m10 / m.m00;
    m_object_center_y = m.m01 / m.m00;
    circle(m_drawing, Point((int)m_object_center_x, (int)m_object_center_y), 16, c_centerColor, 2);

    circle(m_drawing, Point((int)m_im_center_x, (int)m_im_center_y), 1, c_centerColor);    // Replacement for drawMarker since drawMarker didn't work.
}

void ProcessingBase::FindVerticalRange()
{
    if (m_contours.size() == 0)
    { 
        return;
    }

    // Find cube height per center coordinate of the cube and contour info
    m_object_contour_max_y = 10000;   // assign a big value
    m_object_contour_min_y = 10000;   // assign a big value
	double im_actual_dist;

    for (size_t i = 0; i < m_contours[m_biggestContourLocation].size(); i++)
    {
        im_actual_dist = abs(m_object_center_x - m_contours[m_biggestContourLocation][i].x);
        if (m_object_contour_max_y > im_actual_dist)
        {
            if (m_contours[m_biggestContourLocation][i].y > m_object_center_y) // Find coordinate > object center
            {
                m_object_contour_max_index = (int)i;
                m_object_contour_max_y = im_actual_dist;
            }
        }

        if (m_object_contour_min_y > im_actual_dist)
        {
            if (m_contours[m_biggestContourLocation][i].y <= m_object_center_y) // Find coordinate <= object center
            {
                m_object_contour_min_index = (int)i;
                m_object_contour_min_y = im_actual_dist;
            }
        }
    }
}

void ProcessingBase::CalcObjectHeight()
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

        x_original[0] = m_object_center_x;
        x_original[1] = m_object_center_x;
        x_original[2] = m_object_center_x;
    
        y_original[0] = m_contours[m_biggestContourLocation][m_object_contour_min_index].y;
        y_original[1] = m_object_center_y;
        y_original[2] = m_contours[m_biggestContourLocation][m_object_contour_max_index].y;

        for (int i = 0; i < 3; i++)
        {
			FishEyeCorrect(x_original[i], y_original[i], x_corrected[i], y_corrected[i]);
        }

        m_object_height = abs(y_corrected[2] -  y_corrected[0]);
        m_object_center_x = x_corrected[1];
        m_object_center_y = y_corrected[1];
    }
    else   
    {
        // Return the cube height with Fish Eye correction
        m_object_height = 	abs(m_contours[m_biggestContourLocation][m_object_contour_max_index].y -  
                                m_contours[m_biggestContourLocation][m_object_contour_min_index].y);
    }
}

void ProcessingBase::FishEyeCorrect(double xIn, double yIn, double& xOut, double& yOut)
{
	if (m_bFishEyeCorrection)
	{
#ifndef BUILD_ON_WINDOWS
		long int start_time;
		long int time_difference;
		struct timespec gettime_now;

		clock_gettime(CLOCK_REALTIME, &gettime_now);
		start_time = gettime_now.tv_nsec;		//Get nS value
#endif

		int ndx = (int)((c_imageWidthPixel * yIn) + xIn);		// [y][x] if 2D array
		uint16_t col = 0;
		uint16_t row = 0;
		if (ndx < c_imageWidthPixel * c_imageHeightPixel)
		{
			col = m_pXfisheyeData[ndx];
			row = m_pYfisheyeData[ndx];
        }

		if (row == 0)
		{
			yOut = yIn;
		}
		else   
		{
			yOut = row - 1;
		}

		if (col == 0)
		{
			xOut = xIn;
		}
		else
		{
			xOut = col - 1;
		}

#if TEST_TRIG_FISHEYE_CONVERSION
		// Apply distortion correction for fisheye camera to three sets of coordinates only to speed up calculation
		double xt;
		double yt;
		double ut;
		double vt;
		double r;
		double theta;
		double s;
		double s2;

		xt = xIn - m_im_center_x;
		yt = yIn - m_im_center_y;

		r = sqrt(xt * xt + yt * yt);					// Find radius
		theta = atan(yt / xt) * m_radiansToDegrees; 	// Find theta for the angle

		if (yt > 0 && xt >= 0)
		{
			//theta = theta;
		}
		else if (yt < 0 && xt >= 0)
		{
			theta += 360.0;
		}
		else if (yt >= 0 && xt <= 0)
		{
			theta += 180.0;
		}
		else if (yt < 0 && xt <= 0)
		{
			theta += 180.0;
		}

		r = r / m_R;									// Normalize the polar coordinate r
		s = r * (1 + m_k * r);							// Apply r-based transform with k
		s2 = s * m_R;									// Un-normalize s
		s2 = s2 * m_borderCorr;							// Scale radius 
		ut = s2 * cos(theta * m_degreesToRadians);		// Converted back to cartesian coordinates
		vt = s2 * sin(theta * m_degreesToRadians);

		xOut = ut + m_im_center_x;						// Add image center back 
		yOut = vt + m_im_center_y;
#endif
#ifndef BUILD_ON_WINDOWS
		clock_gettime(CLOCK_REALTIME, &gettime_now);
		time_difference = gettime_now.tv_nsec - start_time;
		if (time_difference < 0)
			time_difference += 1000000000;				//(Rolls over every 1 second)
		cout << "Fisheye correction took " << time_difference << " nanoseconds" << endl;
#endif
	}
}

void ProcessingBase::CalcOutputValues()
{
    if (m_contours.size() == 0)
    { 
        return;
    }
    
	double total_Distance_Inch = 0.0;							//!< [inch] Estimated total distance as the crow flies from robot to target
	double vertical_Distance_Pixel = 0.0;						//!< [pixel] Estimated distance from floor to target
	double vertical_Distance_Inch = 0.0;						//!< [inch] Estimated distance from floor to target
	double vertical_Angle_Degree = 0.0;							//!< [inch] Estimated angle in the vertical plane from robot to target
	double horizontal_Distance_Pixel = 0.0;						//!< [pixel] Estimated distance from camera to target
	double horizontal_Distance_Inch = 0.0;						//!< [inch] Estimated distance from camera to target
	double comp_Horizontal_Distance_Inch = 0.0;					//!< [inch] Estimated distance from camera to target with compensation for offset camera
	//double forward_Distance_Inch = 0.0;						//!< [inch] Estimated distance to command the robot forward
	
	double scaledHeightPixel = c_imageHeightPixel / (double)m_drawing.size().height;
	double standard_height_p = m_calibTargetSizePixel / scaledHeightPixel;
	double pixel_per_in = m_defaultPixelPerInch / scaledHeightPixel;

	total_Distance_Inch = ((standard_height_p / m_object_height) * m_calibCameraDistInch);
	horizontal_Distance_Inch = (m_object_center_x - m_im_center_x) / pixel_per_in;	// Convert horizontal pixel offset to inches @ 18 camera dist
	comp_Horizontal_Distance_Inch = horizontal_Distance_Inch + c_camera_offset_x0;	// because camera at right side of robot when facing to object 
	vertical_Distance_Pixel = m_im_center_y - m_object_center_y;
	vertical_Distance_Inch = vertical_Distance_Pixel / pixel_per_in;				// Convert vertical pixel offset to inches @ 18 camera dist
	m_Horizontal_Angle_Degree = atan(comp_Horizontal_Distance_Inch / m_calibCameraDistInch) * m_radiansToDegrees;
	vertical_Angle_Degree = atan(vertical_Distance_Pixel / (pixel_per_in * m_calibCameraDistInch)) * m_radiansToDegrees;
	//forward_Distance_Inch = total_Distance_Inch * cos(m_Vertical_Angle_Degree * m_degreesToRadians) * cos(m_Horizontal_Angle_Degree * m_degreesToRadians);
	m_Actual_Distance_Inch = total_Distance_Inch * cos(vertical_Angle_Degree * m_degreesToRadians);

    // Add constraints for cube distance and angle
  //  if ((abs(m_Horizontal_Angle_Degree ) > m_maxAngle) ||
  //      (abs(vertical_Angle_Degree) > m_maxAngle) ||
  //      (m_Actual_Distance_Inch < 0) || (m_Actual_Distance_Inch > m_maxActualDist ))
  //  {
		//m_Horizontal_Angle_Degree = 0.0;
		//m_Actual_Distance_Inch = 0.0;
  //  }

    int quality = eYellowTrackingObjects; // TODO: Calculate this

    m_OutputValues.SetDistance(m_Actual_Distance_Inch);
    m_OutputValues.SetAngle(m_Horizontal_Angle_Degree);
    m_OutputValues.SetQuality(quality);

	PrintDebugValues(comp_Horizontal_Distance_Inch, vertical_Distance_Inch);
}

void ProcessingBase::PrintDebugValues(double horzDistInch, double vertDistInch)
{
	int ndx = loopCounter % testFiles.size();
#ifdef BUILD_ON_WINDOWS
	cout << testFiles[ndx] << endl;
#else
	cout << testFiles[ndx] << endl;
#endif

	cout    << "Counter: " << loopCounter
			<< " Horizontal_Distance_Inch: " << horzDistInch
			<< " Vertical_Distance_Inch " << vertDistInch
			<< " Actual_Distance_Inch: " << m_Actual_Distance_Inch
			<< " Horizontal_Angle " << m_Horizontal_Angle_Degree 
			<< endl;

	cout << endl;
}
