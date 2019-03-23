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
//const char* c_testOutputPath = "C:/Users/212036134/Documents/Personal/FIRST Robotics/TestData/Output/";

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

	//cout << "Loop,"
	//	<< "Dist,"
	//	<< "Area,"
	//	<< "ShortSide,"
	//	<< "LongSide,"
	//	<< "AspectRatio,"
	//	<< "Angle"
	//	<< endl;

#ifdef BUILD_ON_WINDOWS
#ifdef PORTRAIT_IMAGE
	string fileName = "C:/Users/Developer/Documents/TestData/FishEyeCorrected/c_index_960x1280_Portrait_uint16.img";
#else
	string fileName = "C:/Users/Developer/Documents/TestData/FishEyeCorrected/c_index_960x1280_Landscape_uint16.img";
	//string fileName = "C:/Users/212036134/Documents/Personal/FIRST Robotics/TestData/FishEyeCorrected/c_index_960x1280_Landscape_uint16.img";
#endif
#else
#ifdef PORTRAIT_IMAGE
	string fileName = "c_index_960x1280_Portrait_uint16.img";
#else
	string fileName = "c_index_960x1280_Landscape_uint16.img";
#endif
#endif

	m_pXfisheyeData = new uint16_t[c_imageWidthPixel * c_imageHeightPixel];
	m_pYfisheyeData = new uint16_t[c_imageWidthPixel * c_imageHeightPixel];
	memset(m_pXfisheyeData, 0, sizeof(uint16_t) * c_imageWidthPixel * c_imageHeightPixel);
	memset(m_pYfisheyeData, 0, sizeof(uint16_t) * c_imageWidthPixel * c_imageHeightPixel);

	cout << "Loading fisheye correction look up table " << fileName << endl;
	std::ios_base::openmode flags = ios_base::in | ios_base::binary;
	ifstream in(fileName, flags);
	while (!in.eof())
	{
		in.read((char *)m_pXfisheyeData, sizeof(uint16_t) * c_imageWidthPixel * c_imageHeightPixel);
	}
	in.close();

#ifdef BUILD_ON_WINDOWS
#ifdef PORTRAIT_IMAGE
	fileName = "C:/Users/Developer/Documents/TestData/FishEyeCorrected/r_index_960x1280_Portrait_uint16.img";
#else
	fileName = "C:/Users/Developer/Documents/TestData/FishEyeCorrected/r_index_960x1280_Landscape_uint16.img";
	//fileName = "C:/Users/212036134/Documents/Personal/FIRST Robotics/TestData/FishEyeCorrected/r_index_960x1280_Landscape_uint16.img";
#endif
#else
#ifdef PORTRAIT_IMAGE
	fileName = "r_index_960x1280_Portrait_uint16.img";
#else
	fileName = "r_index_960x1280_Landscape_uint16.img";
#endif
#endif

	cout << "Loading fisheye correction look up table " << fileName << endl;
	in.open(fileName, flags);
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

	if (m_imageWriteTask.valid())
	{
		m_imageWriteTask.wait();
	}

	if (m_inrangeWriteTask.valid())
	{
		m_inrangeWriteTask.wait();
	}

	if (m_drawingWriteTask.valid())
	{
		m_drawingWriteTask.wait();
	}

	if (m_trapezoidWriteTask.valid())
	{
		m_trapezoidWriteTask.wait();
	}
}

void ProcessingBase::Prepare(const Mat& imageHSV)
{
    if (c_bUseLastDiagImage)
	{
		//if (loopCounter == 0)
		//{
		//	CalibrateHSV(imageHSV, 67, 75, 5, 50);
		//	CalibrateHSV(imageHSV, 30, 50, 10, 50);
		//}

		cv::inRange(imageHSV, m_lower, m_upper, m_inrange);

		char fileName[255];
#ifdef BUILD_ON_WINDOWS
		int ndx = loopCounter % testFiles.size();
		sprintf_s<sizeof(fileName)>(fileName, "%s%dinrange%s_%s.jpg", c_testOutputPath, ndx + 1, GetTargetName(), testFiles[ndx].c_str());
#else
		if (c_bUseLastDiagImage)
		{
			sprintf(fileName, "inrange%s%d.jpg", GetTargetName(), loopCounter % testFiles.size() + 1);
		}
		else
		{
			sprintf(fileName, "inrange%s%d.jpg", GetTargetName(), loopCounter);
		}
#endif
		//imwrite(fileName, m_inrange);
		SaveFileInBackground(m_inrangeWriteTask, fileName, m_inrange);
    }
    else
    {    
        // Searching for color in the image that has a high of upper scaler and a low of lower scaler. Stores result in inrange
		inRange(imageHSV, m_lower, m_upper, m_inrange);	// Identify color per HSV image

		if (bImageCaptureTrigger)
        {
            // For manually calibrating the camera
			char fileName[255];
#ifdef BUILD_ON_WINDOWS
			sprintf_s<sizeof(fileName)>(fileName, "%sinrange%s_%d.jpg", c_testOutputPath, GetTargetName(), loopCounter);
#else
			sprintf(fileName, "inrange%s_%.0f_%.0f_%d.jpg", GetTargetName(), m_upper[0], m_lower[0], loopCounter);
#endif
			//imwrite(fileName, m_inrange);
			SaveFileInBackground(m_inrangeWriteTask, fileName, m_inrange);
		}
    }
}

void ProcessingBase::CalibrateHSV(const Mat& imageHSV, int hue1, int hue2, int span, int satValueIncrement)
{
	Mat inrange;
	char buf[300];
	for (int h = hue1; h < hue2; h++)
	{
		for (int s = 50; s < 256; s += satValueIncrement)
		{
			for (int v = 50; v < 256; v += satValueIncrement)
			{
				cout << "Generating test inrange for hue " << h << " saturation " << s << " value " << v << endl;
				Scalar lower = Scalar(h, s, v);
				//Scalar upper = Scalar(h + span, 255, 255);
				Scalar upper = Scalar(h + span, 175, 175);
				inRange(imageHSV, lower, upper, inrange);
#ifdef BUILD_ON_WINDOWS
				sprintf_s<sizeof(buf)>(buf, "%sinrange_H%d-%d_S%d_V%d.jpg", c_testOutputPath, h, h + span, s, v);
#else
				sprintf(buf, "inrange_H%d-%d_S%d_V%d.jpg", h, h + span, s, v);
#endif
				//imwrite(buf, inrange);
				SaveFileInBackground(m_inrangeWriteTask, buf, inrange);
			}
		}
	}
}

void ProcessingBase::FindContours()
{
    findContours(m_inrange, m_contours, m_hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    // creates a drawing matrix with inrange size and fills it with zeroes
    m_drawing = Mat::zeros(m_inrange.size(), CV_8UC3);

	// Find image center coordinate
	m_im_center_x = m_drawing.size().width / 2;
	m_im_center_y = m_drawing.size().height / 2;

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
    }

	if (m_contours.size() > 0)
    {
        // Actually draw the contour for the biggest contour we find 
        drawContours(m_drawing, m_contours, m_biggestContourLocation, c_biggestContourColor, 2, 8, m_hierarchy, 0);
    }
    else
    {
        cout << "Did not find any contour " << endl;
    }

	if (c_bUseLastDiagImage || bImageCaptureTrigger)
    {
        // For manually calibrating the camera
		char fileName[255];
#ifdef BUILD_ON_WINDOWS
		int ndx = loopCounter % testFiles.size();
		sprintf_s<sizeof(fileName)>(fileName, "%s%ddrawing%s_%s.jpg", c_testOutputPath, ndx + 1, GetTargetName(), testFiles[ndx].c_str());
#else
		if (c_bUseLastDiagImage)
		{
			sprintf(fileName, "drawing%s%d.jpg", GetTargetName(), loopCounter % testFiles.size() + 1);
		}
		else
		{
			sprintf(fileName, "drawing%s%d.jpg", GetTargetName(), loopCounter);
		}
#endif
        //imwrite(fileName, m_drawing);
		SaveFileInBackground(m_drawingWriteTask, fileName, m_drawing);
	}
}

void ProcessingBase::RejectSmallContours()
{
    // Walk through each contour that we found looking for the biggest contour by point count
    size_t maxSize = 0;
    //cout << "Finding max contour size out of " << m_contours.size() << " contours"<< endl;
	for (size_t i = 0; i < m_contours.size(); i++)
	{
		if (m_contours[i].size() < c_maxContourPoints)
		{
			maxSize = max(m_contours[i].size(), maxSize);
		}
    }

	m_linePoints.clear();
	m_rectDescr.clear();
	vector<vector<Point>> contours;
    size_t threshold = max(c_minContourPoints, (size_t)c_smallContourPercentOfMax * maxSize);
    //cout << "Max contour size " << maxSize << " threshold " << threshold << endl;
    for (size_t i = 0; i < m_contours.size(); i++)
    {
		bool bContourInLowerImage = false;
		for (size_t j = 0; j < m_contours[i].size(); j++)
		{
			if (m_contours[i].at(j).y >= 860)
			{
				bContourInLowerImage = true;
				break;
			}
		}

		if (bContourInLowerImage)
		{
			continue;
		}

		// Block out the upper corners
		// We are getting reflections of the green LEDs off of the diffuser plastic
		int minX = 0;
		int minY = 0;
		int maxX = 125;
		int maxY = 125;
		line(m_drawing, Point(minX, minY), Point(minX, maxY), c_contourColor, 2, 8);
		line(m_drawing, Point(minX, maxY), Point(maxX, maxY), c_contourColor, 2, 8);
		line(m_drawing, Point(maxX, maxY), Point(maxX, minY), c_contourColor, 2, 8);
		line(m_drawing, Point(maxX, minY), Point(minX, minY), c_contourColor, 2, 8);

		auto x = m_contours[i].at(0).x;
		auto y = m_contours[i].at(0).y;
		if (y <= maxY && x <= maxX)
		{
			continue;
		}

		minX = c_imageWidthPixel - 125;
		maxX = c_imageWidthPixel;
		line(m_drawing, Point(minX, minY), Point(minX, maxY), c_contourColor, 2, 8);
		line(m_drawing, Point(minX, maxY), Point(maxX, maxY), c_contourColor, 2, 8);
		line(m_drawing, Point(maxX, maxY), Point(maxX, minY), c_contourColor, 2, 8);
		line(m_drawing, Point(maxX, minY), Point(minX, minY), c_contourColor, 2, 8);

		if (y <= maxY && x >= minX)
		{
			continue;
		}

		if (c_bDrawAllContours)
		{
			drawContours(m_drawing, m_contours, (int)i, c_fishEyeContourColor, 2, 8, m_hierarchy, 0); // Line thickness 2, line type 8, offset 0
		}
		FishEyeCorrectContour(i);

		RectDescr rd;
		rd.m_minRect = minAreaRect(m_contours[i]);

		cout << "Contour size " << m_contours[i].size() << " area " << rd.m_minRect.size.area() << endl;
		//if (m_contours[i].size() > threshold && abs(ld.m_slope) > 1.0f)
		if (m_contours[i].size() > threshold && rd.m_minRect.size.area() > 0.0f)
        {
			//cout << "Saving contour of size " << m_contours[i].size() << endl;
			LineDescr ld = FitLineToContour(m_contours[i]);
			rd.m_originalContourIndex = i;
			rd.m_slope = ld.m_slope;
			if (rd.m_minRect.size.width < rd.m_minRect.size.height)
			{
				rd.m_angle = rd.m_minRect.angle + 180.0f;
			}
			else
			{
				rd.m_angle = rd.m_minRect.angle + 90.0f;
			}
			rd.m_side = eUnknownSide;
			m_rectDescr.push_back(rd);
			m_linePoints.push_back(make_pair(ld.m_point1, ld.m_point2));
			contours.push_back(m_contours[i]);
		}
  //      else
  //      {
  //          cout << "Rejecting contour of size " << m_contours[i].size() << endl;
  //      }
    }

    m_contours.swap(contours);

	for (size_t i = 0; i < m_contours.size(); i++)
	{
		if (c_bDrawAllContours)
		{
			drawContours(m_drawing, m_contours, (int)i, c_contourColor, 2, 8, m_hierarchy, 0); // Line thickness 2, line type 8, offset 0
		}
	}

#ifdef DRAW_OPENCV_FIT_LINE
	for (auto& it : m_linePoints)
	{
		line(m_drawing, it.first, it.second, c_lineColor, 2, 4, 0);	// Sixth arg LINE_4 = 4 px wide line
	}
#endif
	if (c_bUseLastDiagImage || bImageCaptureTrigger)
	{
		// For manually calibrating the camera
		char fileName[255];
#ifdef BUILD_ON_WINDOWS
		int ndx = loopCounter % testFiles.size();
		sprintf_s<sizeof(fileName)>(fileName, "%s%ddrawing%s_%s.jpg", c_testOutputPath, ndx + 1, GetTargetName(), testFiles[ndx].c_str());
#else
		if (c_bUseLastDiagImage)
		{
			sprintf(fileName, "drawing%s%d.jpg", GetTargetName(), loopCounter % testFiles.size() + 1);
		}
		else
		{
			sprintf(fileName, "drawing%s%d.jpg", GetTargetName(), loopCounter);
		}
#endif
		//imwrite(fileName, m_drawing);
		SaveFileInBackground(m_drawingWriteTask, fileName, m_drawing);
	}
}

void ProcessingBase::FishEyeCorrectContours()
{
	//cout << __func__ << " start" << endl;

	for (size_t i = 0; i < m_contours.size(); i++)
	{
		FishEyeCorrectContour(i);
	}

	//cout << __func__ << " end" << endl;
}

void ProcessingBase::FishEyeCorrectContour(size_t index)
{
	//cout << __func__ << " start" << endl;

	for (size_t j = 0; j < m_contours[index].size(); j++)
	{
		int uncorrectedX = m_contours[index][j].x;
		int uncorrectedY = m_contours[index][j].y;
		int correctedX;
		int correctedY;

		FishEyeCorrectPoint(uncorrectedX, uncorrectedY, correctedX, correctedY);

		m_contours[index][j].x = correctedX;
		m_contours[index][j].y = correctedY;
	}
	//cout << __func__ << " end" << endl;
}

//void ProcessingBase::SortContours()
//{
//	std::sort(m_contours.begin(), m_contours.end(), [](vector<Point>& innerVec)
//	{
//		if ()
//	});
//}

void ProcessingBase::FitLinesToContours()
{
	Vec4f lineOutput;
	m_linePoints.clear();
	m_rectDescr.clear();
	//m_rectDescr.resize(m_contours.size());
	for (int i = 0; i < m_contours.size(); i++)
	{
		fitLine(m_contours[i], lineOutput, 2, 0, 0.01, 0.01);	// Third arg: DIST_L2 = 2 is the fastest least squares method

		// Output line parameters.
		// A vector of 4 elements (vx, vy, x0, y0), where(vx, vy) is a normalized vector collinear to the line and (x0, y0) is a point on the line.
		float vx = lineOutput[0];
		if (vx != 0.0f)
		{
			float vy = lineOutput[1];
			float x = lineOutput[2];
			float y = lineOutput[3];

			float  lefty = round((-x * vy / vx) + y);
			float  righty = round(((m_drawing.cols - x) * vy / vx) + y);
			Point point1(m_drawing.cols - 1, (int)righty);
			Point point2(0, (int)lefty);
			m_linePoints.push_back(make_pair(point1, point2));

			float slope = 0.0f;
			if (vx != 0.0f)
			{
				slope = vy / vx;
			}
			RectDescr rd;
			rd.m_slope = slope;
			m_rectDescr.push_back(rd);
			//m_rectDescr[i].m_slope = slope;
			//cout << "slope  " << slope << endl;
		}

		if (c_bDrawAllContours)
		{
			drawContours(m_drawing, m_contours, (int)i, c_contourColor, 2, 8, m_hierarchy, 0); // Line thickness 2, line type 8, offset 0
		}
	}

#ifdef DRAW_OPENCV_FIT_LINE
	for (auto& it : m_linePoints)
	{
		line(m_drawing, it.first, it.second, c_lineColor, 2, 4, 0);	// Sixth arg LINE_4 = 4 px wide line
	}
#endif
}

LineDescr ProcessingBase::FitLineToContour(const vector<Point>& contour)
{
	LineDescr ld;
	ld.m_slope = 0.0f;
	ld.m_point1.x = 0;
	ld.m_point1.y = 0;
	ld.m_point2.x = 0;
	ld.m_point2.y = 0;

	Vec4f lineOutput;
	fitLine(contour, lineOutput, 2, 0, 0.01, 0.01);	// Third arg: DIST_L2 = 2 is the fastest least squares method

	// Output line parameters.
	// A vector of 4 elements (vx, vy, x0, y0), where(vx, vy) is a normalized vector collinear to the line and (x0, y0) is a point on the line.
	float vx = lineOutput[0];
	if (vx != 0.0f)
	{
		float vy = lineOutput[1];
		float x = lineOutput[2];
		float y = lineOutput[3];

		float  lefty = round((-x * vy / vx) + y);
		float  righty = round(((m_drawing.cols - x) * vy / vx) + y);
		ld.m_point1.x = m_drawing.cols - 1;
		ld.m_point1.y = (int)righty;
		ld.m_point2.y = (int)lefty;

		if (vx != 0.0f)
		{
			ld.m_slope = vy / vx;
		}
		ld.m_yIntercept = y - ld.m_slope * x;
	}

	return ld;
}

void ProcessingBase::ApproximatePolygons()
{
	//cout << __func__ << " start" << endl;
	if (m_contours.size() == 0)
	{
		//cout << __func__ << " end no countours" << endl;
		return;
	}
	Mat image(m_inrange.rows, m_inrange.cols, CV_8UC3, Scalar(0));
	vector<Point> vertVectOut;

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

	char fileName[255];
#ifdef BUILD_ON_WINDOWS
	int ndx = loopCounter % testFiles.size();
	sprintf_s<sizeof(fileName)>(fileName, "%s%d_polygons_%s", c_testOutputPath, ndx + 1, testFiles[ndx].c_str());
#else
	if (c_bUseLastDiagImage)
	{
		sprintf(fileName, "polygons%d.jpg", loopCounter % testFiles.size() + 1);
	}
	else
	{
		sprintf(fileName, "polygons.jpg");
	}
#endif
	//imwrite(fileName, image);
	if (bImageCaptureTrigger)
	{
		SaveFileInBackground(m_imageWriteTask, fileName, image);
	}
}


void ProcessingBase::FindCornerCoordinates()
{
	//cout << __func__ << " start" << endl;
	if (m_contours.size() == 0)
	{
		//cout << __func__ << " end no countours" << endl;
		return;
	}

	Mat image(m_inrange.rows, m_inrange.cols, CV_8UC3, Scalar(0));
	//cout << "Finding minimum area rotated rectangles for " << m_contours.size() << " contours" << endl;

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

	size_t numContours = m_contours.size();

	// Sort by x coord of minimum area rect
	std::sort(m_rectDescr.begin(), m_rectDescr.end(), [](RectDescr& rd1, RectDescr& rd2)
	{
		return rd1.m_minRect.center.x < rd2.m_minRect.center.x;
	});
	//-------------------------------------------------------------
	// NOTE: m_rectDescr is not in the same order as m_contours now
	//-------------------------------------------------------------

	float minXdiff = FLT_MAX;			// Minimum difference of pair center to image center
	size_t minPairIndex = 0;
	for (size_t i = 0; i < m_rectDescr.size(); i++)
	{
		//cout << "Find left/right" << endl;
		// Is this a left or right leaning retroreflective target 
		if (numContours > 1 && i < numContours - 1)
		{
			float m1 = m_rectDescr[i].m_slope;
			float m2 = m_rectDescr[i + 1].m_slope;

			bool bSameSignTwoGtOne = (m2 < m1) && ((m1 > 0 && m2 > 0) || (m1 < 0 && m2 < 0));
			bool bChangeSign = (m1 < 0 && m2 > 0);	// if (m1 > 0 && m2 < 0) you get the intersection on the other side 

			if (bSameSignTwoGtOne || bChangeSign)
			{
				//cout << "Intersection above" << endl;
				m_rectDescr[i].m_side = eLeft;
				m_rectDescr[i + 1].m_side = eRight;
				//float xDiff = m_rectDescr[i + 1].m_minRect.center.x - m_rectDescr[i].m_minRect.center.x;
				//cout << "maxXdiff " << maxXdiff << " xDiff " << xDiff << endl;
				float xDiffLeft = abs((float)m_im_center_x - m_rectDescr[i].m_minRect.center.x);
				float xDiffRight = abs((float)m_im_center_x - m_rectDescr[i + 1].m_minRect.center.x);
				//cout << "minXdiff " << minXdiff << " xDiffLeft " << xDiffLeft << " xDiffRight " << xDiffRight << endl;
				float xDiff = min(xDiffLeft, xDiffRight);

				if (minXdiff > xDiff)
				{
					minXdiff = xDiff;
					minPairIndex = i;
				}
				i++;
			}
			else
			{
				//cout << "Intersection below" << endl;
				m_rectDescr[i].m_side = eRight;
			}
		}
	}
	//cout << "final maxXdiff " << maxXdiff << " maxPairIndex " << maxPairIndex << endl;
	//cout << "final minXdiff " << minXdiff << " maxPairIndex " << minPairIndex << endl;

	m_leftTarget.m_side = eUnknownSide;
	m_rightTarget.m_side = eUnknownSide;

	const vector<string> sideStr =
	{
		  "Unknown"
		, "Left"
		, "Right"
	};
	//cout << "Screen out bad rectangles" << endl;
	Point2f vertices[4];
#ifdef WRITE_OPENCV_TEXT_ON_IMAGES
	char text[255];
#endif
	//const float areaThreshold = c_areaThresholdPercent * maxArea;
	m_object_height = 0.0;
	size_t endIndex = min(minPairIndex + 2, m_rectDescr.size());
	for (size_t i = minPairIndex; i < endIndex; i++)			// TODO this is kind of hacky
	{
		float longSide = max(m_rectDescr[i].m_minRect.size.width, m_rectDescr[i].m_minRect.size.height);
		//float longSide = max(m_rectDescr[i].m_minRect.boundingRect2f().width, m_rectDescr[i].m_minRect.boundingRect2f().height);
		//cout << "bounding box width " << m_rectDescr[i].m_minRect.boundingRect2f().width << " height " << m_rectDescr[i].m_minRect.boundingRect2f().height << endl;

		float shortSide = 0.0f;
		float aspectRatio = 0.0f;
		//cout << "longSide " << longSide << endl;
		if (longSide > 0.0f)
		{
			if (m_object_height < (double)longSide && m_rectDescr[i].m_side == eLeft)
			{
#define USE_BOUNDING_BOX
#ifdef USE_BOUNDING_BOX
				longSide = max(m_rectDescr[i + 1].m_minRect.boundingRect2f().width, m_rectDescr[i + 1].m_minRect.boundingRect2f().height);
				m_object_height = max(m_object_height, (double)longSide);
#else
				m_object_height = (double)longSide;
#endif
				m_object_center_x = (m_rectDescr[i].m_minRect.center.x + m_rectDescr[i + 1].m_minRect.center.x) / 2.0f;
				m_object_center_y = (m_rectDescr[i].m_minRect.center.y + m_rectDescr[i + 1].m_minRect.center.y) / 2.0f;
				//m_object_center_y = m_rectDescr[i].m_minRect.center.y;
				circle(image, Point((int)m_object_center_x, (int)m_object_center_y), 16, c_centerColor, 2);

				m_leftTarget = m_rectDescr[i];
				m_rightTarget = m_rectDescr[i + 1];
			}

			shortSide = min(m_rectDescr[i].m_minRect.size.width, m_rectDescr[i].m_minRect.size.height);
			//cout << "shortSide " << shortSide << endl;
			if (shortSide > 0.0f)
			{
				aspectRatio = shortSide / longSide;
				//cout << "aspectRatio " << aspectRatio << endl;
				//if (aspectRatio > c_occludedAspectRatio)
				//{
				//	cout << " aspect ratio greater than 0.4 may be occluded by hatch " << aspectRatio << endl;
				//}

				auto color = colors[i % colors.size()];
				m_rectDescr[i].m_minRect.points(vertices);
				for (int j = 0; j < 4; j++)
				{
					line(image, vertices[j], vertices[(j + 1) % 4], color, 2, 8);
				}

#ifdef WRITE_OPENCV_TEXT_ON_IMAGES
				//cout << "Write text on images" << endl;
				//void putText(InputOutputArray img, const String& text, Point org, int fontFace, double fontScale, Scalar color, int thickness = 1, int lineType = LINE_8, bool bottomLeftOrigin = false)
				const double fontScale = 0.37;
				const int textThickness = 1;
				float offset = m_rectDescr[i].m_side == eRight ? 50.f : 0.0f;
				float y = i * 10.0f;
				Point2f areaLoc(vertices[0].x - offset, y + 10.0f);
				Point2f whLoc(vertices[0].x - offset, y + 30.0f);
				Point2f angleLoc(vertices[0].x - offset, y + 50.0f);
				Point2f sideLoc(vertices[0].x - offset, y + 70.0f);
				Point2f slopeLoc(vertices[0].x - offset, y + 90.0f);
#ifdef BUILD_ON_WINDOWS
				sprintf_s<sizeof(text)>(text, "Area: %.2f", m_rectDescr[i].m_minRect.size.area());
				cv::putText(image, text, areaLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
				sprintf_s<sizeof(text)>(text, "width: %.2f height: %.2f", m_rectDescr[i].m_minRect.size.width, m_rectDescr[i].m_minRect.size.height);
				cv::putText(image, text, whLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
				sprintf_s<sizeof(text)>(text, "Angle %.2f", m_rectDescr[i].m_angle);
				cv::putText(image, text, angleLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
				sprintf_s<sizeof(text)>(text, "%s", sideStr[m_rectDescr[i].m_side].c_str());
				cv::putText(image, text, sideLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
				sprintf_s<sizeof(text)>(text, "Slope %.3f index %zu", m_rectDescr[i].m_slope, i);
				cv::putText(image, text, slopeLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
				if (aspectRatio > c_occludedAspectRatio)
				{
					Point2f loc(vertices[0].x - offset, y + 110.0f);
					sprintf_s<sizeof(text)>(text, "Occluded - Aspect ratio %.2f", aspectRatio);
					cv::putText(image, text, loc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
				}
#else
				sprintf(text, "Area: %.2f", m_rectDescr[i].m_minRect.size.area());
				cv::putText(image, text, areaLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
				sprintf(text, "width: %.2f height: %.2f", m_rectDescr[i].m_minRect.size.width, m_rectDescr[i].m_minRect.size.height);
				cv::putText(image, text, whLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
				sprintf(text, "Angle %.2f", m_rectDescr[i].m_angle);
				cv::putText(image, text, angleLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
				sprintf(text, "%s", sideStr[m_rectDescr[i].m_side].c_str());
				cv::putText(image, text, sideLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
				sprintf(text, "Slope %.3f", m_rectDescr[i].m_slope);
				cv::putText(image, text, slopeLoc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
				if (aspectRatio > c_occludedAspectRatio)
				{
					Point2f loc(vertices[0].x - offset, y + 110.0f);
					sprintf(text, "Occluded - Aspect ratio %.2f", aspectRatio);
					cv::putText(image, text, loc, FONT_HERSHEY_SIMPLEX, fontScale, color, textThickness, LINE_AA);
				}
#endif
#endif	// WRITE_OPENCV_TEXT_ON_IMAGES
			}
		}
	}

	if (c_bUseLastDiagImage || bImageCaptureTrigger)
	{
		char fileName[255];
#ifdef BUILD_ON_WINDOWS
		int ndx = loopCounter % testFiles.size();
		sprintf_s<sizeof(fileName)>(fileName, "%s%d_trapezoid%s_%s.jpg", c_testOutputPath, ndx + 1, GetTargetName(), testFiles[ndx].c_str());
#else
		if (c_bUseLastDiagImage)
		{
			sprintf(fileName, "trapezoid%s%d.jpg", GetTargetName(), loopCounter % testFiles.size() + 1);
		}
		else
		{
			sprintf(fileName, "trapezoid%s%d.jpg", GetTargetName(), loopCounter);
		}
#endif
		//imwrite(fileName, image);
		SaveFileInBackground(m_trapezoidWriteTask, fileName, image);
	}
	//cout << __func__ << " end" << endl;
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

    // Find object center coordinate and draw a circle at that point
    m_object_center_x = m.m10 / m.m00;
    m_object_center_y = m.m01 / m.m00;
    circle(m_drawing, Point((int)m_object_center_x, (int)m_object_center_y), 16, c_centerColor, 2);

    circle(m_drawing, Point((int)m_im_center_x, (int)m_im_center_y), 1, c_centerColor);    // Replacement for drawMarker since drawMarker didn't work.
}

void ProcessingBase::FindVerticalRange()
{
	//cout << __func__ << " start" << endl;
	if (m_contours.size() == 0)
    { 
		//cout << __func__ << " end no countours" << endl;
		return;
    }
	//cout << "FindVerticalRange() m_contours.size() " << m_contours.size() << " m_biggestContourLocation " << m_biggestContourLocation << endl;

    // Find object contour height (check each point in relation to object center coordinate)
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
	//cout << __func__ << " end" << endl;
}

void ProcessingBase::CalcObjectHeight()
{
	//cout << __func__ << " start" << endl;
	if (m_contours.size() == 0)
    { 
		//cout << __func__ << " end no countours" << endl;
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
			FishEyeCorrectPoint(x_original[i], y_original[i], x_corrected[i], y_corrected[i]);
        }

        m_object_height = abs(y_corrected[2] -  y_corrected[0]);
        m_object_center_x = x_corrected[1];
        m_object_center_y = y_corrected[1];
    }
    else   
    {
        // Return the object height with Fish Eye correction
        m_object_height = 	abs(m_contours[m_biggestContourLocation][m_object_contour_max_index].y -  
                                m_contours[m_biggestContourLocation][m_object_contour_min_index].y);
    }
	//cout << __func__ << " end" << endl;
}

void ProcessingBase::FishEyeCorrectPoint(double xIn, double yIn, double& xOut, double& yOut)
{
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
}

void ProcessingBase::FishEyeCorrectPoint(int xIn, int yIn, int& xOut, int& yOut)
{
	int ndx = ((c_imageWidthPixel * yIn) + xIn);		// [y][x] if 2D array
	uint16_t col = 0;
	uint16_t row = 0;
	if (ndx < c_imageWidthPixel * c_imageHeightPixel)
	{
		col = m_pXfisheyeData[ndx];
		row = m_pYfisheyeData[ndx];
	}

	if (row == 0)
	{
		yOut = 0;// yIn;
	}
	else
	{
		yOut = row - 1;
	}

	if (col == 0)
	{
		xOut = 0;//xIn;
	}
	else
	{
		xOut = col - 1;
	}
}

template <class Task>
void ProcessingBase::SaveFileInBackground(Task& writeTask, const std::string& fileName, const Mat& matrix)
{
	if (writeTask.valid())
	{
		cout << "Waiting for previous write task to complete" << endl;
		writeTask.wait();
		cout << "Done waiting for previous write task to complete" << endl;
	}

	// Do not pass the arguments as references; we need copies, otherwise both the foregroud and background threads will access the objects
	writeTask = async(launch::async, [fileName, matrix]()
	{
		imwrite(fileName, matrix);
	});
}

void ProcessingBase::CalcOutputValues(const char* objType)
{
	cout << "calling CalcOutputValues " << objType
		<< " m_object_height " << m_object_height
		<< " object x " << m_object_center_x
		<< " object y " << m_object_center_y
		<< endl;
	EQuality quality = CalcOutputValues(objType, m_object_height, m_object_center_x, m_object_center_y, m_Actual_Distance_Inch, m_Horizontal_Angle_Degree);
	m_OutputValues.SetDistance(m_Actual_Distance_Inch);
	m_OutputValues.SetAngle(m_Horizontal_Angle_Degree);
	m_OutputValues.SetQuality(quality);
}

EQuality ProcessingBase::CalcOutputValues(const char* objType, double objHeight, double objCenterX, double objCenterY, double& actualDistInch, double& horzAngleDegree)
{
	//cout << __func__ << " start" << endl;
	if (m_contours.size() == 0)
    { 
		//cout << __func__ << " end no countours" << endl;
		return eRedNoData;
	}

	//cout << "m_object_height " << m_object_height << endl;
	if (objHeight == 0.0)
	{
		//cout << __func__ << " end no object height" << endl;
		return eRedNoData;
	}

	double total_Distance_Inch = 0.0;							//!< [inch] Estimated total distance as the crow flies from robot to target
	double vertical_Distance_Pixel = 0.0;						//!< [pixel] Estimated distance from floor to target
	double vertical_Distance_Inch = 0.0;						//!< [inch] Estimated distance from floor to target
	double vertical_Angle_Degree = 0.0;							//!< [inch] Estimated angle in the vertical plane from robot to target
	double horizontal_Distance_Pixel = 0.0;						//!< [pixel] Estimated distance from camera to target
	double horizontal_Distance_Inch = 0.0;						//!< [inch] Estimated distance from camera to target
	double comp_Horizontal_Distance_Inch = 0.0;					//!< [inch] Estimated distance from camera to target with compensation for offset camera
	//double forward_Distance_Inch = 0.0;						//!< [inch] Estimated distance to command the robot forward
	double drawingHeight = (double)m_drawing.size().height;
	if (drawingHeight == 0.0)
	{
		drawingHeight = 1.0;
	}

	double scaledHeightPixel = c_imageHeightPixel / drawingHeight;
	double standard_height_p = m_calibTargetSizePixel / scaledHeightPixel;
	double pixel_per_in = m_defaultPixelPerInch / scaledHeightPixel;

	total_Distance_Inch = ((standard_height_p / objHeight) * m_calibCameraDistInch);
	//cout << "total_Distance_Inch: " << total_Distance_Inch << endl;
	horizontal_Distance_Inch = (objCenterX - m_im_center_x) / pixel_per_in;	// Convert horizontal pixel offset to inches
	//cout << "horizontal_Distance_Inch: " << horizontal_Distance_Inch << endl;
	comp_Horizontal_Distance_Inch = horizontal_Distance_Inch + c_camera_offset_x0;
	//cout << "comp_Horizontal_Distance_Inch: " << comp_Horizontal_Distance_Inch << endl;
	vertical_Distance_Pixel = m_im_center_y - objCenterY;
	//cout << "vertical_Distance_Pixel: " << vertical_Distance_Pixel << endl;
	vertical_Distance_Inch = vertical_Distance_Pixel / pixel_per_in;				// Convert vertical pixel offset to inches
	//cout << "vertical_Distance_Inch: " << vertical_Distance_Inch << endl;
	horzAngleDegree = atan(comp_Horizontal_Distance_Inch / m_calibCameraDistInch) * m_radiansToDegrees;
	//cout << "m_Horizontal_Angle_Degree: " << horzAngleDegree << endl;
	vertical_Angle_Degree = atan(vertical_Distance_Pixel / (pixel_per_in * m_calibCameraDistInch)) * m_radiansToDegrees;
	//cout << "vertical_Angle_Degree: " << vertical_Angle_Degree << endl;
	actualDistInch = total_Distance_Inch * cos(horzAngleDegree * m_degreesToRadians) - m_cameraToFrontOfRobotDistInch;
	double fudge = 36.0 * actualDistInch / 100.0;	// Remove the effect of subtracting m_cameraToFrontOfRobotDistInch as you get further away
	actualDistInch += fudge;
	//actualDistInch = total_Distance_Inch * cos(vertical_Angle_Degree * m_degreesToRadians) - m_cameraToFrontOfRobotDistInch;
	//actualDistInch = total_Distance_Inch * cos(vertical_Angle_Degree * m_degreesToRadians);
	cout << "m_Actual_Distance_Inch: " << actualDistInch << endl;

	EQuality quality = eYellowTrackingObjects;

	// Constraint output values
    if ((abs(horzAngleDegree) > m_maxAngle) ||
        (abs(vertical_Angle_Degree) > m_maxAngle) ||
        (actualDistInch < 0) || (actualDistInch > m_maxActualDist ))
    {
		cout << "One or more calculated output values are out of bounds, setting to horz angle and dist to zero" << endl;
		cout << " m_Actual_Distance_Inch : " << actualDistInch
			<< " m_Horizontal_Angle_Degree: " << horzAngleDegree
			<< " vertical_Angle_Degree: " << vertical_Angle_Degree
			<< endl;
		horzAngleDegree = 0.0;
		actualDistInch = 0.0;
		quality = eRedNoData;
	}
	else
	{
		quality = eGreenReady;
	}

	PrintDebugValues(objType, comp_Horizontal_Distance_Inch, vertical_Distance_Inch, objHeight, actualDistInch, horzAngleDegree);
	//cout << __func__ << " end" << endl;

	return quality;
}

void ProcessingBase::PrintDebugValues(const char* objType, double horzDistInch, double vertDistInch, double objHeight, double actualDistInch, double horzAngleDegree)
{
	int ndx;
	double camDist;
	
	if (c_bUseLastDiagImage)
	{
		ndx = loopCounter % testFiles.size();
		camDist = testDist[loopCounter % testFiles.size()];
		cout << testFiles[ndx] << endl;
	}
	else
	{
		ndx = 0;
		camDist = 0.0;
	}

//#define CSV_OUTPUT
#ifdef CSV_OUTPUT
	// For comma separated value (CSV) output
	if (loopCounter == 0)
	{
		cout << "Counter,ObjType,ObjHeight,Horizontal_Distance_Inch,Vertical_Distance_Inch,CamDist,EstActualDist,Horizontal_Angle,TestFile" << endl;
	}

	cout << loopCounter << ","
		<< " " << objType
		<< objHeight << ","
		<< horzDistInch << ","
		<< vertDistInch << ","
		<< camDist << ","
		<< actualDistInch << ","
		<< horzAngleDegree << ","
		<< testFiles[ndx]
		<< endl;
#else
	cout    << "Counter: " << loopCounter
			<< " " << objType
			<< " Horizontal_Distance_Inch: " << horzDistInch
			<< " Vertical_Distance_Inch " << vertDistInch
			<< " Actual_Distance_Inch: " << actualDistInch
			<< " Test Image Camera dist: " << camDist
			<< " Diff from actual: " << actualDistInch - camDist
			<< " Horizontal_Angle " << horzAngleDegree
			<< endl;

	//cout << endl;
#endif
}
