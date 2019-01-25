/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#include "OffBoardComms.h"
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
		m_visioncounter = ntval->GetDouble();
		cout << "Retrieving counter value: " << m_visioncounter << endl;
	}

	m_counter = m_visioncounter;

}

void OffBoardComms::Publish()
{
    /*Horizontal_Distance_Inch = Total_Distance_Inch * sin(Horizontal_Angle_Degree*PI/180);
    INCLUDE CUBE PROCESSING DIRECTORY IN ORDER TO ACCESS VARIABLES BELOW
	Vertical_Distance_Inch = Total_Distance_Inch * sin(Vertical_Angle_Degree*PI/180);
	Forward_Distance_Inch = Total_Distance_Inch*cos(Vertical_Angle_Degree*PI/180)*cos(Horizontal_Angle_Degree*PI/180);*/

	m_counter++;

}

