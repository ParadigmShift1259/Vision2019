/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef OUTPUTVALUES_H
#define OUTPUTVALUES_H

enum EVisionTarget
{
	eUnknownState,
	eLoadingStation,
	eDeliverHatch,
	eDeliverCargo,
	eFindCargo,
	eFindHatch
};

enum EQuality
{
    eRedNoData,
    eYellowTrackingObjects,
    eGreenReady
};

class OutputValues
{
public:
    OutputValues(){}
    OutputValues(double dist, double angle, int quality)
        : m_distance(dist)
        , m_angle(angle)
        , m_quality(quality)
    {
        if (m_distance != dist)
        {
            m_distance = dist;
            m_bChanged = true;
        }

        if (m_angle != angle)
        {
            m_angle = angle;
            m_bChanged = true;
        }

        if (m_quality != quality)
        {
            m_quality = quality;
            m_bChanged = true;
        }
    }

    const bool IsChanged() const
    {
        return m_bChanged;
    }

    const double GetDistance() const 
    {
        return m_distance;
    }

    const double GetAngle() const
    {
        return m_angle;
    }

    const int GetQuality() const
    {
        return m_quality;
    }

    void SetDistance(double distance) 
    {
        m_distance = distance;
    }

    void SetAngle(double angle) 
    {
        m_angle = angle;
    }

    void SetQuality(int quality) 
    {
        m_quality = quality;
    }

private:
    double m_distance = 0.0;
    double m_angle = 0.0;
    int m_quality = eRedNoData;

    bool m_bChanged = false;    
};

#endif  // OUTPUTVALUES_H