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

    m_netTable = m_nt_Inst.GetTable("OpenCV");

    m_visioncounter = 0;
	//Keys = netTable->GetKeys();
	//for (auto key: Keys)
	//{
	//	cout<<"These are the keys: "<<key<<endl;
	//}
	m_ntval = m_netTable->GetValue("visioncounter");

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
		m_netTable->PutNumber("visioncounter", m_counter);

		m_netTable->PutNumber("RetroDistance",  m_RetroValues.GetDistance());
		m_netTable->PutNumber("RetroAngle", m_RetroValues.GetAngle());
		m_netTable->PutNumber("RetroQuality", m_RetroValues.GetQuality());

		m_netTable->PutNumber("LineDistance",  m_LineValues.GetDistance());
		m_netTable->PutNumber("LineAngle", m_LineValues.GetAngle());
		m_netTable->PutNumber("LineQuality", m_LineValues.GetQuality());

		m_netTable->PutNumber("CargoDistance",  m_CargoValues.GetDistance());
		m_netTable->PutNumber("CargoAngle", m_CargoValues.GetAngle());
		m_netTable->PutNumber("CargoQuality", m_CargoValues.GetQuality());

		m_netTable->PutNumber("HatchDistance",  m_HatchValues.GetDistance());
		m_netTable->PutNumber("HatchAngle", m_HatchValues.GetAngle());
		m_netTable->PutNumber("HatchQuality", m_HatchValues.GetQuality());

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
	return m_netTable->GetNumber("GyroAngle", 0);
}

int OffBoardComms::GetState()
{
	return m_netTable->GetNumber("Quality", 0);
}