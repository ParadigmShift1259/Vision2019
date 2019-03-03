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
	: ProcessingBase({ 179, 52, 255 },
					 {   0,  0, 180 })     // White gaffer tape test
{
}

ProcessingLine::~ProcessingLine()
{
}

void ProcessingLine::ProcessImage(const Mat& image)
{
	Prepare(image, true);	// We can skip HSV conversion since the retro object already did it
	//FindContours();
	//RejectSmallContours();
	//FindCornerCoordinates();
	//FindCenter();
	//FindBiggestContour();
	//CalcObjectHeight();
	//CalcOutputValues();
}
