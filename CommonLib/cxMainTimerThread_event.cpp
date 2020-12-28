/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "smShare.h"
#include "evtService.h"
#include "cxDiscretes.h"
#include "cxCProcParms.h"

#include "cxMainTimerThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread timer execution. This is called periodically.

void MainTimerThread::executeOnTimer_Events(int aTimeCount)
{
   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Poll the clear latched alarms flag.

   if (SM::gShare->mSuperWantsCU.mClearInactiveAlarms)
   {
      // Clear the flag.
      SM::gShare->mSuperWantsCU.mClearInactiveAlarms = false;

      // Send an event to the event log thread.
      Evt::doSendEvent(Evt::cEvt_Ident_ClearInactiveAlarms);
   }

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Set the front panel leds.

   int tActiveAlarmCount = SM::gShare->mSuperStateCU.mActiveAlarmCount;

   // Set the mode led according to the test mode.
   if (SM::gShare->mModeInfo.mSysTestMode == cSys_TestMode_None)
   {
      // Not in test mode, green.
      gDiscretes.mModeLed.streamWrite(HIGH);
   }
   else
   {
      // In test mode, yellow.
      gDiscretes.mModeLed.streamWrite(LOW);
      tActiveAlarmCount = 0;
   }

   // Set the status led according to the active alarm count.
   if (tActiveAlarmCount == 0)
   {
      // No active alarms, green.
      gDiscretes.mStatusLed.streamWrite(HIGH);
      gDiscretes.mTTA0Out.streamWrite(HIGH);
      gDiscretes.mTTA1Out.streamWrite(HIGH);
   }
   else
   {
      // Active alarms, red.
      gDiscretes.mStatusLed.streamWrite(LOW);
      gDiscretes.mTTA0Out.streamWrite(LOW);
      gDiscretes.mTTA1Out.streamWrite(LOW);
   }

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Set the buzzer according to the hlc detections.

   if (gCProcParms.mBuzzerEnable)
   {
      // Set the buzzer on hlc detect.
      if (SM::gShare->mHLCInfo.mDetect)
      {
         gDiscretes.mBuzzer.streamWrite(HIGH);
      }
      else
      {
         gDiscretes.mBuzzer.streamWrite(LOW);
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace