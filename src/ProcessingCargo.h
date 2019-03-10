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

	void ProcessImage(const Mat& image) override;

	const char* GetTargetName() override { return "Cargo"; }								//!< Derived class name to get unique filenames

private:
};


#endif  // PROCESSING_CARGO_H