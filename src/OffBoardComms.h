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

class OffBoardComms
{
public:
    OffBoardComms();
    void Publish(double value);

private:
    NetworkTableInstance m_nt_Inst;
    shared_ptr<NetworkTable> m_netTable;
    double m_visioncounter;
    //auto m_Keys;
    shared_ptr<Value> m_ntval;
    int m_counter; 
};
#endif  // OFFBOARDCOMMS_H
