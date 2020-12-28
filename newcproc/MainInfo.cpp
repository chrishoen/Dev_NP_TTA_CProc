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
	Prn::print(Prn::View11, "CU  Update sys info");

	gSysInfo.readFrom(&tBirthCertificateCU);
	gSysInfo.mCU_SoftwareVersion = cSoftwareVersion;

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
		Prn::print(Prn::View11, "CU  Update factory test record ess");

		// Read the current system gain calculation json file.
		Prn::print(Prn::View11, "CU  Update gain calc ess");
		Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
		tCalc->doReadModifyWriteBegin();

		// Set some variables.
		tCalc->readFrom(&gFactoryTestRecordCUESS);

		// Calculate the system gain and write to json file.
		tCalc->doReadModifyWriteEnd();
		tCalc->show(Prn::View13);
	}
	else
	{
		// Read the factory test record.
		gFactoryTestRecordCUSA.doReadFromJsonFile();
		Prn::print(Prn::View11, "CU  Update factory test record sa");

		// Read the current system gain calculation json file.
		Prn::print(Prn::View11, "CU  Update gain calc ");
		Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
		tCalc->doReadModifyWriteBegin();

		// Set some variables.
		tCalc->readFrom(&gFactoryTestRecordCUSA);

		// Calculate the system gain and write to json file.
		tCalc->show(Prn::View13);
	}
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
