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

class OffBoardComms()
{
    public:

    void Init();
    void Loop();

    double Total_Distance_Inch = 0;
    double Horizontal_Angle_Degree = 0;
    double Vertical_Angle_Degree = 0;

    private:
    NetworkTableInstance nt_Inst;
    shared_ptr<NetworkTable> netTable;
    double visioncounter;
    auto Keys;
    auto ntval;
    int counter; 
    double Horizontal_Distance_Inch = 0;
    double Vertical_Distance_Inch = 0;
    double Forward_Distance_Inch = 0;


}
#endif
