/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
 * 
 * Class for processing white gaffer's tape on floor for the 2019 FRC Deep Space Challenge
*/

#include "ProcessingLine.h"

ProcessingLine::ProcessingLine()
	//: ProcessingBase({ 179, 52, 255 },
	//				 {   0,  0, 180 })     // White gaffer tape test
	//: ProcessingBase({  60, 175, 175 },
	//				 {  40,  45, 100 })     // White gaffer tape test
	: ProcessingBase({ 179,  51, 255 },
					 {   0,   0, 178 })     // White gaffer tape test
{
}

ProcessingLine::~ProcessingLine()
{
}

void ProcessingLine::RejectSmallContours()
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
		// TODO need this?
		//bool bContourInLowerImage = false;
		//for (size_t j = 0; j < m_contours[i].size(); j++)
		//{
		//	if (m_contours[i].at(j).y >= 860)
		//	{
		//		bContourInLowerImage = true;
		//		break;
		//	}
		//}

		//if (bContourInLowerImage)
		//{
		//	continue;
		//}

		RectDescr rd;
		rd.m_minRect = minAreaRect(m_contours[i]);

		int minX = 0;
		int minY = 0;
		int maxX = c_imageWidthPixel;
		int maxY = c_imageHeightPixel;
		if (m_leftTarget.m_side != eUnknownSide && m_rightTarget.m_side != eUnknownSide)
		{
			// Set up a bounding box in which we look for the white gaffer's tape on the floor
			minX = (int)m_leftTarget.m_minRect.center.x - 50;	// Give a little margin since we are measuring from the center of the rotated rect
			maxX = (int)m_rightTarget.m_minRect.center.x + 50;
			minY = (int)m_leftTarget.m_minRect.center.y + 150;  // The lines are on the floor
			maxY = (int)m_rightTarget.m_minRect.center.y + 350;	// The lines are on the floor

			line(m_drawing, Point(minX, minY), Point(minX, maxY), c_contourColor, 2, 8);
			line(m_drawing, Point(minX, maxY), Point(maxX, maxY), c_contourColor, 2, 8);
			line(m_drawing, Point(maxX, maxY), Point(maxX, minY), c_contourColor, 2, 8);
			line(m_drawing, Point(maxX, minY), Point(minX, minY), c_contourColor, 2, 8);
		}

		auto x = m_contours[i].at(0).x;
		auto y = m_contours[i].at(0).y;
		if (m_contours[i].size() > threshold && rd.m_minRect.size.area() > 0.0f && y >= minY && y <= maxY && x >= minX && x <= maxX)
		{
			LineDescr ld = FitLineToContour(m_contours[i]);
			rd.m_originalContourIndex = i;
			rd.m_slope = ld.m_slope;
			rd.m_yIntercept = ld.m_yIntercept;
			if (rd.m_minRect.size.width < rd.m_minRect.size.height)
			{
				rd.m_angle = rd.m_minRect.angle + 180.0f;
			}
			else
			{
				rd.m_angle = rd.m_minRect.angle + 90.0f;
			}
			rd.m_side = eUnknownSide;
			//cout << "Alignment line slope " << rd.m_slope << endl;
			if (abs(rd.m_slope) > 1.0f)
			{
				//cout << "Saving contour of size " << m_contours[i].size() << endl;
				m_rectDescr.push_back(rd);
				m_linePoints.push_back(make_pair(ld.m_point1, ld.m_point2));
				contours.push_back(m_contours[i]);
			}
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

		float longSide = max(m_rectDescr[i].m_minRect.size.width, m_rectDescr[i].m_minRect.size.height);
		//cout << "longSide " << longSide << endl;
		if (longSide > 0.0f)
		{
			Point2f vertices[4];
			float minY = FLT_MAX;
			float maxY = 0.0f;
			m_rectDescr[i].m_minRect.points(vertices);
			for (int j = 0; j < 4; j++)
			{
				line(m_drawing, vertices[j], vertices[(j + 1) % 4], c_lineColor, 2, 8);
				minY = min(vertices[j].y, minY);
				maxY = max(vertices[j].y, maxY);
				//cout << "vertex " << j << " " << vertices[j].x << "," << vertices[j].y << " to " << vertices[(j + 1) % 4].x << "," << vertices[(j + 1) % 4].y << endl;
			}

			m_lineLen = (double)longSide;	// TODO is this OK, alignment lines are 18 inches or so
			//cout << "maxY " << maxY << " yIntercept " << m_rectDescr[i].m_yIntercept << " slope " << m_rectDescr[i].m_slope;
			m_endOfLineX = fabs((maxY - m_rectDescr[i].m_yIntercept) / m_rectDescr[i].m_slope);
			m_endOfLineY = maxY;
			circle(m_drawing, Point((int)m_object_center_x, (int)m_object_center_y), 16, c_centerColor, 2);

			// Calc the other point of intersection
			m_startOfLineX = fabs((minY - m_rectDescr[i].m_yIntercept) / m_rectDescr[i].m_slope);
			m_startOfLineY = minY;
			circle(m_drawing, Point((int)m_startOfLineX, (int)m_startOfLineY), 16, c_centerColor, 2);
		}
	}

	if (m_contours.size() > 1)
	{
		cout << "More than one alignment line contour found, rejecting image" << endl;
		m_contours.clear();
	}

#ifdef DRAW_OPENCV_FIT_LINE
	for (auto& it : m_linePoints)
	{
		line(m_drawing, it.first, it.second, c_lineColor, 2, 4, 0);	// Sixth arg LINE_4 = 4 px wide line
	}
#endif
	if (c_bUseLastDiagImage || bImageCaptureTrigger)
	//if (loopCounter == c_loopCountToSaveDiagImage || c_bUseLastDiagImage || bImageCaptureTrigger)
	{
		// For manually calibrating the camera
		char fileName[255];
#ifdef BUILD_ON_WINDOWS
		int ndx = loopCounter % testFiles.size();
		sprintf_s<sizeof(fileName)>(fileName, "%s%ddrawingLine_%s.jpg", c_testOutputPath, ndx + 1, testFiles[ndx].c_str());
#else
		if (c_bUseLastDiagImage)
		{
			sprintf(fileName, "drawingLine%d.jpg", loopCounter % testFiles.size() + 1);
		}
		else
		{
			sprintf(fileName, "drawingLine%d.jpg", loopCounter);
		}
#endif
		imwrite(fileName, m_drawing);
	}
}

void ProcessingLine::CalcOutputValues()
{
	//cout << __func__ << " start" << endl;
	if (m_contours.size() == 0)
	{
		//cout << __func__ << " end no countours" << endl;
		return;
	}

	//cout << "m_lineLen " << m_lineLen << endl;
	if (m_lineLen == 0.0)
	{
		//cout << __func__ << " end no object height" << endl;
		m_OutputValues.SetQuality(eRedNoData);
		return;
	}

	double total_Distance_Inch = 0.0;							//!< [inch] Estimated total distance as the crow flies from robot to target
	double vertical_Distance_Pixel = 0.0;						//!< [pixel] Estimated distance from floor to target
	double vertical_Distance_Inch = 0.0;						//!< [inch] Estimated distance from floor to target
	double vertical_Angle_Degree = 0.0;							//!< [inch] Estimated angle in the vertical plane from robot to target
	double horizontal_Distance_Pixel = 0.0;						//!< [pixel] Estimated distance from camera to target
	double horizontal_Distance_Inch = 0.0;						//!< [inch] Estimated distance from camera to target
	double comp_Horizontal_Distance_Inch = 0.0;					//!< [inch] Estimated distance from camera to target with compensation for offset camera
	double drawingHeight = (double)m_drawing.size().height;
	if (drawingHeight == 0.0)
	{
		drawingHeight = 1.0;
	}

	double scaledHeightPixel = c_imageHeightPixel / drawingHeight;
	double standard_height_p = m_calibTargetSizePixel / scaledHeightPixel;
	double pixel_per_in = m_defaultPixelPerInch / scaledHeightPixel;

	total_Distance_Inch = ((standard_height_p / m_lineLen) * m_calibCameraDistInch);
	horizontal_Distance_Inch = (m_endOfLineX - m_im_center_x) / pixel_per_in;	// Convert horizontal pixel offset to inches @ 18 camera dist
	comp_Horizontal_Distance_Inch = horizontal_Distance_Inch + c_camera_offset_x0;	// because camera at right side of robot when facing to object 
	vertical_Distance_Pixel = m_im_center_y - m_endOfLineY;
	vertical_Distance_Inch = vertical_Distance_Pixel / pixel_per_in;				// Convert vertical pixel offset to inches @ 18 camera dist
	m_Horizontal_Angle_Degree = atan(comp_Horizontal_Distance_Inch / m_calibCameraDistInch) * m_radiansToDegrees;
	vertical_Angle_Degree = atan(vertical_Distance_Pixel / (pixel_per_in * m_calibCameraDistInch)) * m_radiansToDegrees;
	m_Actual_Distance_Inch = total_Distance_Inch * cos(vertical_Angle_Degree * m_degreesToRadians) - m_calibCameraDistInch;	// Subtract the dist from camera to front of robot

	// TOOD calc start of line dist and angle
	//double m_startOfLineX = 0.0;									//!< X coord for start of alignment line (start = closest to wall)
	//double m_startOfLineY = 0.0;									//!< Y coord for start of alignment line 

	int quality = eYellowTrackingObjects;

	// Constraint output values
	if ((abs(m_Horizontal_Angle_Degree) > m_maxAngle) ||
		(abs(vertical_Angle_Degree) > m_maxAngle) ||
		(m_Actual_Distance_Inch < 0) || (m_Actual_Distance_Inch > m_maxActualDist))
	{
		cout << "One or more calculated output values are out of bounds, setting to horz angle and dist to zero" << endl;
		cout << " m_Actual_Distance_Inch : " << m_Actual_Distance_Inch
			<< " m_Horizontal_Angle_Degree: " << m_Horizontal_Angle_Degree
			<< " vertical_Angle_Degree: " << vertical_Angle_Degree
			<< endl;
		m_Horizontal_Angle_Degree = 0.0;
		m_Actual_Distance_Inch = 0.0;
		quality = eRedNoData;
	}
	else
	{
		quality = eGreenReady;
	}

	m_OutputValues.SetDistance(m_Actual_Distance_Inch);
	m_OutputValues.SetAngle(m_Horizontal_Angle_Degree);
	m_OutputValues.SetQuality(quality);

	PrintDebugValues("Line", comp_Horizontal_Distance_Inch, vertical_Distance_Inch, m_lineLen, m_Actual_Distance_Inch, m_Horizontal_Angle_Degree);
	//cout << __func__ << " end" << endl;
}

void ProcessingLine::ProcessImage(const Mat& image)
{
	Prepare(image);
	FindContours();
	RejectSmallContours();
	//FishEyeCorrectContour(m_selectedPairIndex);
	FishEyeCorrectContours();
	FitLinesToContours();
	//FindCornerCoordinates();
	//FindBiggestContour();
	//FindVerticalRange();
	CalcOutputValues();
}
