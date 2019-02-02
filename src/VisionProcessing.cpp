/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#include "CameraWrapper.h"
#include "OffBoardComms.h"
#include "Processing.h"
#include <iostream>

using namespace std;

int main()
{
	CameraWrapper camera;
	Processing processing;
	//OffBoardComms offBoardComms;

	cout << "Starting main loop" << endl;
	while (true)
	{
		camera.Loop();

		processing.Prepare(camera.GetImage());
	    processing.FindContour();
		processing.FindCenter();
		processing.FindBiggestContour();
		processing.CalcCubeHeight();
		double value = processing.CalcOutputValues();
		//offBoardComms.Publish(value);

		processing.PrintDebugValues();
		cout << "Printing Debug values" << endl;
	}  //end of while  

	return 0;
}
