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

private:
    double m_distHatch;
    double m_angleHatch;
    
    enum EQualityHatch 
    {
        eRedNoData,
        eYellowTrackingObjects,
        eGreenReady,
    };
};


#endif  // PROCESSING_CARGO_H