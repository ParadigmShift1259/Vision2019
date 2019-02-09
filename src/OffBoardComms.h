/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef OFFBOARDCOMMS_H
#define OFFBOARDCOMMS_H

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <memory>

using namespace nt;
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
    OuputValues(double dist, double angle, int quality)
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
    NetworkTableInstance m_nt_Inst;
    shared_ptr<NetworkTable> m_netTable;

    double m_visioncounter;

    OutputValues m_RetroValues;
    OutputValues m_LineValues;
    OutputValues m_CargoValues;
    OutputValues m_HatchValues;

    //auto m_Keys;
    shared_ptr<Value> m_ntval;
    int m_counter; 

};
#endif  // OFFBOARDCOMMS_H
