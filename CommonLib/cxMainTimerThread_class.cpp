/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "evtEventParms.h"
#include "cmnPriorities.h"

#include "smShare.h"
#include "evtService.h"

#define  _CXMAINTIMERTHREAD_CPP_
#include "cxMainTimerThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

MainTimerThread::MainTimerThread()
{
   // Set base class variables.
   BaseClass::setThreadName("MainTimer");
   BaseClass::setThreadPriority(Cmn::gPriorities.mMainTimer);
   BaseClass::setThreadPrintLevel(0);
   BaseClass::mTimerPeriod = 200;

   // Set member variables.
   mFirstFlag = true;
   mLastAlarmEnable = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. It initializes SDL and creates the
// thread SDL window and associated resources.

void MainTimerThread::threadInitFunction()
{
   // Initialize the event table for aux alarms.
   initialize_AuxAlarms();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread timer execution. This is called periodically.

void MainTimerThread::executeOnTimer(int aTimeCount)
{
   // Activity count.
   SM::gShare->mSuperStateCU.mCount++;

   // Execute subroutines.
   executeOnTimer_Events(aTimeCount);
   executeOnTimer_AuxAlarms(aTimeCount);

   // Done.
   mFirstFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It releases SDL resources and closes
// the thread SDL window.

void MainTimerThread::threadExitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace