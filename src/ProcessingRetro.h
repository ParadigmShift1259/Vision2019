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

private:
    double m_distRetro;
    double m_angleRetro;
    
    enum EQualityRetro 
    {
        eRedNoData,
        eYellowTrackingObjects,
        eGreenReady,
    };
};


#endif  // PROCESSING_RETRO_H