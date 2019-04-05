/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
 * 
 * Class for processing retro reflective tape for the 2019 FRC Deep Space Challenge
*/

#include "ProcessingRetro.h"

ProcessingRetro::ProcessingRetro()
    //: ProcessingBase(   { 90, 255, 255 }, 
    //                    { 65,  90,  90 } )     // Colors for Retro reflective tape with green lighting (FIRST sample images)

	: ProcessingBase({ 90, 255, 255 },
					 { 50, 153, 100 })		// Colors for Retro reflective tape with green lighting (Team 1259 2019 Mar 2)
	//				 { 40, 153, 100 })		// Colors for Retro reflective tape with green lighting (Team 1259 2019 Feb 16)

	//: ProcessingBase({ 90, 255, 255 },
	//				 { 45, 102, 153 })		// Colors for Retro reflective tape with green lighting (Team 1259 2019 Feb 16)

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// St. Louis competition settings
	//: ProcessingBase({ 81, 255, 255 },
	//				 { 33, 77, 153 })		// Colors for Retro reflective tape with green lighting (Team 1259 2019 Mar 14)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//: ProcessingBase({ 90, 255, 255 },
	//				 { 85,  90,  90 })		// Colors for Retro reflective tape with 2016 discrete LED green lighting (Team 1259 2019 Mar 6)
{
}

ProcessingRetro::~ProcessingRetro()
{

}

void ProcessingRetro::ProcessImage(const Mat& image)
{
	Prepare(image);
	FindContours();
	RejectSmallContours();
	FindCornerCoordinates();
	CalcOutputValues("Retro");

#ifdef CALC_LEFT_RIGHT_TARGETS
	double actualDistLeft = 0.0;
	double horzAngle;
	float longSide = max(m_leftTarget.m_minRect.size.width, m_leftTarget.m_minRect.size.height);

	//cout << "calling CalcOutputValues LeftRetro"
	//	<< " longside " << longSide
	//	<< " left x " << m_leftTarget.m_minRect.center.x
	//	<< " left y " << m_leftTarget.m_minRect.center.y
	//	<< endl;
	EQuality quality = CalcOutputValues("LeftRetro", longSide, m_leftTarget.m_minRect.center.x, m_leftTarget.m_minRect.center.y, actualDistLeft, horzAngle);
	//m_OutputValuesLeftTarget.SetDistance(actualDistLeft);
	//m_OutputValuesLeftTarget.SetAngle(horzAngle);
	//m_OutputValuesLeftTarget.SetQuality(quality);

	longSide = max(m_rightTarget.m_minRect.size.width, m_rightTarget.m_minRect.size.height);
	//cout << "calling CalcOutputValues RightRetro"
	//	<< " longside " << longSide
	//	<< " right x " << m_rightTarget.m_minRect.center.x
	//	<< " right y " << m_rightTarget.m_minRect.center.y
	//	<< endl;
	double actualDistRight = 0.0;
	quality = CalcOutputValues("RightRetro", longSide, m_rightTarget.m_minRect.center.x, m_rightTarget.m_minRect.center.y, actualDistRight, horzAngle);
	//m_OutputValuesRightTarget.SetDistance(actualDistRight);
	//m_OutputValuesRightTarget.SetAngle(horzAngle);
	//m_OutputValuesRightTarget.SetQuality(quality);

	double leftRightdiff = actualDistLeft - actualDistRight;
	string direction = "Unkown";
	if (m_Actual_Distance_Inch < 30)
	{
		m_approachDirection = eUnknownDirection;
		direction = "Unkown";
	}
	else
	{
		if (abs(leftRightdiff) < 2.0)
		{
			m_approachDirection = eApproachFromCenter;
			direction = "Center";
		}
		else if (leftRightdiff < 0.0)
		{
			m_approachDirection = eApproachFromLeft;
			direction = "Left  ";
		}
		else //if (leftRightdiff > 0.0)
		{
			m_approachDirection = eApproachFromRight;
			direction = "Right ";
		}
	}

	printf("Loop %06d Retro ActualDist %8.3f HorzAngle %8.3f ApproachDir %s L/R %8.3f "	// NOTE do not add \n, main loop will do it
		, loopCounter
		, m_Actual_Distance_Inch
		, m_Horizontal_Angle_Degree
		, direction.c_str()
		, leftRightdiff
	);
#else
	printf("Loop %06d Retro ActualDist %8.3f HorzAngle %8.3f "	// NOTE do not add \n, main loop will do it
		, loopCounter
		, m_Actual_Distance_Inch
		, m_Horizontal_Angle_Degree
	);
#endif	// CALC_LEFT_RIGHT_TARGETS
}
