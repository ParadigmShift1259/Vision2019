/**
 * Developed by 
 * Agastya Asthana
 * Scott Coursin
 * Ping Xue
*/

#ifndef PROCESSING_RETRO_H
#define PROCESSING_RETRO_H

#include "ProcessingBase.h"

class ProcessingRetro : public ProcessingBase
{
public:
    ProcessingRetro();
    ~ProcessingRetro();

	void ProcessImage(const Mat& image) override;

	RectDescr GetLeftTarget() const { return m_leftTarget; }
	RectDescr GetRightTarget() const { return m_rightTarget; }
	
	//const OutputValues& GetLeftTargetOutputValues() const override { return m_OutputValuesLeftTarget; }
	//const OutputValues& GetRightTargetOutputValues() const override { return m_OutputValuesRightTarget; }

	const char* GetTargetName() override { return "Retro"; }								//!< Derived class name to get unique filenames

	EApproachDirection GetApproachDirection() const
	{
		return m_approachDirection;
	}

private:
	//OutputValues m_OutputValuesLeftTarget;                                //!< Values to send to the Robot
	//OutputValues m_OutputValuesRightTarget;                                //!< Values to send to the Robot
	EApproachDirection m_approachDirection = eUnknownDirection;
};


#endif  // PROCESSING_RETRO_H