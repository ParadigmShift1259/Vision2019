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

#ifdef USE_OFFBOARD_COMMS
	OffBoardComms offBoardComms;
#endif

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
				retro.CalcOutputValues();
#ifdef USE_OFFBOARD_COMMS
				offBoardComms.SetRetro(retro.GetOutputValues().GetDistance(), retro.GetOutputValues().GetAngle(), retro.GetOutputValues().GetQuality());
#endif
				retro.PrintDebugValues();

				line.SetImageHSV(retro.GetImageHSV());
				line.Prepare(camera.GetImage(), true);
				line.FindContour();
				line.FindCenter();
				line.FindBiggestContour();
				line.CalcCubeHeight();
				line.CalcOutputValues();
#ifdef USE_OFFBOARD_COMMS
				offBoardComms.SetLine(line.GetOutputValues().GetDistance(), line.GetOutputValues().GetAngle(), line.GetOutputValues().GetQuality());
#endif
				line.PrintDebugValues();				
				break;	
			
			case eFindCargo:
				cargo.Prepare(camera.GetImage());
				cargo.FindContour();
				cargo.FindCenter();
				cargo.FindBiggestContour();
				cargo.CalcCubeHeight();
				cargo.CalcOutputValues();
#ifdef USE_OFFBOARD_COMMS
				offBoardComms.SetCargo(cargo.GetOutputValues().GetDistance(), cargo.GetOutputValues().GetAngle(), cargo.GetOutputValues().GetQuality());
#endif
				cargo.PrintDebugValues();
				break;

			case eFindHatch:
				hatch.Prepare(camera.GetImage());
				hatch.FindContour();
				hatch.FindCenter();
				hatch.FindBiggestContour();
				hatch.CalcCubeHeight();
				hatch.CalcOutputValues();
#ifdef USE_OFFBOARD_COMMS
				offBoardComms.SetHatch(hatch.GetOutputValues().GetDistance(), hatch.GetOutputValues().GetAngle(), hatch.GetOutputValues().GetQuality());
#endif
				hatch.PrintDebugValues();
				break;

			default: 
				break;
		}

#ifdef USE_OFFBOARD_COMMS
		offBoardComms.Publish();
#endif

	}  //end of while  

	return 0;
}
