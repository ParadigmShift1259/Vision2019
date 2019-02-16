/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef PROCESSING_CARGO_H
#define PROCESSING_CARGO_H

#include "ProcessingBase.h"

class ProcessingCargo : public ProcessingBase
{
public:
    ProcessingCargo();
    ~ProcessingCargo();

private:
	// TODO rename these
	static constexpr double m_DEFAULT_HEIGHT_PIXEL = 510.0;	    //!< [pixel] Height in pixels of a cube placed 12 inches from the camer
	static constexpr double m_CAL_DISTANCE_INCH = 12.0;  		//!< [inch] Calibration distance from camera to cube
	static constexpr double m_MEAS_CUBE_HEIGHT = 12.3;  		//!< [inch] Height of cube in inches; used a tape measure in the real world
	static constexpr double m_DEFAULT_PIXEL_PER_INCH = m_DEFAULT_HEIGHT_PIXEL / m_MEAS_CUBE_HEIGHT;	// [pxiel/inch] ~41.463
	// Output value bounds
	static constexpr double m_ANGLE_THRESHOLD = 60.0;		    //!< [degrees] If we calculate an output angle more than this, do not send it
	static constexpr double m_FORWARD_DIST_THRESHOLD = 240.0;	//!< [inch] 20 feet; if we calculate an output distance more than this, do not send it
	static constexpr double m_CUBE_CONTOUR_THRESHOLD = 100.0; 	//!< [pixel] If the contour is smaller than this, do not process it
};


#endif  // PROCESSING_CARGO_H