/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef CAMERAWRAPPER_H
#define CAMERAWRAPPER_H

#include <raspicam.h>
#include <raspicam_cv.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace raspicam;
using namespace std;

class CameraWrapper
{
public:
    CameraWrapper();
    ~CameraWrapper();

    void AcquireImage();

    const Mat& GetImage() const { return m_image; }

private:
    RaspiCam_Cv m_Camera;
    Mat m_image;
};
#endif  // CAMERAWRAPPER_H
