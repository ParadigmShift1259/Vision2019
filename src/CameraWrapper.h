/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef CAMERAWRAPPER_H
#define CAMERAWRAPPER_H

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class CameraWrapper
{
public:
    CameraWrapper();
    ~CameraWrapper();

    void AcquireImage();

    const Mat& GetImage() const { return m_image; }

private:
    VideoCapture m_Camera = VideoCapture(0);
    Mat m_image;
};
#endif  // CAMERAWRAPPER_H
