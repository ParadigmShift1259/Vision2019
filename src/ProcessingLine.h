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

private:
	void RejectSmallContours();												//!< Process the contours to reject very small and very large contours by point count
};


#endif  // PROCESSING_LINE_H