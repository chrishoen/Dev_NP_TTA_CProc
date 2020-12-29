/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "smShare.h"
#include "SuperStateDefs.h"
#include "evtService.h"

#include "cxSuperStateEvaluator.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Evaluate superstates. The superstates are obtained from shared memory.

void SuperStateEvaluator::doEvaluateDA()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Store copies of the last and current superstate.

   if (!mValidFlagDA)
   {
      // If not valid then this is the first update.
      // Set the last and the current to the current from shared memory.
      mValidFlagDA = true;
      mSuperStateDA = SM::gShare->mSuperStateDA;
      mLastSuperStateDA = mSuperStateDA;
   }
   else
   {
      // If valid then this is not the first update.
      // Set the last to the previous current and set the current from shared
      // memory.
      mLastSuperStateDA = mSuperStateDA;
      mSuperStateDA = SM::gShare->mSuperStateDA;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate measurement variables.

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_DA_Temperature,
      mSuperStateDA.mTemperature > cDA_Temperature_ThreshHi))
   {
      tRecord->setArg1("%.1f", mSuperStateDA.mTemperature);
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_DA_MainVoltage,
      mSuperStateDA.mMainInputVoltage < cDA_MainVoltage_ThreshLo))
   {
      tRecord->setArg1("%.1f", mSuperStateDA.mMainInputVoltage);
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_DA_MainCurrent,
      mSuperStateDA.mMainInputCurrent < cDA_MainCurrent_ThreshLo))
   {
      tRecord->setArg1("%.1f", mSuperStateDA.mMainInputCurrent);
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_DA_TowerVoltage,
      mSuperStateDA.mTowerVoltage < cDA_TowerVoltage_ThreshLo))
   {
      tRecord->setArg1("%.1f", mSuperStateDA.mTowerVoltage);
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_DA_TowerCurrent,
      mSuperStateDA.mTowerCurrent < cDA_TowerCurrent_ThreshLo))
   {
      tRecord->setArg1("%.1f", mSuperStateDA.mTowerCurrent);
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate amp class variables.

   // Evaluate the superstate. Send an event accordingly.
   if (mSuperStateDA.mAmpClass != mLastSuperStateDA.mAmpClass)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_DA_AmpCurrent);
      tRecord->mCState = abs(mSuperStateDA.mAmpClass) > 0;
      switch (abs(mSuperStateDA.mAmpClass))
      {
      case 0: tRecord->mSeverity = Evt::cEvt_SeverityInfo;
      case 1: tRecord->mSeverity = Evt::cEvt_SeveritySevere;
      case 2: tRecord->mSeverity = Evt::cEvt_SeverityCritical;
      }
      tRecord->setArg1("%.1f", mSuperStateDA.mAmpRegCurrent);
      tRecord->setArg2("%s", get_AmpClass_asString(mSuperStateDA.mAmpClass));
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate mode variables.

   // Update the mode info.
   SM::gShare->doUpdateModeInfoDA();

   // Evaluate the superstate. Send an event accordingly.
   if (mSuperStateDA.mOpMode != mLastSuperStateDA.mOpMode)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_DA_OpMode);
      tRecord->setArg1("%s", get_OpMode_asString(mSuperStateDA.mOpMode));
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate path variables.

   // Evaluate the superstate. Send an event accordingly.
   if (mSuperStateDA.mRFPath != mLastSuperStateDA.mRFPath)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_DA_RFPath);
      tRecord->setArg1("%s", get_DA_RFPath_asString(mSuperStateDA.mRFPath));
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (mSuperStateDA.mUserAtten != mLastSuperStateDA.mUserAtten)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_DA_UserAtten);
      tRecord->setArg1("%.1f", mSuperStateDA.mUserAtten);
      tRecord->sendToEventLogThread();

      // Update the gain calculation json file.
      Prn::print(Prn::DA1, "DA  Update gain calc");
      Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
      tCalc->doReadModifyWriteBegin();
      tCalc->mAttenSetting = mSuperStateDA.mUserAtten;
      tCalc->doReadModifyWriteEnd();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
