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
		cout << "Past camera.Loop()" << endl;

		processing.Prepare(camera.GetImage());
		cout << "Past processing.Prepare()" << endl;
	    processing.FindContour();
		cout << "Past processing.FindContour()" << endl;
		processing.FindCenter();
		cout << "processing.FindCenter()" << endl;
		processing.FindBiggestContour();
		cout << "processing.FindBiggestContour()" << endl;
		processing.CalcCubeHeight();
		cout << "Calc cube height" << endl;
		double value = processing.CalcOutputValues();
		cout << "Calc output variable defined" << endl;
		//offBoardComms.Publish(value);

		processing.PrintDebugValues();
		cout << "Printing Debug values" << endl;
	}  //end of while  

	return 0;
}
