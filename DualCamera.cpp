#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat image0, image1;

int main()
{
	VideoCapture m_Camera1 = VideoCapture(0);
	VideoCapture m_Camera2 = VideoCapture(1);
	
	for (int loop = 0; loop < 60 ; loop++)
	{
		m_Camera1.grab();
		m_Camera2.grab();
	}
	m_Camera1.retrieve(image0);
	m_Camera2.retrieve(image1);
	
	VideoCapture::imshow("Test Image.jpg", image0);
	VideoCapture::imshow("Test Image.jpg", image1);
}
