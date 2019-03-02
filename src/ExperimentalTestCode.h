/**
 * Developed by
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

//using namespace std;

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
        originalRect = RotatedRect(Point2f(100.0f, 100.0f), Size2f(100.0f, 50.0f), (float)angle);

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
#ifdef BUILD_ON_WINDOWS
		sprintf_s<sizeof(buf)>(buf, "angle_%d_%.2f_%.2f.jpg", angle, calculatedRect.angle, correctedAngle);
#else
		sprintf(buf, "angle_%d_%.2f_%.2f.jpg", angle, calculatedRect.angle, correctedAngle);
#endif
        imwrite(buf, image);

        // Print the angle values.
        printf("---\n");
        printf("Original angle:             %7.2f\n", (float)angle);
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
    Mat image(c_imageHeightPixel, c_imageWidthPixel, CV_8UC3, Scalar(0));
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
