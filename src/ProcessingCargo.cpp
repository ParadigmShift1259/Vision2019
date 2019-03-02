/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
 * 
 * Class for processing Game ball for the 2019 FRC Deep Space Challenge
*/

#include "ProcessingCargo.h"

ProcessingCargo::ProcessingCargo()
    : ProcessingBase( { 16, 255, 255 }, { 1, 50, 30 } )
{
}

ProcessingCargo::~ProcessingCargo()
{
}

void ProcessingCargo::ProcessImage(const Mat& image)
{
	Prepare(image);
	FindContour();
	FindVerticalRange();
	FindCenter();
	FindBiggestContour();
	CalcObjectHeight();
	CalcOutputValues();
}
