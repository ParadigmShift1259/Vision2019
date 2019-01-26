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

void OffBoardComms::Publish(double value)
{
	m_netTable->PutNumber("visioncounter", m_counter);
	m_netTable->PutNumber("XOffAngle", value);

	m_counter++;

}

