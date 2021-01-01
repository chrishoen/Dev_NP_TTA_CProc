#include "stdafx.h"

#include "BirthCertificate.h"
#include "SoftwareVersion.h"
#include "FactoryTestRecordCUESS.h"
#include "FactoryTestRecordCUSA.h"
#include "SysInfo.h"
#include "smShare.h"
#include "CProcInfo.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize program info.

void main_info_initialize()
{
	//***************************************************************************
	//***************************************************************************
	//***************************************************************************
	// Update the cproc info.

	gCProcInfo.doProtectedRead();

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

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
