/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
 * 
 * Class for processing hatch covers for the 2019 FRC Deep Space Challenge
*/

#include "ProcessingHatch.h"

ProcessingHatch::ProcessingHatch()
    //: ProcessingBase( { 40, 255, 255 }, { 20, 280, 50 } )     // TODO: Pick colors for Hatch game piece		if not already picked  HUE: 18-22, SATURATION 8-11, VALUE 77-82	PAINT.NET values
	: ProcessingBase({ 11, 20, /*148*/ 180 }, { 9, 14, 139 })
{
}

ProcessingHatch::~ProcessingHatch()
{
}

void ProcessingHatch::ProcessImage(const Mat& image)
{
	Prepare(image);
	FindContour();
	FindVerticalRange();
	FindCenter();
	FindBiggestContour();
	CalcObjectHeight();
	CalcOutputValues();
}


