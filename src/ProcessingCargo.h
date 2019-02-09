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

    double m_distCargo;
    double m_angleCargo;
    
    enum EQualityCargo 
    {
        eRedNoData,
        eYellowTrackingObjects,
        eGreenReady,
    };

};


#endif  // PROCESSING_CARGO_H