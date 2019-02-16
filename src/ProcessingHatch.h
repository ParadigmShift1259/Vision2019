/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef PROCESSING_HATCH_H
#define PROCESSING_HATCH_H

#include "ProcessingBase.h"

class ProcessingHatch : public ProcessingBase
{
public:
    ProcessingHatch();
    ~ProcessingHatch();

	void ProcessImage(const Mat& image) override;

private:
    
};


#endif  // PROCESSING_CARGO_H