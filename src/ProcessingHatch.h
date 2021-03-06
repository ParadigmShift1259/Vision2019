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

	const char* GetTargetName() override { return "Hatch"; }								//!< Derived class name to get unique filenames

private:
    
};


#endif  // PROCESSING_CARGO_H