/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef OFFBOARDCOMMS_H
#define OFFBOARDCOMMS_H

#include "Const.h"
#include "OutputValues.h"
#include "ProcessingBase.h"

#ifdef USE_OFFBOARD_COMMS
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#endif

#include <memory>

#ifdef USE_OFFBOARD_COMMS
using namespace nt;
#endif
using namespace std;


class OffBoardComms
{
public:
    OffBoardComms();

	bool IsConnected();
	void Connect();
    void Publish();

	void SetRetro(const ProcessingBase& retro);
    void SetLine(const ProcessingBase& line);
    void SetCargo(const ProcessingBase& cargo);
    void SetHatch(const ProcessingBase& hatch);

	//void SetLeftTargetValues(const ProcessingBase& retro);
	//void SetRightTargetValues(const ProcessingBase& retro);
	void SetApproachDirection(EApproachDirection approachDirection)
	{
		m_approachDirection = approachDirection;
	}

    double GetGyroAngle();
	EVisionTarget GetState();

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
	//OutputValues m_LeftTargetValues;
	//OutputValues m_RightTargetValues;
	EApproachDirection m_approachDirection = eUnknownDirection;
};

#endif  // OFFBOARDCOMMS_H
