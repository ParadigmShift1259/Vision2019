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

#ifndef BUILD_ON_WINDOWS
#define PI_TIMING_MAIN_LOOP
#endif

#ifdef PI_TIMING_MAIN_LOOP
#include <time.h>	// For clock_gettime()
#endif

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

	EVisionTarget state = offBoardComms.GetState();

#ifdef TEST_ANGLE_CONV
	TestAngleConversion();
	return 0;
#endif

#ifdef TEST_MIN_AREA_RECT_ON_TRAPEZIOD
	TestMinAreaRectOnTrapezoid();
	return 0;
#endif

	cout << "Starting main loop" << endl;
#ifndef BUILD_ON_WINDOWS
	while (true)
#else
	for (int i = 0; i < testFiles.size(); i++)
#endif
	{
#ifdef PI_TIMING_MAIN_LOOP
		long int start_time;
		long int time_difference;
		struct timespec gettime_now;

		clock_gettime(CLOCK_REALTIME, &gettime_now);
		start_time = gettime_now.tv_nsec;		//Get nS value
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
		cout << "Loop time " << millisec << " milliseconds" << endl;
#endif
	}  //end of while

	cout << "Exiting" << endl;

	return 0;
}
