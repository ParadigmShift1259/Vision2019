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
#ifdef TEST_GAFFER_TAPE_ALIGNMENT_IMGS
	: ProcessingBase({ 179, 52, 255 },
			 		 { 0,  0, 180 })     // White gaffer tape test
#else
    : ProcessingBase(   { 90, 255, 255 }, 
                        { 65,  90,  90 } )     // Colors for Retro reflective tape
#endif
{
}

ProcessingRetro::~ProcessingRetro()
{
}

void ProcessingRetro::ProcessImage(const Mat& image)
{
	Prepare(image);
	FindContour();
	RejectSmallContours();
	FindCornerCoordinates();
	//FindCenter();
	FindBiggestContour();
	//CalcCubeHeight();
	CalcOutputValues();
	PrintDebugValues();
}
