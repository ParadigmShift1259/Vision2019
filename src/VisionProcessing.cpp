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

//#define TEST_ANGLE_CONV
//#define TEST_MIN_AREA_RECT_ON_TRAPEZIOD
#if defined(TEST_ANGLE_CONV) || defined(TEST_MIN_AREA_RECT_ON_TRAPEZIOD)
#include "ExperimentalTestCode.h"
#endif

using namespace std;

int main()
{
	CameraWrapper camera;
	ProcessingCargo cargo;
	ProcessingHatch hatch;
	ProcessingLine line;
	ProcessingRetro retro;
	OffBoardComms offBoardComms;

	EVisionTarget state = eUnknownState;

#ifdef TEST_ANGLE_CONV
	TestAngleConversion();
	return 0;
#endif

#ifdef TEST_MIN_AREA_RECT_ON_TRAPEZIOD
	TestMinAreaRectOnTrapezoid();
	return 0;
#endif

	cout << "Starting main loop" << endl;
	// TODO while (true)
#ifdef TEST_GAFFER_TAPE_ALIGNMENT_IMGS
	for (int i = 0; i < 34; i++)
#else
	for (int i = 0; i < 18; i++)
#endif
	{
		camera.AcquireImage();
		
		state = offBoardComms.GetState();
		switch (state)
		{
			case eLoadingStation:
			case eDeliverHatch:
			case eDeliverCargo:
				retro.ProcessImage(camera.GetImage());
				offBoardComms.SetRetro(retro);

 				line.SetImageHSV(retro.GetImageHSV());	// Re-use the convtered HSV image
 				line.ProcessImage(camera.GetImage());
				offBoardComms.SetLine(line);
				break;

			case eFindCargo:
				cargo.ProcessImage(camera.GetImage());
				offBoardComms.SetCargo(cargo);
				break;

			case eFindHatch:
				hatch.ProcessImage(camera.GetImage());
				offBoardComms.SetHatch(hatch);
				break;

			default:
				break;
		}

		offBoardComms.Publish();

	}  //end of while

	return 0;
}
