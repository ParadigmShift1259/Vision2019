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

int loopCounter = 0;
bool bImageCaptureTrigger = false;

int main()
{
	cout << "Vision has Started" << endl;

	OffBoardComms offBoardComms;
	CameraWrapper camera;
	ProcessingCargo cargo;
	ProcessingHatch hatch;
	ProcessingLine line;
	ProcessingRetro retro;

	EVisionTarget state = eLoadingStation;	// offBoardComms.GetState();

	cout << "Starting main loop" << endl;
	//namedWindow("TestWindow");
	chrono::system_clock::time_point startTimePoint;
	chrono::system_clock::time_point now;
#ifndef BUILD_ON_WINDOWS
	while (true)
#else
	for (int i = 0; i < testFiles.size(); i++)
#endif
	{
		{
			ScopedTimer t(" ");
			//startTimePoint = chrono::system_clock::now();

			if (!offBoardComms.IsConnected())
			{
				offBoardComms.Connect();
			}
			//waitKey(0);

#ifdef CAPTURE_EVERY_NTH_IMAGE
			bImageCaptureTrigger = false;
			if (loopCounter % c_SaveEveryNthImage == 0)
			{
				bImageCaptureTrigger = true;
			}
#endif
			camera.AcquireImage();
		
			state = offBoardComms.GetState();
			switch (state)
			{
				case eLoadingStation:
				case eDeliverHatch:
				case eDeliverCargo:
					retro.ProcessImage(camera.GetImage());
					offBoardComms.SetRetro(retro);
					//offBoardComms.SetApproachDirection(retro.GetApproachDirection());
					//offBoardComms.SetLeftTargetValues(retro);
					//offBoardComms.SetRightTargetValues(retro);

					//line.SetRetroCoords(retro.GetLeftTarget(), retro.GetRightTarget());
					//line.ProcessImage(camera.GetImage());
					//offBoardComms.SetLine(line);
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
					cout << "Unknown state" << endl;
					break;
			}

			offBoardComms.Publish();

			loopCounter++;

			//now = chrono::system_clock::now();
			//chrono::milliseconds elapsed = chrono::duration_cast<chrono::milliseconds>(now - startTimePoint);
#ifdef BUILD_ON_WINDOWS
			//printf("Loop time %4I64d ms\n", elapsed.count());
#else
//			printf("Loop time %4d ms\n", elapsed.count());
#endif

		// Temp
		//printf("\n");
		}
	}  //end of while

	cout << "Exiting" << endl;

	return 0;
}
