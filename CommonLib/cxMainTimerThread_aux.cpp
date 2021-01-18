/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "smShare.h"
#include "cxDiscretes.h"
#include "cxCProcParms.h"
#include "evtService.h"
#include "evtEventStore.h"

#include "cxMainTimerThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize the event table for aux alarms.

void MainTimerThread::initialize_AuxAlarms()
{

	// Nick name.
	AuxAlarmInfo& tA = SM::gShare->mAuxAlarmInfo;

	// Seq through the aux alarms.
	for (int i = 0; i < 5; i++)
	{
		// Set the alarm severity.
		Evt::gEventStore.mEventTable(Evt::cEvt_Ident_Aux_AuxInput[i]).mSeverity =
			tA.mSeverity[i];
	}
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread timer execution. This is called periodically.

void MainTimerThread::executeOnTimer_AuxAlarms(int aTimeCount)
{
	//***************************************************************************
	//***************************************************************************
	//***************************************************************************
	// Do this first.

   // Nick name.
	AuxAlarmInfo& tA = SM::gShare->mAuxAlarmInfo;

	// Save the last test mode state.
	if (mFirstFlag)
	{
		tA.mLastTestState = false;
	}
	else
	{
		tA.mLastTestState = tA.mTestState;
	}

	// Default to this.
	tA.mTestState = false;

	//***************************************************************************
	//***************************************************************************
	//***************************************************************************
	// Seq through the aux alarms.

	for (int i = 0; i < 5; i++)
	{
		//************************************************************************
		//************************************************************************
		//************************************************************************
		// Save state.

		// Save the last cstate.
		if (mFirstFlag)
		{
			tA.mLastCState[i] = false;
		}
		else
		{
			tA.mLastCState[i] = tA.mCState[i];
		}

		//************************************************************************
		//************************************************************************
		//************************************************************************
		// Update the aux alarm discrete inputs via the gpio or from the
		// overrides.

		if (!gCProcParms.mAuxOverrideEnable)
		{
			// Read the inputs from the discretes.
			tA.mInput[i] = !gDiscretes.mAuxAlarmIn[i]->getValue();
		}
		else
		{
			// Override the inputs.
			tA.mInput[i] = tA.mOverrideInput[i];
		}

		//************************************************************************
		//************************************************************************
		//************************************************************************
		// Evaluate an input.

   	// Set the condition state.
		if (tA.mEnable[i])
		{
			// If enabled then compare the input with the polarity.
			tA.mCState[i] = tA.mInput[i] == tA.mPolarity[i];
		}
		else
		{
			// If not enabled then false.
			tA.mCState[i] = false;
		}

		// Send an event accordingly.
		if (Evt::EventRecord* tRecord = Evt::trySendEvent(
			Evt::cEvt_Ident_Aux_AuxInput[i],
			tA.mCState[i],
			tA.mSeverity[i]))
		{
			tRecord->setArg1("%s", tA.mLabel[i]);
			tRecord->setArg2("%s", my_string_from_bool(tA.mInput[i]));
			tRecord->sendToEventLogThread();
		}

		// Test for test enable.
		if (tA.mTestEnable[i])
		{
			// Test if the alarm is active.
			if (tA.mCState[i])
			{
				tA.mTestState = true;
			}
		}
	}

	//***************************************************************************
	//***************************************************************************
	//***************************************************************************
	// Process test mode requests.

   // Test if the test state has changed.
	if (tA.mTestState != tA.mLastTestState)
	{
		// Test if the alarm is active.
		if (tA.mTestState)
		{
			// Request test mode for terminate a.
			Prn::print(Prn::View21, "setTestMode_TerminateA");
			SM::gShare->setTestMode_TerminateA(3600);
		}
		// Test if the alarm is not active.
		else
		{
			// Request test mode for none.
			Prn::print(Prn::View21, "setTestMode_None");
			SM::gShare->setTestMode_None();
		}
	}
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace