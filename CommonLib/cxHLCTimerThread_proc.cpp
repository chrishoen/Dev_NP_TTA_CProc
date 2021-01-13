/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risProgramTime.h"
#include "smShare.h"
#include "cxCProcParms.h"
#include "evtService.h"
#include "evtEventStore.h"

#include "cxHLCTimerThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Process the input.

void HLCTimerThread::doProcess(int aTimeCount)
{
	//***************************************************************************
	//***************************************************************************
	//***************************************************************************
	// Do this first.

   // Nick name.
	HLCInfo& tH = SM::gShare->mHLCInfo;

	//***************************************************************************
	//***************************************************************************
	//***************************************************************************
	// Save state.

	// Save the last cstate.
	if (mFirstFlag)
	{
		tH.mLastCState = false;
	}
	else
	{
		tH.mLastCState = tH.mCState;
	}

	//************************************************************************
	//************************************************************************
	//************************************************************************
	// Evaluate the input.

	// Set the pulse detection.
	tH.mDetect  = tH.mInput > tH.mThreshHi;
	tH.mInRange = tH.mInput > tH.mThreshLo;

   // Set the condition state.
	tH.mCState = tH.mDetect && tH.mAlarmEnable;

	// Test if the condition state changed. If so then record the times.
	if (tH.mCState != tH.mLastCState)
	{
		if (tH.mCState)
		{
			mTime1 = Ris::getCurrentProgramTime();
			mDuration = 0;
		}
		else
		{
			mTime2 = Ris::getCurrentProgramTime();
			mDuration = mTime2 - mTime1;
		}
	}


   // Send an event accordingly.
	if (Evt::EventRecord* tRecord = Evt::trySendEvent(
		Evt::cEvt_Ident_HLC_AlarmEnable,
		tH.mAlarmEnable))
	{
		tRecord->sendToEventLogThread();
	}

	// Send an event accordingly.
	if (Evt::EventRecord* tRecord = Evt::trySendEvent(
		Evt::cEvt_Ident_HLC_Detect,
		tH.mCState))
	{
		tRecord->setArg1("%.1f", tH.mInput);
		tRecord->setArg2("%.1f", tH.mThreshHi);
		tRecord->setArg3("%.3f", mDuration);
		tRecord->sendToEventLogThread();
	}
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace