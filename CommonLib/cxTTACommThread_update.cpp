/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "smShare.h"
#include "SysInfo.h"
#include "sxMsgDefs.h"
#include "cxCProcParms.h"

#include "cxTTACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Update the system information.

void TTACommThread::doUpdateSysInfoTTA()
{
	return;
	// Guard.
	if (mSysInfoValid) return;
	if (!mBirthCertificateValid) return;
	if (!mSoftwareVersionValid) return;


	// Read from the json file, update it with variables, 
	// and write to it.
	gSysInfo.doReadModifyWriteBegin();
	Prn::print(Prn::TTA1, "doSysInfoTTA BEGIN");

	gSysInfo.readFrom(&mBirthCertificate);
	gSysInfo.mTTA_SoftwareVersion = mSoftwareVersion;

	Prn::print(Prn::TTA1, "doSysInfoTTA END");
	gSysInfo.doReadModifyWriteEnd();

	// Done.
	mSysInfoValid = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Update the gain calculator.

void TTACommThread::doUpdateGainCalcTTA()
{
	return;
	// Nicknames.
	SuperStateTTA& tTTAX = SM::gShare->mSuperStateTTA;
	SuperStateDA& tDAX = SM::gShare->mSuperStateDA;

	// Guard.
	if (mGainCalcValid) return;
	if (!tTTAX.mValidFlag) return;
	if (!tDAX.mValidFlag) return;
	if (!mFactoryTestRecordValid) return;

	// Read the current system gain calculation json file.
	Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
	tCalc->doReadModifyWriteBegin();

	Prn::print(Prn::TTA1, "doUpdateGainCalcTTA BEGIN");

	// Set some variables.
	tCalc->mRFPath = tTTAX.mRFPath;
	tCalc->readFrom(&mFactoryTestRecordTTA);

	// Calculate the system gain and write to json file.
	Prn::print(Prn::TTA1, "doUpdateGainCalcTTA END");
	tCalc->doReadModifyWriteEnd();
	tCalc->show(Prn::TTA3);

	// Done.
	mGainCalcValid = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Update the gain calculator with the rf path.

void TTACommThread::doUpdateRFPathTTA()
{
	return;
	// Nicknames.
	SuperStateTTA& tTTAX = SM::gShare->mSuperStateTTA;
	SuperWantsTTA& tTTAW = SM::gShare->mSuperWantsTTA;

	if (mRFPathFirstFlag)
	{
		tTTAW.mPreferRFPath = tTTAX.mRFPath;
	}
	mRFPathFirstFlag = false;

	if (SM::gShare->mModeInfo.mTTARFPathChangeFlag_cproc)
	{
		SM::gShare->mModeInfo.mTTARFPathChangeFlag_cproc = false;

		if (tTTAX.mRFPath == 0)
		{
			//addNewEvent(EV_LNA_A_USER_REQUESTED);
			//std::string tString("TTA RFPath A");
			//addNewEvent(EV_SOFT_TEST1, false, { tString });

		}
		else
		{
			//addNewEvent(EV_LNA_B_USER_REQUESTED);
			//std::string tString("TTA RFPath B");
			//addNewEvent(EV_SOFT_TEST1, false, { tString });
		}
	}
	else
	{
		return;
	}

	// Guard.
	if (!mGainCalcValid) return;

	// Read the current system gain calculation json file.
	Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
	tCalc->doReadModifyWriteBegin();

	Prn::print(Prn::TTA1, "doUpdateRFPathTTA BEGIN");

	// Set some variables.
	tCalc->mRFPath = tTTAX.mRFPath;

	// Calculate the system gain and write to json file.
	Prn::print(Prn::TTA1, "doUpdateRFPAthTTA END");
	tCalc->doReadModifyWriteEnd();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace