/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef PROCESSING_RETRO_H
#define PROCESSING_RETRO_H

#include "ProcessingBase.h"

class ProcessingRetro : public ProcessingBase
{
public:
    ProcessingRetro();
    ~ProcessingRetro();

	void ProcessImage(const Mat& image) override;

private:
	static constexpr double m_DEFAULT_HEIGHT_PIXEL = 510.0;	    //!< [pixel] Height in pixels of a cube placed 12 inches from the camer
	static constexpr double m_calibCameraDistInch = 12.0;  		//!< [inch] Calibration distance from camera to cube
	static constexpr double m_MEAS_CUBE_HEIGHT = 12.3;  		//!< [inch] Height of cube in inches; used a tape measure in the real world
	static constexpr double m_DEFAULT_PIXEL_PER_INCH = m_DEFAULT_HEIGHT_PIXEL / m_MEAS_CUBE_HEIGHT;	// [pxiel/inch] ~41.463
};


#endif  // PROCESSING_RETRO_H