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

enum EApproachDirection
{
	eUnknownDirection,
	eApproachFromLeft,
	eApproachFromCenter,
	eApproachFromRight,
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

	OutputValues& operator=(const OutputValues& rhs)
	{
		m_bChanged = false;
		
		if (m_distance != rhs.GetDistance())
		{
			m_distance = rhs.GetDistance();
			m_bChanged = true;
		}

		if (m_angle != rhs.GetAngle())
		{
			m_angle = rhs.GetAngle();
			m_bChanged = true;
		}

		if (m_quality != rhs.GetQuality())
		{
			m_quality = rhs.GetQuality();
			m_bChanged = true;
		}

		//if (m_approachDirection != rhs.m_approachDirection)
		//{
		//	m_approachDirection = rhs.GetApproachDirection();
		//	m_bChanged = true;
		//}

		return *this;
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

	//EApproachDirection GetApproachDirection() const
	//{
	//	return m_approachDirection;
	//}
	
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

	//void SetApproachDirection(EApproachDirection approachDirection)
	//{
	//	m_approachDirection = approachDirection;
	//}

private:
    double m_distance = 0.0;
    double m_angle = 0.0;
    int m_quality = eRedNoData;
	//EApproachDirection m_approachDirection = eUnknownDirection;

    bool m_bChanged = false;    
};

#endif  // OUTPUTVALUES_H
