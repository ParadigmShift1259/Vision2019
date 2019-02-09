/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#include "OffBoardComms.h"
#include <iostream>
//#include <stdio.h>
//#include <cstdint>
#include <unistd.h>
//#include "CubeProcessing"

OffBoardComms::OffBoardComms()
{
    m_nt_Inst = NetworkTableInstance::GetDefault();
	m_nt_Inst.StartClientTeam(1259);

	while (!m_nt_Inst.IsConnected())
	{
		sleep(0.1);
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
	m_counter = m_visioncounter;
}

void OffBoardComms::Publish()
{
	if (m_RetroValues.IsChanged() || m_LineValues.IsChanged() || m_CargoValues.IsChanged() || m_HatchValues.IsChanged())
	{
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

		m_counter++;
	}
}

void OffBoardComms::SetRetro(double distance, double angle, int quality)
{
	m_RetroValues = OutputValues(distance, angle, quality);
}

void OffBoardComms::SetLine(double distance, double angle, int quality)
{
	m_LineValues = OutputValues(distance, angle, quality);
}

void OffBoardComms::SetHatch(double distance, double angle, int quality)
{
	m_HatchValues = OutputValues(distance, angle, quality);
}

void OffBoardComms::SetCargo(double distance, double angle, int quality)
{
	m_CargoValues = OutputValues(distance, angle, quality);
}

double OffBoardComms::GetGyroAngle()
{
	return m_netTableSmartDashboard->GetNumber("GyroFused", 0);
}

int OffBoardComms::GetState()
{
	return m_netTableOpenCV->GetNumber("SelectedVisionTarget", 0);
}