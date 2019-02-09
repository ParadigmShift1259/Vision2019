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

private:
    double m_distLine;
    double m_angleLine;
    
    enum EQualityLine 
    {
        eRedNoData,
        eYellowTrackingObjects,
        eGreenReady,
    };
};


#endif  // PROCESSING_LINE_H