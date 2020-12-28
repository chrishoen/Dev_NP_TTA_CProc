#include "stdafx.h"

#include "BirthCertificate.h"
#include "SoftwareVersion.h"
#include "FactoryTestRecordCUESS.h"
#include "FactoryTestRecordCUSA.h"
#include "SysInfo.h"
#include "smShare.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize program info.

void main_info_initialize()
{
	//***************************************************************************
	//***************************************************************************
	//***************************************************************************
	// Update the system info.
	
	// Read the birth certificate.
	BirthCertificateCU tBirthCertificateCU;
	tBirthCertificateCU.doReadFromJsonFile();

	// Read from the sys info json file, update it with variables, 
	// and write to it.
	gSysInfo.doReadModifyWriteBegin();
	Prn::print(Prn::View11, "doSysInfoCU BEGIN");

	gSysInfo.readFrom(&tBirthCertificateCU);
	gSysInfo.mCU_SoftwareVersion = cSoftwareVersion;

	Prn::print(Prn::View11, "doSysInfoCU END");
	gSysInfo.doReadModifyWriteEnd();

	// Set the hlc threshold.
   SM::gShare->setHLCThresh(gSysInfo.mESSFlag);

	//***************************************************************************
	//***************************************************************************
	//***************************************************************************
	// Update the gain calc..

	if (gSysInfo.mESSFlag)
	{
		// Read the factory test record.
		gFactoryTestRecordCUESS.doReadFromJsonFile();
		Prn::print(Prn::View11, "CU mCtrlUnitGain %6.2f", gFactoryTestRecordCUESS.mCtrlUnitGain);

		// Read the current system gain calculation json file.
		Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
		tCalc->doReadModifyWriteBegin();

		Prn::print(Prn::View11, "doUpdateGainCalcCUESS BEGIN");

		// Set some variables.
		tCalc->readFrom(&gFactoryTestRecordCUESS);

		// Calculate the system gain and write to json file.
		Prn::print(Prn::View11, "doUpdateGainCalcCUESS END");
		tCalc->doReadModifyWriteEnd();
		tCalc->show(Prn::View13);
	}
	else
	{
		// Read the factory test record.
		gFactoryTestRecordCUSA.doReadFromJsonFile();
		Prn::print(Prn::View11, "CU mCtrlUnitGain %6.2f", gFactoryTestRecordCUSA.mCtrlUnitGain);

		// Read the current system gain calculation json file.
		Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
		tCalc->doReadModifyWriteBegin();

		Prn::print(Prn::View11, "doUpdateGainCalcCUSA BEGIN");

		// Set some variables.
		tCalc->readFrom(&gFactoryTestRecordCUSA);

		// Calculate the system gain and write to json file.
		Prn::print(Prn::View11, "doUpdateGainCalcCUSA END");
		tCalc->doReadModifyWriteEnd();
		tCalc->show(Prn::View13);
	}
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
