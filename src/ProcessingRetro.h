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

	RectDescr GetLeftTarget() const { return m_leftTarget; }
	RectDescr GetRightTarget() const { return m_rightTarget; }

private:
};


#endif  // PROCESSING_RETRO_H