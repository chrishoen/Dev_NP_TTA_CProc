/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risSystemCalls.h"

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
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Poll the clear latched alarms flag.

   if (SM::gShare->mSuperWantsCU.mClearInactiveAlarms)
   {
      // Clear the flag.
      SM::gShare->mSuperWantsCU.mClearInactiveAlarms = false;

      // Send an event to the event log thread.
      Evt::doSendEvent(Evt::cEvt_Ident_ClearInactiveAlarms);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Poll the alarm flag.

   // Manage the flags.
   if (mFirstFlag) mLastAlarmEnable = true;
   bool tAlarmEnable = SM::gShare->mSuperWantsCU.mAlarmEnable;

   // Test for a change.
   if (tAlarmEnable != mLastAlarmEnable)
   {
      Prn::print(Prn::CProc1, "CU  alarm enable **************************** %s", my_string_from_bool(tAlarmEnable));
      Evt::doSendEvent(Evt::cEvt_Ident_EnableEventLog, tAlarmEnable);
   }

   // Manage the flags.
   mLastAlarmEnable = tAlarmEnable;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Poll the modeinfo.

   // Nickname.
   ModeInfo& tModeInfo = SM::gShare->mModeInfo;

   // Test if the sys test mode has changed.
   if (tModeInfo.mSysTestModeChangeFlag_cproc)
   {
      tModeInfo.mSysTestModeChangeFlag_cproc = false;
      Prn::print(Prn::CProc1, "CU  Sys Test Mode *********************** %s",
         get_Sys_TestMode_asString2(tModeInfo.mSysTestMode));
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = Evt::trySendEvent(Evt::cEvt_Ident_CU_SysTestMode);
      if (tRecord)
      {
         tRecord->setArg1("%s", get_Sys_TestMode_asString2(tModeInfo.mSysTestMode));
         tRecord->sendToEventLogThread();
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
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
   if (tActiveAlarmCount == 0 || !SM::gShare->mSuperWantsCU.mAlarmEnable)
   {
      // If there are no active alarms or if global alarms are disabled.
      // No active alarms, green.
      gDiscretes.mStatusLed.streamWrite(HIGH);
      gDiscretes.mTTA0Out.streamWrite(HIGH);
      gDiscretes.mTTA1Out.streamWrite(HIGH);
   }
   else
   {
      // If there are active alarms and if global alarms are enabled.
      // Active alarms, red.
      gDiscretes.mStatusLed.streamWrite(LOW);
      gDiscretes.mTTA0Out.streamWrite(LOW);
      gDiscretes.mTTA1Out.streamWrite(LOW);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Set the buzzer according to the hlc detections.

   // Set the buzzer on hlc detect.
   if (SM::gShare->mHLCInfo.mBuzzerEnable && SM::gShare->mHLCInfo.mDetect)
   {
      gDiscretes.mBuzzer.streamWrite(HIGH);
   }
   else
   {
      gDiscretes.mBuzzer.streamWrite(LOW);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Poll the reboot flag.

   if (SM::gShare->mSuperWantsCU.mCUReboot)
   {
      Prn::print(Prn::CProc1, "CU  REBOOT *********************************************");
      SM::gShare->mSuperWantsCU.mCUReboot = false;
      Ris::doSystemCommand("reboot");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace