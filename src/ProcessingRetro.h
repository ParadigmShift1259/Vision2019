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
};


#endif  // PROCESSING_RETRO_H