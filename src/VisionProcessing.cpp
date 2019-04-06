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

//#ifndef BUILD_ON_WINDOWS
//#define PI_TIMING_MAIN_LOOP
//#endif
//
//#ifdef PI_TIMING_MAIN_LOOP
//#include <time.h>	// For clock_gettime()
//#endif

//#define CATCH_FLOATING_POINT_EXCEPTIONS
#ifdef CATCH_FLOATING_POINT_EXCEPTIONS
#include <signal.h>
#include <math.h>
#include <errno.h>
#include <float.h>
#include <fenv.h>
#endif

//#define TEST_ANGLE_CONV
//#define TEST_MIN_AREA_RECT_ON_TRAPEZIOD
#if defined(TEST_ANGLE_CONV) || defined(TEST_MIN_AREA_RECT_ON_TRAPEZIOD)
#include "ExperimentalTestCode.h"
#endif

using namespace std;

#ifdef CATCH_FLOATING_POINT_EXCEPTIONS
void overflow_handler(int signal_number)
{
	if (feclearexcept(FE_OVERFLOW | FE_UNDERFLOW | FE_DIVBYZERO | FE_INVALID))
	{
		cout << "Nothing Cleared" << endl;
	}
	else
	{
		cout << "All Cleared" << endl;
	}
}
#endif

int loopCounter = 0;
bool bImageCaptureTrigger = false;

int main()
{
	cout << "Vision has Started" << endl;

#ifdef CATCH_FLOATING_POINT_EXCEPTIONS
	//===Enable Exceptions===//
	int failure = feenableexcept(FE_OVERFLOW | FE_UNDERFLOW | FE_DIVBYZERO | FE_INVALID);
	if (failure)
	{
		cout << "FE ENABLE EXCEPTIONS FAILED" << endl;
	}

	//===Create Error Handler===//
	signal(SIGFPE, overflow_handler);
#endif

	CameraWrapper camera;
	ProcessingCargo cargo;
	ProcessingHatch hatch;
	ProcessingLine line;
	ProcessingRetro retro;
	OffBoardComms offBoardComms;

	EVisionTarget state = eLoadingStation;	// offBoardComms.GetState();

#ifdef TEST_ANGLE_CONV
	TestAngleConversion();
	return 0;
#endif

#ifdef TEST_MIN_AREA_RECT_ON_TRAPEZIOD
	TestMinAreaRectOnTrapezoid();
	return 0;
#endif

	cout << "Starting main loop" << endl;
//namedWindow("TestWindow");
	//offBoardComms.Connect();
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
		//waitKey(0);
#ifdef PI_TIMING_MAIN_LOOP
		long int start_time;
		long int time_difference;
		struct timespec gettime_now;

		clock_gettime(CLOCK_REALTIME, &gettime_now);
		start_time = gettime_now.tv_nsec;		//Get nS value
#endif
		//startTimePoint = chrono::system_clock::now();

#ifdef CAPTURE_EVERY_NTH_IMAGE
		bImageCaptureTrigger = false;
		if (loopCounter % c_SaveEveryNthImage == 0)
		{
			bImageCaptureTrigger = true;
		}
#endif
		//cout << "Acquiring image" << endl;
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

#ifdef PI_TIMING_MAIN_LOOP
		clock_gettime(CLOCK_REALTIME, &gettime_now);
		time_difference = gettime_now.tv_nsec - start_time;
		if (time_difference < 0)
			time_difference += 1000000000;				//(Rolls over every 1 second)
		double millisec = time_difference / 1000000.0;
		printf("Loop time %4.3f ms\n", millisec);
#endif

		//now = chrono::system_clock::now();
		//chrono::milliseconds elapsed = chrono::duration_cast<chrono::milliseconds>(now - startTimePoint);
#ifdef BUILD_ON_WINDOWS
		//printf("Loop time %4I64d ms\n", elapsed.count());
#else
//		printf("Loop time %4d ms\n", elapsed.count());
#endif

// Temp
//printf("\n");
}
	}  //end of while

	cout << "Exiting" << endl;

	return 0;
}
