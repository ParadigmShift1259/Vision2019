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

private:
    
};


#endif  // PROCESSING_LINE_H