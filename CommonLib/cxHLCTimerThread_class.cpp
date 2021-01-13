/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "cmnPriorities.h"

#include "FactoryTestRecordCUESS.h"
#include "FactoryTestRecordCUSA.h"
#include "SysInfo.h"

#include "smShare.h"
#include "evtService.h"
#include "cxCProcParms.h"

#define  _CXHLCTIMERTHREAD_CPP_
#include "cxHLCTimerThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

HLCTimerThread::HLCTimerThread()
   : mHLC("/dev/spidev1.0")
{
   // Set base class variables.
   BaseClass::setThreadName("HLCTimer");
   BaseClass::setThreadPriority(Cmn::gPriorities.mHLCTimer);
   BaseClass::setThreadPrintLevel(0);
   BaseClass::mTimerPeriod = gCProcParms.mHLCTimerPeriod;

   // Set member variables.
   mFirstFlag = true;
   mTime1 = 0;
   mTime2 = 0;
   mDuration = 0;

   mAlphaFilter.initializeFromStep(
      gCProcParms.mHLCTimerPeriod/1000.0,
      gCProcParms.mHLCStepTime,
      0.90);

   // Set the hlc info variables.
   HLCInfo& tH = SM::gShare->mHLCInfo;

   if (gSysInfo.mESSFlag)
   {
      tH.mThreshHi = -35;
   }
   else
   {
      tH.mThreshHi = -40;
   }
   tH.mThreshLo = gCProcParms.mHLCThreshLo;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. It initializes hlc.

void HLCTimerThread::threadInitFunction()
{
   Prn::print(Prn::HLC1, "HLCTimerThread::threadInitFunction BEGIN %d", gCProcParms.mHLCOverrideEnable);

   if (!gCProcParms.mHLCOverrideEnable)
   {
      // Set the hlc info variables.
      if (gSysInfo.mESSFlag)
      {
         mHLC.setOffset(gCProcParms.mHLCOffsetESS);
      }
      else
      {
         mHLC.setOffset(gCProcParms.mHLCOffsetSA);
      }

      // Initialize.
      mHLC.enable();
      mHLC.connect();
   }

   Prn::print(Prn::HLC1, "HLCTimerThread::threadInitFunction END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread timer execution. This is called periodically.

void HLCTimerThread::executeOnTimer(int aTimeCount)
{
   // Read the input. Sensor or override.
   doRead(aTimeCount);

   // Process the input.
   doProcess(aTimeCount);

   // Done.
   mFirstFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It releases SDL resources and closes
// the thread SDL window.

void HLCTimerThread::threadExitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace