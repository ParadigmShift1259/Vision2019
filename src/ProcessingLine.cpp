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
	: ProcessingBase({  60, 175, 175 },
					 {  40,  45, 100 })     // White gaffer tape test
{
}

ProcessingLine::~ProcessingLine()
{
}

void ProcessingLine::RejectSmallContours()
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

	m_linePoints.clear();
	m_rectDescr.clear();
	vector<vector<Point>> contours;
	size_t threshold = max(c_minContourPoints, (size_t)c_smallContourPercentOfMax * maxSize);
	cout << "Max contour size " << maxSize << " threshold " << threshold << endl;
	for (size_t i = 0; i < m_contours.size(); i++)
	{
		RectDescr rd;
		rd.m_minRect = minAreaRect(m_contours[i]);

		int minX = 0;
		int minY = 0;
		int maxX = c_imageWidthPixel;
		int maxY = c_imageHeightPixel;
		if (m_leftTarget.m_side != eUnknownSide && m_rightTarget.m_side != eUnknownSide)
		{
			minX = (int)m_leftTarget.m_minRect.center.x - 20;
			maxX = (int)m_rightTarget.m_minRect.center.x + 20;
			minY = (int)m_leftTarget.m_minRect.center.y;
			maxY = (int)m_rightTarget.m_minRect.center.y + 450;

			line(m_drawing, Point(minX, minY), Point(minX, maxY), c_contourColor, 2, 8);
			line(m_drawing, Point(minX, maxY), Point(maxX, maxY), c_contourColor, 2, 8);
			line(m_drawing, Point(maxX, maxY), Point(maxX, minY), c_contourColor, 2, 8);
			line(m_drawing, Point(maxX, minY), Point(minX, minY), c_contourColor, 2, 8);
		}

		auto x = m_contours[i].at(0).x;
		auto y = m_contours[i].at(0).y;
		if (m_contours[i].size() > threshold && rd.m_minRect.size.area() > 0.0f && y >= minY && y <= maxY && x >= minX && x <= maxX)
		{
			//auto arcLen = arcLength(m_contours[i], true);
			//auto unorientedArea = contourArea(m_contours[i], false);
			//cout << "arcLen " << arcLen << " unorientedArea " << unorientedArea << endl;
			//if (arcLen > c_arcLenThreshold && unorientedArea > c_areaThreshold)
			{
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
				if (abs(rd.m_slope) > 1.0f)
				{
					cout << "Saving contour of size " << m_contours[i].size() << endl;
					m_rectDescr.push_back(rd);
					m_linePoints.push_back(make_pair(ld.m_point1, ld.m_point2));
					contours.push_back(m_contours[i]);
				}
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
			//char buf[500];
			//sprintf_s<sizeof(buf)>(buf, "Contour index %zu  x     y", i);
			//cout << buf << endl;
			//for (auto& pt : m_contours[i])
			//{
			//	//sprintf_s<sizeof(buf)>(buf, "Contour index %d  x     y", i);
			//	sprintf_s<sizeof(buf)>(buf,   "                  %4d   %4d", pt.x, pt.y);
			//	cout << buf << endl;
			//}
		}
	}

#ifdef PI_TIMING
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
			FishEyeCorrectPoint(vIn[j].x, vIn[j].y, xCorrected, yCorrected);
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

#ifdef DRAW_OPENCV_FIT_LINE
	for (auto& it : m_linePoints)
	{
		line(m_drawing, it.first, it.second, c_lineColor, 2, 4, 0);	// Sixth arg LINE_4 = 4 px wide line
	}
#endif
	//if (loopCounter == c_loopCountToSaveDiagImage || c_bUseLastDiagImage)
	{
		// For manually calibrating the camera
		char fileName[255];
#ifdef BUILD_ON_WINDOWS
		int ndx = loopCounter % testFiles.size();
		sprintf_s<sizeof(fileName)>(fileName, "%s%ddrawing_%s", c_testOutputPath, ndx + 1, testFiles[ndx].c_str());
#else
		if (c_bUseLastDiagImage)
		{
			sprintf(fileName, "drawing%d.jpg", loopCounter % testFiles.size() + 1);
		}
		else
		{
			sprintf(fileName, "drawing.jpg");
		}
#endif
		imwrite(fileName, m_drawing);
	}
}

void ProcessingLine::ProcessImage(const Mat& image)
{
	Prepare(image);
	FindContours();
	RejectSmallContours();
#ifndef TEST_FISHEYE_CORRECTION_BY_LUT
	//FishEyeCorrectContour(m_selectedPairIndex);
	FishEyeCorrectContours();
#endif
	FitLinesToContours();
	//FindCornerCoordinates();
	//FindBiggestContour();
	//FindVerticalRange();
	CalcOutputValues();

}
