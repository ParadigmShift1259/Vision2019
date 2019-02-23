#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat image0, image1;

int main()
{
	VideoCapture m_Camera1 = VideoCapture(0);
	VideoCapture m_Camera2 = VideoCapture(1);
	
	m_Camera1.set(CV_CAP_PROP_EXPOSURE, 200.0);			// Default 0.09991
	//m_Camera.set(CV_CAP_PROP_BRIGHTNESS, 0.0);
	m_Camera1.set(CV_CAP_PROP_CONTRAST, 0.5);			// Default 0.5
	m_Camera1.set(CV_CAP_PROP_SATURATION, 0.5);			// Default 0.5
	//m_Camera.set(CV_CAP_PROP_HUE, 0.0);
	m_Camera1.set(CV_CAP_PROP_WHITE_BALANCE_BLUE_U, 1000.0);	// Default 1000
	m_Camera1.set(CV_CAP_PROP_WHITE_BALANCE_RED_V, 1000.0);		// Default 1000
	
	m_Camera2.set(CV_CAP_PROP_EXPOSURE, 200.0);			// Default 0.09991
	//m_Camera.set(CV_CAP_PROP_BRIGHTNESS, 0.0);
	m_Camera2.set(CV_CAP_PROP_CONTRAST, 0.5);			// Default 0.5
	m_Camera2.set(CV_CAP_PROP_SATURATION, 0.5);			// Default 0.5
	//m_Camera.set(CV_CAP_PROP_HUE, 0.0);
	m_Camera2.set(CV_CAP_PROP_WHITE_BALANCE_BLUE_U, 1000.0);	// Default 1000
	m_Camera2.set(CV_CAP_PROP_WHITE_BALANCE_RED_V, 1000.0);		// Default 1000
	
	for (int loop = 0; loop < 60 ; loop++)
	{
		m_Camera1.grab();
		m_Camera2.grab();
	}
	m_Camera1.retrieve(image0);
	m_Camera2.retrieve(image1);
	
	imshow("Test Image.jpg", image0);
	imshow("Test Image.jpg", image1);
}
