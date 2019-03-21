/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#include "OffBoardComms.h"
#include <iostream>

#ifndef BUILD_ON_WINDOWS
#include <unistd.h>	// For sleep; building on Pi only
#endif

OffBoardComms::OffBoardComms()
{
#ifdef USE_OFFBOARD_COMMS
	cout << "Initializing off board comms" << endl;
	m_nt_Inst = NetworkTableInstance::GetDefault();
	m_nt_Inst.StartClientTeam(1259);

	while (!m_nt_Inst.IsConnected())
	{
#ifdef BUILD_ON_WINDOWS
		Sleep(100);
#else
		sleep(0.1);
#endif
	}

    m_netTableOpenCV = m_nt_Inst.GetTable("OpenCV");
    m_netTableSmartDashboard = m_nt_Inst.GetTable("SmartDashboard");

    m_visioncounter = 0;
	//Keys = netTable->GetKeys();
	//for (auto key: Keys)
	//{
	//	cout<<"These are the keys: "<<key<<endl;
	//}
	m_ntval = m_netTableOpenCV->GetValue("visioncounter");

    if (m_ntval)
	{
		m_visioncounter = m_ntval->GetDouble();
		cout << "Retrieving counter value: " << m_visioncounter << endl;
	}
#endif
	m_counter = (int)m_visioncounter;
}

void OffBoardComms::Publish()
{
	if (m_RetroValues.IsChanged() || m_LineValues.IsChanged() || m_CargoValues.IsChanged() || m_HatchValues.IsChanged())
	{
		//cout << "Retro IsChanged " << m_RetroValues.IsChanged() << endl;
		//cout << "Line  IsChanged " << m_LineValues.IsChanged() << endl;
		//cout << "Cargo IsChanged " << m_CargoValues.IsChanged() << endl;
		//cout << "Hatch IsChanged " << m_HatchValues.IsChanged() << endl;
#ifdef USE_OFFBOARD_COMMS
		m_netTableOpenCV->PutNumber("visioncounter", m_counter);

		m_netTableOpenCV->PutNumber("RetroDistance",  m_RetroValues.GetDistance());
		m_netTableOpenCV->PutNumber("RetroAngle", m_RetroValues.GetAngle());
		m_netTableOpenCV->PutNumber("RetroQuality", m_RetroValues.GetQuality());

		m_netTableOpenCV->PutNumber("LineDistance",  m_LineValues.GetDistance());
		m_netTableOpenCV->PutNumber("LineAngle", m_LineValues.GetAngle());
		m_netTableOpenCV->PutNumber("LineQuality", m_LineValues.GetQuality());

		m_netTableOpenCV->PutNumber("CargoDistance",  m_CargoValues.GetDistance());
		m_netTableOpenCV->PutNumber("CargoAngle", m_CargoValues.GetAngle());
		m_netTableOpenCV->PutNumber("CargoQuality", m_CargoValues.GetQuality());

		m_netTableOpenCV->PutNumber("HatchDistance",  m_HatchValues.GetDistance());
		m_netTableOpenCV->PutNumber("HatchAngle", m_HatchValues.GetAngle());
		m_netTableOpenCV->PutNumber("HatchQuality", m_HatchValues.GetQuality());

		m_netTableOpenCV->PutNumber("RetroLeftDistance", m_LeftTargetValues.GetDistance());
		m_netTableOpenCV->PutNumber("RetroLeftAngle", m_LeftTargetValues.GetAngle());
		m_netTableOpenCV->PutNumber("RetroLeftQuality", m_LeftTargetValues.GetQuality());

		m_netTableOpenCV->PutNumber("RetroRightDistance", m_RightTargetValues.GetDistance());
		m_netTableOpenCV->PutNumber("RetroRightAngle", m_RightTargetValues.GetAngle());
		m_netTableOpenCV->PutNumber("RetroRightQuality", m_RightTargetValues.GetQuality());
#endif
		m_counter++;
	}
}

void OffBoardComms::SetRetro(const ProcessingBase& retro)
{
	m_RetroValues = retro.GetOutputValues();
}

void OffBoardComms::SetLine(const ProcessingBase& line)
{
	m_LineValues = line.GetOutputValues();
}

void OffBoardComms::SetHatch(const ProcessingBase& hatch)
{
	m_HatchValues = hatch.GetOutputValues();
}

void OffBoardComms::SetCargo(const ProcessingBase& cargo)
{
	m_CargoValues = cargo.GetOutputValues();
}

void OffBoardComms::SetLeftTargetValues(const ProcessingBase& retro)
{
	m_LeftTargetValues = retro.GetLeftTargetOutputValues();
}

void OffBoardComms::SetRightTargetValues(const ProcessingBase& retro)
{
	m_RightTargetValues = retro.GetRightTargetOutputValues();
}

double OffBoardComms::GetGyroAngle()
{
#ifdef USE_OFFBOARD_COMMS
	return m_netTableSmartDashboard->GetNumber("GyroFused", 0);
#else
	return 0.0;
#endif
}

EVisionTarget OffBoardComms::GetState()
{
//#ifdef USE_OFFBOARD_COMMS
//	return (EVisionTarget)(int)m_netTableOpenCV->GetNumber("SelectedVisionTarget", 0);
//#else
	return eLoadingStation;
//#endif
}