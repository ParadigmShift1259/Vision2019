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

using namespace nt;

class OffBoardComms
{
    public:

    void Init();
    void Publish();

    double m_Total_Distance_Inch = 0;
    double m_Horizontal_Angle_Degree = 0;
    double m_Vertical_Angle_Degree = 0;

    private:
    NetworkTableInstance m_nt_Inst;
    shared_ptr<NetworkTable> m_netTable;
    double m_visioncounter;
    //auto m_Keys;
    shared_ptr<Value> m_ntval;
    int m_counter; 
    double m_Horizontal_Distance_Inch = 0;
    double m_Vertical_Distance_Inch = 0;
    double m_Forward_Distance_Inch = 0;


}
#endif
