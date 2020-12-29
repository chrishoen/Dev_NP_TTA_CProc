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

#define  _CXSUPERSTATEEVALUATORTTA_CPP_
#include "cxSuperStateEvaluatorTTA.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

SuperStateEvaluatorTTA::SuperStateEvaluatorTTA()
{
   reset();
}

void SuperStateEvaluatorTTA::reset()
{
   mValidFlagDA = false;
   mValidFlagTTA = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Evaluate superstates. The superstates are obtained from shared memory.

void SuperStateEvaluatorTTA::doEvaluate()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Store copies of the last and current superstate.

   if (!mValidFlagTTA)
   {
      // If not valid then this is the first update.
      // Set the last and the current to the current from shared memory.
      mValidFlagTTA = true;
      mTTAX = SM::gShare->mSuperStateTTA;
      mLastTTAX = mTTAX;
   }
   else
   {
      // If valid then this is not the first update.
      // Set the last to the previous current and set the current from shared
      // memory.
      mLastTTAX = mTTAX;
      mTTAX = SM::gShare->mSuperStateTTA;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate measurement variables.

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_TTA_Temperature,
      mTTAX.mTemperature > cTTA_Temperature_ThreshHi))
   {
      tRecord->setArg1("%.1f", mTTAX.mTemperature);
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_TTA_MainVoltage,
      mTTAX.mMainVoltage < cTTA_MainVoltage_ThreshLo))
   {
      tRecord->setArg1("%.1f", mTTAX.mMainVoltage);
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_TTA_MainCurrent,
      mTTAX.mMainCurrent < cTTA_MainCurrent_ThreshLo))
   {
      tRecord->setArg1("%.1f", mTTAX.mMainCurrent);
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate calculated variables.

   // Line resistiance.
   float tLineResistance = 0.0;
#if 0
   // If tta and da superstates are valid.
   if (mValidFlagDA)
   {
      // Guard.
      bool tGoing = true;
      if (fabs(mSuperStateDA.mTowerVoltage) < cAD_Guard_ThreshLo) tGoing = false;
      if (fabs(mSuperStateDA.mTowerCurrent) < cAD_Guard_ThreshLo) tGoing = false;
      if (fabs(mTTAX.mMainVoltage) < cAD_Guard_ThreshLo) tGoing = false;
      if (fabs(mTTAX.mMainCurrent) < cAD_Guard_ThreshLo) tGoing = false;

      if (tGoing)
      {
         // Calculate line resistance.
         tLineResistance =
            (mSuperStateDA.mTowerVoltage - mTTAX.mMainVoltage) / mSuperStateDA.mTowerCurrent;
         if (tLineResistance < 0) tLineResistance = 0;
      }
   }
#endif
   // Store line resistance.
   mTTAX.mLineResistance = tLineResistance;
   SM::gShare->mSuperStateTTA.mLineResistance = tLineResistance;

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_TTA_LineResistance,
      mTTAX.mLineResistance > cTTA_LineResistance_ThreshHi))
   {
      tRecord->setArg1("%.1f", mTTAX.mMainCurrent);
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate amp class variables.

   // Evaluate the superstate. Send an event accordingly.
   if (mTTAX.mAmpAClass != mLastTTAX.mAmpAClass)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_TTA_AmpACurrent);
      tRecord->mCState = abs(mTTAX.mAmpAClass) > 0;
      switch (abs(mTTAX.mAmpAClass))
      {
      case 0: tRecord->mSeverity = Evt::cEvt_SeverityInfo;
      case 1: tRecord->mSeverity = Evt::cEvt_SeveritySevere;
      case 2: tRecord->mSeverity = Evt::cEvt_SeverityCritical;
      }
      tRecord->setArg1("%.1f", mTTAX.mAmpCurrentA);
      tRecord->setArg2("%s", get_AmpClass_asString(mTTAX.mAmpAClass));
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (mTTAX.mAmpBClass != mLastTTAX.mAmpBClass)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_TTA_AmpBCurrent);
      tRecord->mCState = abs(mTTAX.mAmpBClass) > 0;
      switch (abs(mTTAX.mAmpBClass))
      {
      case 0: tRecord->mSeverity = Evt::cEvt_SeverityInfo;
      case 1: tRecord->mSeverity = Evt::cEvt_SeveritySevere;
      case 2: tRecord->mSeverity = Evt::cEvt_SeverityCritical;
      }
      tRecord->setArg1("%.1f", mTTAX.mAmpCurrentB);
      tRecord->setArg2("%s", get_AmpClass_asString(mTTAX.mAmpBClass));
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate mode variables.

   // Update the mode info.
   SM::gShare->doUpdateModeInfoTTA();

   // Evaluate the superstate. Send an event accordingly.
   if (mTTAX.mOpMode != mLastTTAX.mOpMode)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_TTA_OpMode);
      tRecord->setArg1("%s", get_OpMode_asString(mTTAX.mOpMode));
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate path variables.

   // Evaluate the superstate. Send an event accordingly.
   if (mTTAX.mPreferRFPath != mLastTTAX.mPreferRFPath)
   {
      Prn::print(Prn::TTA1, "TTA Prefer RF Path ********************** %s", get_TTA_RFPath_asString(mTTAX.mPreferRFPath));
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_TTA_PreferRFPath);
      tRecord->setArg1("%s", get_TTA_RFPath_asString(mTTAX.mPreferRFPath));
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   // Update the gain calculator.
   if (mTTAX.mRFPath != mLastTTAX.mRFPath)
   {
      Prn::print(Prn::TTA1, "TTA RF Path ***************************** %s", get_TTA_RFPath_asString(mTTAX.mRFPath));

      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_TTA_RFPath);
      tRecord->setArg1("%s", get_TTA_RFPath_asString(mTTAX.mRFPath));
      tRecord->sendToEventLogThread();

      // Update the gain calculation json file.
      Prn::print(Prn::TTA1, "TTA Update gain calc");
      Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
      tCalc->doReadModifyWriteBegin();
      tCalc->mRFPath = mTTAX.mRFPath;
      tCalc->doReadModifyWriteEnd();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
