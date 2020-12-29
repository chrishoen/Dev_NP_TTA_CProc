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

#define  _CXSUPERSTATEEVALUATORDA_CPP_
#include "cxSuperStateEvaluatorDA.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

SuperStateEvaluatorDA::SuperStateEvaluatorDA()
{
   reset();
}

void SuperStateEvaluatorDA::reset()
{
   mValidFlagDA = false;
   mValidFlagTTA = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Evaluate superstates. The superstates are obtained from shared memory.

void SuperStateEvaluatorDA::doEvaluate()
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
      mDAX = SM::gShare->mSuperStateDA;
      mLastDAX = mDAX;
   }
   else
   {
      // If valid then this is not the first update.
      // Set the last to the previous current and set the current from shared
      // memory.
      mLastDAX = mDAX;
      mDAX = SM::gShare->mSuperStateDA;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate measurement variables.

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_DA_Temperature,
      mDAX.mTemperature > cDA_Temperature_ThreshHi))
   {
      tRecord->setArg1("%.1f", mDAX.mTemperature);
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_DA_MainVoltage,
      mDAX.mMainInputVoltage < cDA_MainVoltage_ThreshLo))
   {
      tRecord->setArg1("%.1f", mDAX.mMainInputVoltage);
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_DA_MainCurrent,
      mDAX.mMainInputCurrent < cDA_MainCurrent_ThreshLo))
   {
      tRecord->setArg1("%.1f", mDAX.mMainInputCurrent);
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_DA_TowerVoltage,
      mDAX.mTowerVoltage < cDA_TowerVoltage_ThreshLo))
   {
      tRecord->setArg1("%.1f", mDAX.mTowerVoltage);
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_DA_TowerCurrent,
      mDAX.mTowerCurrent < cDA_TowerCurrent_ThreshLo))
   {
      tRecord->setArg1("%.1f", mDAX.mTowerCurrent);
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate amp class variables.

   // Evaluate the superstate. Send an event accordingly.
   if (mDAX.mAmpClass != mLastDAX.mAmpClass)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_DA_AmpCurrent);
      tRecord->mCState = abs(mDAX.mAmpClass) > 0;
      switch (abs(mDAX.mAmpClass))
      {
      case 0: tRecord->mSeverity = Evt::cEvt_SeverityInfo;
      case 1: tRecord->mSeverity = Evt::cEvt_SeveritySevere;
      case 2: tRecord->mSeverity = Evt::cEvt_SeverityCritical;
      }
      tRecord->setArg1("%.1f", mDAX.mAmpRegCurrent);
      tRecord->setArg2("%s", get_AmpClass_asString(mDAX.mAmpClass));
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate mode variables.

   // Update the mode info.
   SM::gShare->doUpdateModeInfoDA();

   // Evaluate the superstate. Send an event accordingly.
   if (mDAX.mOpMode != mLastDAX.mOpMode)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_DA_OpMode);
      tRecord->setArg1("%s", get_OpMode_asString(mDAX.mOpMode));
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate path variables.

   // Evaluate the superstate. Send an event accordingly.
   if (mDAX.mRFPath != mLastDAX.mRFPath)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_DA_RFPath);
      tRecord->setArg1("%s", get_DA_RFPath_asString(mDAX.mRFPath));
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (mDAX.mUserAtten != mLastDAX.mUserAtten)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_DA_UserAtten);
      tRecord->setArg1("%.1f", mDAX.mUserAtten);
      tRecord->sendToEventLogThread();

      // Update the gain calculation json file.
      Prn::print(Prn::DA1, "DA  Update gain calc");
      Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
      tCalc->doReadModifyWriteBegin();
      tCalc->mAttenSetting = mDAX.mUserAtten;
      tCalc->doReadModifyWriteEnd();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
