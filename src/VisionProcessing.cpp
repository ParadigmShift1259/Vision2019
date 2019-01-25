//#define FISHEYE_CORR_FLAG_TEST 	// define for distortion correction  

#define PI 3.1415926535897932384626433832795

#include "CameraWrapper.h"
#include "OffBoardComms.h"
#include "Processing.h"


#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdint>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <cmath>

using namespace std;





int cube_contour_max_index = 0;
int cube_contour_min_index = 0;
double cube_contour_max_y, cube_contour_min_y, im_actual_dist;
double im_center_x, im_center_y;

int main()
{
	CameraWrapper camera;
	Processing processing;
	OffBoardComms offBoardComms;

	cout<<"Starting main loop"<<endl;
	while (true)
	{
		camera.Loop();

		processing.Prepare(camera.GetImage());
	
		// Finding Contour
	

		// Finding the moment of the biggest contour
		Moments m = moments(contours[biggestContourLocation], true);

		// Find cube center coordinate and draw a circle at that point
		cube_center_x = m.m10/m.m00;
		cube_center_y = m.m01/m.m00;
		circle(drawing, Point(cube_center_x,cube_center_y), 16, Scalar(255,105,180), 2);

		// Find image center coordinate
		im_center_x = drawing.size().width/2;
		im_center_y = drawing.size().height/2;
		circle(drawing, Point(im_center_x,im_center_y), 1, Scalar(255,105,180));
//		drawMarker(drawing, Point(im_center_x, im_center_y), Scalar(255,255,255));

		// Find cube height per center coordinate of the cube and contour info
		cube_contour_max_y = 10000;   // assign a big value
		cube_contour_min_y = 10000;   // assign a big value

		for (int i = 0; i < contours[biggestContourLocation].size(); i++)
		{
			im_actual_dist = abs(cube_center_x - contours[biggestContourLocation][i].x);
			if (cube_contour_max_y > im_actual_dist)
			{
				if (contours[biggestContourLocation][i].y > cube_center_y) // Find coordinate > object center
				{
					cube_contour_max_index = i;
					cube_contour_max_y = im_actual_dist;
				}
			}

			if (cube_contour_min_y > im_actual_dist)
			{
				if (contours[biggestContourLocation][i].y <= cube_center_y) // Find coordinate <= object center
				{
					cube_contour_min_index = i;
					cube_contour_min_y = im_actual_dist;
				}
			}
		}

		// Apply distortion correction for fisheye camera to three sets of coordinates only to speed up calculation
#ifdef FISHEYE_CORR_FLAG_TEST
		if (FISHEYE_CORR_FLAG == 1)
		{
			double x_original[3];
			double y_original[3];
			double x_corrected[3];
			double y_corrected[3];
			double xt[3];
			double yt[3];
			double ut[3];
			double vt[3];
			double r;
			double R;
			double theta;
			double s;
			double s2;
			double border_corr;
			const double k = -0.46;     		// a constant value for fisheye lens used by Raspberry pi
			
			x_original[0] = cube_center_x;
			x_original[1] = cube_center_x;
			x_original[2] = cube_center_x;
		
			y_original[0] = contours[biggestContourLocation][cube_contour_min_index].y;
			y_original[1] = cube_center_y;
			y_original[2] = contours[biggestContourLocation][cube_contour_max_index].y;

			R = sqrt(im_center_x*im_center_x + im_center_y*im_center_y);
			border_corr = 1/(1 + k*pow( min(im_center_x, im_center_y)/R, 2.0) ); // Scaling factor per border
	
			for (int i = 0; i < 3; i++)
			{
				xt[i] = x_original[i] - im_center_x;
				yt[i] = y_original[i] - im_center_y; 
		
				r = sqrt(xt[i]*xt[i] + yt[i]*yt[i]);	// Find radius

				theta = atan(yt[i]/xt[i])*180/PI; 	// Find theta for the angle

				if 	((yt[i] > 0) && (xt[i] >= 0))
				{	theta = theta;     	}
				else if ((yt[i] < 0) && (xt[i] >= 0))
				{  	theta = 360 + theta;	}
				else if ((yt[i] >= 0) && (xt[i] <= 0))
				{	theta = 180 + theta;	}
				else if ((yt[i] < 0) && (xt[i] <= 0))
				{	theta = 180 + theta;	}
			
				r = r/R;				// Normalize the polar coordinate r
		
				s = r*(1+k*r);				// Apply r-based transform with k

				s2 = s*R;				// Un-normalize s

				s2 = s2*border_corr;			// Scale radius 

				ut[i] = s2*cos(theta*PI/180);		// Converted back to cartesian coordinates
				vt[i] = s2*sin(theta*PI/180);

				x_corrected[i] = ut[i] + im_center_x;	// Add image center back 
				y_corrected[i] = vt[i] + im_center_y;
			}

        	cube_height = abs(y_corrected[2] -  y_corrected[0]);
			cube_center_x = x_corrected[1];
			cube_center_y = y_corrected[1];
		}
		else
#endif
		{
			cube_height = 	abs(contours[biggestContourLocation][cube_contour_max_index].y -  
								contours[biggestContourLocation][cube_contour_min_index].y);
		}

		standard_height_p = DEFAULT_HEIGHT_PIXEL/(DEFAULT_FOV_ROW_NUM/drawing.size().height);
		pixel_per_in = DEFAULT_PIXEL_PER_INCH/(DEFAULT_FOV_ROW_NUM/drawing.size().height);

		Total_Distance_Inch = ((standard_height_p/cube_height)*CAL_DISTANCE_INCH);
		Horizontal_Distance_Pixel = cube_center_x - im_center_x;
		Vertical_Distance_Pixel = im_center_y - cube_center_y;	// Switch due to (0,0) at the top left to convert to bottom left
		Horizontal_Angle_Degree = atan(Horizontal_Distance_Pixel/(pixel_per_in*CAL_DISTANCE_INCH))*180/PI;
		Vertical_Angle_Degree = atan(Vertical_Distance_Pixel/(pixel_per_in*CAL_DISTANCE_INCH))*180/PI;

		// Output values given to Network Table
		Horizontal_Distance_Inch = Total_Distance_Inch * sin(Horizontal_Angle_Degree*PI/180);
		Vertical_Distance_Inch = Total_Distance_Inch * sin(Vertical_Angle_Degree*PI/180);
		Forward_Distance_Inch = Total_Distance_Inch*cos(Vertical_Angle_Degree*PI/180)*cos(Horizontal_Angle_Degree*PI/180);

		// Add constraints for cube distance and angle
		if ((abs(Horizontal_Angle_Degree ) > ANGLE_THRESHOLD ) ||
			(abs(Vertical_Angle_Degree) > ANGLE_THRESHOLD ) ||
			(Forward_Distance_Inch < 0) || (Forward_Distance_Inch > FORWARD_DIST_THRESHOLD ) ||
			(contours[biggestContourLocation].size() < CUBE_CONTOUR_THRESHOLD ))
	   	{
			Horizontal_Distance_Inch = 0;
			Vertical_Distance_Inch=0;
			Forward_Distance_Inch =0;
	   	}

#ifdef USE_NETWORK_TABLES
		netTable->PutNumber("visioncounter", counter);
		netTable->PutNumber("XOffAngle", Horizontal_Angle_Degree);
		//netTable->GetEntry("visioncounter").ForceSetDouble(counter);
#endif

		// For manually calibrating the camera
		// TODO Wrap this in a runtime flag
		imwrite("inrange.bmp",inrange);
		imwrite("drawing.bmp", drawing);
		imwrite("image.bmp", image);

		//cout<<"x: "<<m.m10<<" y: "<<m.m01<<" divided by: "<<m.m00<<endl;
		//cout<<"center of cube x is at: "<<m.m10/m.m00<<" center of cube y is at: "<<m.m01/m.m00<<endl;
		//cout<<"cube_contour_max_index is: "<<cube_contour_max_index<<" cube_contour_min_index is: "<<cube_contour_min_index<<endl;
		//cout<<"Height: "<<cube_height<<endl;
		//cout<<"Horizontal_Angle_Degree: "<<Horizontal_Angle_Degree<<" Vertical_Angle_Degree "<<Vertical_Angle_Degree<<" Total_Distance_Inch : "<<Total_Distance_Inch<<endl;

		cout 	<< "Counter: " << counter 
				<< " Horizontal_Distance_Inch: " << Horizontal_Distance_Inch
				<< " Vertical_Distance_Inch " << Vertical_Distance_Inch
				<< " Forward_Distance_Inch: " << Forward_Distance_Inch 
				<< " Horizontal_Angle " << Horizontal_Angle_Degree 
				<< endl;

	}  //end of while  

	Camera.release();
	return 0;

}
