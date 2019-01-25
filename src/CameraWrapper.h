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

using namespace raspicam;

class CameraWrapper()
{
    public:

    CameraWrapper();
    ~CameraWrapper();

    void Init();
    void Loop();




    private:
    RaspiCam_Cv Camera;
    const Scalar upper = Scalar(20, 280, 50);
    const Scalar lower = Scalar(40, 255, 255);

}
#endif
