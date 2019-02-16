/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef OFFBOARDCOMMS_H
#define OFFBOARDCOMMS_H

#include "Const.h"
#ifdef USE_OFFBOARD_COMMS
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#endif

#include <memory>

#ifdef USE_OFFBOARD_COMMS
using namespace nt;
#endif
using namespace std;

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


class OffBoardComms
{
public:
    OffBoardComms();

    void Publish();

    void SetRetro(double dist, double angle, int quality);
    void SetLine(double dist, double angle, int quality);
    void SetCargo(double dist, double angle, int quality);
    void SetHatch(double dist, double angle, int quality);

    double GetGyroAngle();
    int GetState();

private:
#ifdef USE_OFFBOARD_COMMS
    NetworkTableInstance m_nt_Inst;
    shared_ptr<NetworkTable> m_netTableOpenCV;
    shared_ptr<NetworkTable> m_netTableSmartDashboard;
    shared_ptr<Value> m_ntval;
#endif

    double m_visioncounter;
    int m_counter;

    OutputValues m_RetroValues;
    OutputValues m_LineValues;
    OutputValues m_CargoValues;
    OutputValues m_HatchValues;
};

#endif  // OFFBOARDCOMMS_H
