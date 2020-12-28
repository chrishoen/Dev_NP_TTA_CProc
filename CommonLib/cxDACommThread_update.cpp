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

#include "cxDACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Update the system information.

void DACommThread::doUpdateSysInfoDA()
{
	return;

	SuperStateDA& tDAX = SM::gShare->mSuperStateDA;

	// Guard.
	if (mSysInfoValid) return;
	if (!mBirthCertificateValid) return;
	if (!mSoftwareVersionValid) return;
	if (!tDAX.mValidFlag) return;


	// Read from the jon file, update it with variables, 
	// and write to it.
	gSysInfo.doReadModifyWriteBegin();
	Prn::print(Prn::DA1, "doSysInfoDA BEGIN %d", tDAX.mSystemType);

	gSysInfo.readFrom(&mBirthCertificate);
	gSysInfo.mDA_SoftwareVersion = mSoftwareVersion;
	gSysInfo.mESSFlag = tDAX.mSystemType == cDA_SystemType_ESS;

	Prn::print(Prn::DA1, "doSysInfoDA END");
	gSysInfo.doReadModifyWriteEnd();

	// Done.
	mSysInfoValid = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Update the gain calculator.

void DACommThread::doUpdateGainCalcDA()
{
	return;
	// Nicknames.
	SuperStateTTA& tTTAX = SM::gShare->mSuperStateTTA;
	SuperStateDA& tDAX = SM::gShare->mSuperStateDA;
	Calc::GainCalc& tCalc = SM::gShare->mGainCalc;

	// Guard.
	if (mGainCalcValid) return;
	if (!tTTAX.mValidFlag) return;
	if (!tDAX.mValidFlag) return;

	// Read the current system gain calculation json file. 
	tCalc.doReadModifyWriteBegin();

	Prn::print(Prn::DA1, "doUpdateGainCalcDA BEGIN");

	// Set some variables.
	tCalc.mSystemType = tDAX.mSystemType;
	tCalc.mAttenSetting = tDAX.mUserAtten;

	tCalc.doMakeConsistent();

	// Calculate the system gain and write to json file.
	Prn::print(Prn::DA1, "doUpdateGainCalcDA END");
	tCalc.doReadModifyWriteEnd();

	// Done.
	mGainCalcValid = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Update the gain calculator with the user attenuation.

void DACommThread::doUpdateUserAttenDA()
{
	return;
	// Nicknames.
	SuperStateDA& tDAX = SM::gShare->mSuperStateDA;
	Calc::GainCalc& tCalc = SM::gShare->mGainCalc;

	// Guard.
	if (!mGainCalcValid) return;
	if (!tDAX.mValidFlag) return;

	// If the user atten hasn't changed then exit.
	if (tCalc.mAttenSetting == tDAX.mUserAtten) return;

	// Read the current system gain calculation json file. 
	tCalc.doReadModifyWriteBegin();

	Prn::print(Prn::DA1, "doUpdateUserAttenDA BEGIN");

	// Set some variables.
	tCalc.mAttenSetting = tDAX.mUserAtten;

	// Calculate the system gain and write to json file.
	Prn::print(Prn::DA1, "doUpdateUserAttenDA END");
	tCalc.doReadModifyWriteEnd();

// std::string tString = my_string_from_double(tDAX.mUserAtten);
// addNewEvent(EV_USER_REQUESTED_ATTENUATION_CHANGE, false, { tString });

	// Done.
	mGainCalcValid = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace