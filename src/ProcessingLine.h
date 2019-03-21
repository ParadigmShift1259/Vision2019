/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef PROCESSING_LINE_H
#define PROCESSING_LINE_H

#include "ProcessingBase.h"

class ProcessingLine : public ProcessingBase
{
public:
    ProcessingLine();
    ~ProcessingLine();

	void ProcessImage(const Mat& image) override;
	void SetRetroCoords(const RectDescr& leftTarget, const RectDescr& rightTarget)
	{
		m_leftTarget = leftTarget;
		m_rightTarget = rightTarget;
	}

	const char* GetTargetName() override { return "Line"; }								//!< Derived class name to get unique filenames

private:
	static constexpr double m_calibTargetSizePixel = 176.0;					//!< [pixel] Height in pixels of a target placed m_calibCameraDistInch from the camera

	void RejectSmallContours();												//!< Process the contours to reject very small and very large contours by point count
	void CalcOutputValues();												//!< Calculate the values that will be sent to the robot

	double m_startOfLineX = 0.0;									//!< X coord for start of alignment line (start = closest to wall)
	double m_startOfLineY = 0.0;									//!< Y coord for start of alignment line 
	double m_endOfLineX = 0.0;										//!< X coord for end of alignment line
	double m_endOfLineY = 0.0;										//!< Y coord for end of alignment line
	double m_lineLen = 0.0;    										//!< [pixel] Alignment line length on image 
};


#endif  // PROCESSING_LINE_H