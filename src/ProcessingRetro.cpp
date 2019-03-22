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
					 { 40, 153, 100 })		// Colors for Retro reflective tape with green lighting (Team 1259 2019 Feb 16)

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
	//FitLinesToContours();
	//SelectCenterRetroPairs();
	//FishEyeCorrectContour(m_selectedPairIndex);
	//FishEyeCorrectContours();
	FindCornerCoordinates();
	//FindBiggestContour();
	//FindVerticalRange();
	CalcOutputValues("Retro");

	double actualDist;
	double horzAngle;
	float longSide = max(m_leftTarget.m_minRect.size.width, m_leftTarget.m_minRect.size.height);

	cout << "calling CalcOutputValues LeftRetro"
		<< " longside " << longSide
		<< " left x " << m_leftTarget.m_minRect.center.x
		<< " left y " << m_leftTarget.m_minRect.center.y
		<< endl;
	EQuality quality = CalcOutputValues("LeftRetro", longSide, m_leftTarget.m_minRect.center.x, m_leftTarget.m_minRect.center.y, actualDist, horzAngle);
	m_OutputValuesLeftTarget.SetDistance(actualDist);
	m_OutputValuesLeftTarget.SetAngle(horzAngle);
	m_OutputValuesLeftTarget.SetQuality(quality);

	longSide = max(m_rightTarget.m_minRect.size.width, m_rightTarget.m_minRect.size.height);
	cout << "calling CalcOutputValues RightRetro"
		<< " longside " << longSide
		<< " right x " << m_rightTarget.m_minRect.center.x
		<< " right y " << m_rightTarget.m_minRect.center.y
		<< endl;
	quality = CalcOutputValues("RightRetro", longSide, m_rightTarget.m_minRect.center.x, m_rightTarget.m_minRect.center.y, actualDist, horzAngle);
	m_OutputValuesRightTarget.SetDistance(actualDist);
	m_OutputValuesRightTarget.SetAngle(horzAngle);
	m_OutputValuesRightTarget.SetQuality(quality);
}
