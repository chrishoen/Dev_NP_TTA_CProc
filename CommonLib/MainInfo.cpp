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
	Prn::print(Prn::CProc1, "CU  Update       birth certificate");
	BirthCertificateCU tBirthCertificateCU;
	tBirthCertificateCU.doReadFromJsonFile();

	// Read from the sysinfo json file, update it with variables, 
	// and write to it.
	Prn::print(Prn::CProc1, "CU  Update       sysinfo with birth certificate");
	Prn::print(Prn::CProc1, "CU  Update       sysinfo with software version");
	gSysInfo.doReadModifyWriteBegin();
	gSysInfo.readFrom(&tBirthCertificateCU);
	gSysInfo.mCU_SoftwareVersion = cSoftwareVersion;
	gSysInfo.doReadModifyWriteEnd();

	// Set the hlc threshold.
   SM::gShare->setHLCThresh(gSysInfo.mESSFlag);

	//***************************************************************************
	//***************************************************************************
	//***************************************************************************
	// Update the gain calc..

	return;
	if (gSysInfo.mESSFlag)
	{
		// Read the factory test record.
		gFactoryTestRecordCUESS.doReadFromJsonFile();
		Prn::print(Prn::CProc1, "CU  Update  factory test record ess");

		// Update the gain calculator json file.
		Prn::print(Prn::CProc1, "CU  Update  gain calc with factory test record ess");
		Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
		tCalc->doReadModifyWriteBegin();
		tCalc->readFrom(&gFactoryTestRecordCUESS);
		tCalc->doReadModifyWriteEnd();
	}
	else
	{
		// Update the gain calculator json file.
		gFactoryTestRecordCUSA.doReadFromJsonFile();
		Prn::print(Prn::CProc1, "CU  Update  factory test record sa");

		// Update the gain calculator json file.
		Prn::print(Prn::CProc1, "CU  Update  gain calc with factory test record sa");
		Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
		tCalc->doReadModifyWriteBegin();
		tCalc->readFrom(&gFactoryTestRecordCUSA);
		tCalc->doReadModifyWriteEnd();
	}
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
