/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#include "OffBoardComms.h"
//#include "CubeProcessing"

void OffBoardComms::Init()
{
    nt_Inst = NetworkTableInstance::GetDefault();
	nt_Inst.StartClientTeam(1259);

	while(!nt_Inst.IsConnected())
	{
		sleep(0.1);
	}

    netTable = nt_Inst.GetTable("OpenCV");

    visioncounter = 0;
	Keys = netTable->GetKeys();
	for (auto key: Keys)
	{
		cout<<"These are the keys: "<<key<<endl;
	}
	ntval = netTable->GetValue("visioncounter");

    if (ntval)
	{
		visioncounter = ntval->GetDouble();
		cout<<"Retrieving counter value: "<<visioncounter<<endl;

	}

    if(visioncounter > 0)
	{
		counter = visioncounter;
	}
	else
	{
		counter = 0;
	}
}

void OffBoardComms::Loop()
{
    /*Horizontal_Distance_Inch = Total_Distance_Inch * sin(Horizontal_Angle_Degree*PI/180);
    INCLUDE CUBE PROCESSING DIRECTORY IN ORDER TO ACCESS VARIABLES BELOW
	Vertical_Distance_Inch = Total_Distance_Inch * sin(Vertical_Angle_Degree*PI/180);
	Forward_Distance_Inch = Total_Distance_Inch*cos(Vertical_Angle_Degree*PI/180)*cos(Horizontal_Angle_Degree*PI/180);*/

}

