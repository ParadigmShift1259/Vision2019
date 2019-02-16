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

//#define TEST_ANGLE_CONV
#ifdef TEST_ANGLE_CONV
void TestAngleConversion();
#endif

//#define TEST_MIN_AREA_RECT_ON_TRAPEZIOD
#ifdef TEST_MIN_AREA_RECT_ON_TRAPEZIOD
void TestMinAreaRectOnTrapezoid();
#endif

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

	state = eLoadingStation;

#ifdef TEST_ANGLE_CONV
	TestAngleConversion();
	return 0;
#endif

#ifdef TEST_MIN_AREA_RECT_ON_TRAPEZIOD
	TestMinAreaRectOnTrapezoid();
	return 0;
#endif

	cout << "Starting main loop" << endl;
	//while (true)
#ifdef TEST_GAFFER_TAPE_ALIGNMENT_IMGS
	for (int i = 0; i < 34; i++)
#else
	for (int i = 0; i < 18; i++)
#endif
	{
		camera.AcquireImage();
		switch (state)
		{
			case eLoadingStation:
			case eDeliverHatch:
			case eDeliverCargo:
				retro.Prepare(camera.GetImage());
				retro.FindContour();
				retro.RejectSmallContours();
				retro.FindCornerCoordinates();
				//retro.FindCenter();
				retro.FindBiggestContour();
				//retro.CalcCubeHeight();
				retro.CalcOutputValues();
#ifdef USE_OFFBOARD_COMMS
				offBoardComms.SetRetro(retro.GetOutputValues().GetDistance(), retro.GetOutputValues().GetAngle(), retro.GetOutputValues().GetQuality());
#endif
				retro.PrintDebugValues();

 				line.SetImageHSV(retro.GetImageHSV());
 				line.Prepare(camera.GetImage(), true);
// 				line.FindContour();
// 				line.FindCenter();
// 				line.FindBiggestContour();
// 				line.CalcCubeHeight();
// 				line.CalcOutputValues();
// #ifdef USE_OFFBOARD_COMMS
// 				offBoardComms.SetLine(line.GetOutputValues().GetDistance(), line.GetOutputValues().GetAngle(), line.GetOutputValues().GetQuality());
// #endif
// 				line.PrintDebugValues();
				break;

			case eFindCargo:
				cargo.Prepare(camera.GetImage());
				cargo.FindContour();
				cargo.FindVerticalRange();
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
				hatch.FindVerticalRange();
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

#ifdef TEST_ANGLE_CONV
void TestAngleConversion()
{
    float angle = 0;
    Mat image(200, 400, CV_8UC3, Scalar(0));
    RotatedRect originalRect;
    Point2f vertices[4];
    vector<Point2f> vertVect;
    RotatedRect calculatedRect;
	char buf[30];

    for (int angle = 0; angle < 181; angle += 30) {
        // Create a rectangle, rotating it by 10 degrees more each time.
        originalRect = RotatedRect(Point2f(100,100), Size2f(100,50), angle);

        // Convert the rectangle to a vector of points for minAreaRect to use.
        // Also move the points to the right, so that the two rectangles aren't
        // in the same place.
        originalRect.points(vertices);
        for (int i = 0; i < 4; i++) {
            vertVect.push_back(vertices[i] + Point2f(200, 0));
        }

        // Get minAreaRect to find a rectangle that encloses the points. This
        // should have the exact same orientation as our original rectangle.
        calculatedRect = minAreaRect(vertVect);

        // Draw the original rectangle, and the one given by minAreaRect.
        for (int i = 0; i < 4; i++) {
            line(image, vertices[i], vertices[(i+1)%4], Scalar(0, 255, 0));
            line(image, vertVect[i], vertVect[(i+1)%4], Scalar(255, 0, 0));
        }

		float correctedAngle;
	    if(calculatedRect.size.width < calculatedRect.size.height){
    	    correctedAngle = calculatedRect.angle + 180;
    	}else{
        	correctedAngle = calculatedRect.angle + 90;
	    }
        sprintf(buf, "angle_%d_%.2f_%.2f.jpg", angle, calculatedRect.angle, correctedAngle);
        imwrite(buf, image);

        // Print the angle values.
        printf("---\n");
        printf("Original angle:             %7.2f\n", angle);
        printf("Angle given by minAreaRect: %7.2f\n", calculatedRect.angle);
   	    printf("Angle along longer side:    %7.2f\n", correctedAngle);

        // Reset everything for the next frame.
        image = Mat(200, 400, CV_8UC3, Scalar(0));
        vertVect.clear();
    }
}
#endif

#ifdef TEST_MIN_AREA_RECT_ON_TRAPEZIOD
void TestMinAreaRectOnTrapezoid()
{
    float angle = 0;
    Mat image(960, 1280, CV_8UC3, Scalar(0));
    vector<Point2f> vertVect;
    vector<Point2f> vertVectOut;
    RotatedRect calculatedRect;

    // vertVect.push_back(Point2f(626, 281));
    // vertVect.push_back(Point2f(681, 281));
    // vertVect.push_back(Point2f(681, 626));
    // vertVect.push_back(Point2f(591, 626));

    vertVect.push_back(Point2f(311, 278));
    vertVect.push_back(Point2f(361, 281));
    vertVect.push_back(Point2f(152, 630));
    vertVect.push_back(Point2f( 62, 630));

	approxPolyDP(vertVect, vertVectOut, 5.0, true);

	// Get minAreaRect to find a rectangle that encloses the points.
//	calculatedRect = minAreaRect(vertVect);

	//Point2f vertices[4];
	//calculatedRect.points(vertices);
	for (int i = 0; i < 4; i++)
	{
		//line(image, vertices[i], vertices[(i+1)%4], Scalar(0, 255, 0));
		line(image, vertVect[i] + Point2f(200, 0), vertVect[(i+1)%4], Scalar(0, 255, 0));
		line(image, vertVectOut[i], vertVectOut[(i+1)%4], Scalar(255, 255, 255));
	}

    imwrite("trapezoid.jpg", image);
}
#endif
