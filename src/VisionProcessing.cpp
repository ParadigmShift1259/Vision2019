/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#include "CameraWrapper.h"
#include "OffBoardComms.h"
#include "ProcessingCargo.h"
#include "ProcessingLine.h"
#include "ProcessingHatch.h"
#include "ProcessingRetro.h"
#include <iostream>

using namespace std;

int main()
{
	CameraWrapper camera;
	ProcessingCargo cargo;
	ProcessingHatch hatch;
	ProcessingLine line;
	ProcessingRetro retro;

	enum EVisionTarget
	{
		eUnknown,
		eLoadingStation,
		eDeliverHatch,
		eDeliverCargo,
		eFindCargo,
		eFindHatch
	} state = eUnknown;

	state = eFindCargo;

	//OffBoardComms offBoardComms;
	double value = 0.0;

	cout << "Starting main loop" << endl;
	while (true)
	{
		camera.AcquireImage();
		switch (state)
		{
			case eLoadingStation:
			case eDeliverHatch:
			case eDeliverCargo:
				retro.Prepare(camera.GetImage());
				retro.FindContour();
				retro.FindCenter();
				retro.FindBiggestContour();
				retro.CalcCubeHeight();
				value = retro.CalcOutputValues();
				retro.PrintDebugValues();

				line.SetImageHSV(retro.GetImageHSV());
				line.Prepare(camera.GetImage(), true);
				line.FindContour();
				line.FindCenter();
				line.FindBiggestContour();
				line.CalcCubeHeight();
				value = line.CalcOutputValues();
				line.PrintDebugValues();				
				break;	
			
			case eFindCargo:
				cargo.Prepare(camera.GetImage());
				cargo.FindContour();
				cargo.FindCenter();
				cargo.FindBiggestContour();
				cargo.CalcCubeHeight();
				value = cargo.CalcOutputValues();
				cargo.PrintDebugValues();
				break;

			case eFindHatch:
				hatch.Prepare(camera.GetImage());
				hatch.FindContour();
				hatch.FindCenter();
				hatch.FindBiggestContour();
				hatch.CalcCubeHeight();
				value = hatch.CalcOutputValues();
				hatch.PrintDebugValues();
				break;

			default: 
				break;
		}

		//offBoardComms.Publish(value);
	}  //end of while  

	return 0;
}
