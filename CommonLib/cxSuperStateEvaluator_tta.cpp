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

void SuperStateEvaluator::doEvaluateTTA()
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
      mSuperStateTTA = SM::gShare->mSuperStateTTA;
      mLastSuperStateTTA = mSuperStateTTA;
   }
   else
   {
      // If valid then this is not the first update.
      // Set the last to the previous current and set the current from shared
      // memory.
      mLastSuperStateTTA = mSuperStateTTA;
      mSuperStateTTA = SM::gShare->mSuperStateTTA;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate measurement variables.

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_TTA_Temperature,
      mSuperStateTTA.mTemperature > cTTA_Temperature_ThreshHi))
   {
      tRecord->setArg1("%.1f", mSuperStateTTA.mTemperature);
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_TTA_MainVoltage,
      mSuperStateTTA.mMainVoltage < cTTA_MainVoltage_ThreshLo))
   {
      tRecord->setArg1("%.1f", mSuperStateTTA.mMainVoltage);
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_TTA_MainCurrent,
      mSuperStateTTA.mMainCurrent < cTTA_MainCurrent_ThreshLo))
   {
      tRecord->setArg1("%.1f", mSuperStateTTA.mMainCurrent);
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate calculated variables.

   // Line resistiance.
   float tLineResistance = 0.0;

   // If tta and da superstates are valid.
   if (mValidFlagDA)
   {
      // Guard.
      bool tGoing = true;
      if (fabs(mSuperStateDA.mTowerVoltage) < cAD_Guard_ThreshLo) tGoing = false;
      if (fabs(mSuperStateDA.mTowerCurrent) < cAD_Guard_ThreshLo) tGoing = false;
      if (fabs(mSuperStateTTA.mMainVoltage) < cAD_Guard_ThreshLo) tGoing = false;
      if (fabs(mSuperStateTTA.mMainCurrent) < cAD_Guard_ThreshLo) tGoing = false;

      if (tGoing)
      {
         // Calculate line resistance.
         tLineResistance =
            (mSuperStateDA.mTowerVoltage - mSuperStateTTA.mMainVoltage) / mSuperStateDA.mTowerCurrent;
         if (tLineResistance < 0) tLineResistance = 0;
      }
   }

   // Store line resistance.
   mSuperStateTTA.mLineResistance = tLineResistance;
   SM::gShare->mSuperStateTTA.mLineResistance = tLineResistance;

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_TTA_LineResistance,
      mSuperStateTTA.mLineResistance > cTTA_LineResistance_ThreshHi))
   {
      tRecord->setArg1("%.1f", mSuperStateTTA.mMainCurrent);
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate amp class variables.

   // Evaluate the superstate. Send an event accordingly.
   if (mSuperStateTTA.mAmpAClass != mLastSuperStateTTA.mAmpAClass)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_TTA_AmpACurrent);
      tRecord->mCState = abs(mSuperStateTTA.mAmpAClass) > 0;
      switch (abs(mSuperStateTTA.mAmpAClass))
      {
      case 0: tRecord->mSeverity = Evt::cEvt_SeverityInfo;
      case 1: tRecord->mSeverity = Evt::cEvt_SeveritySevere;
      case 2: tRecord->mSeverity = Evt::cEvt_SeverityCritical;
      }
      tRecord->setArg1("%.1f", mSuperStateTTA.mAmpCurrentA);
      tRecord->setArg2("%s", get_AmpClass_asString(mSuperStateTTA.mAmpAClass));
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (mSuperStateTTA.mAmpBClass != mLastSuperStateTTA.mAmpBClass)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_TTA_AmpBCurrent);
      tRecord->mCState = abs(mSuperStateTTA.mAmpBClass) > 0;
      switch (abs(mSuperStateTTA.mAmpBClass))
      {
      case 0: tRecord->mSeverity = Evt::cEvt_SeverityInfo;
      case 1: tRecord->mSeverity = Evt::cEvt_SeveritySevere;
      case 2: tRecord->mSeverity = Evt::cEvt_SeverityCritical;
      }
      tRecord->setArg1("%.1f", mSuperStateTTA.mAmpCurrentB);
      tRecord->setArg2("%s", get_AmpClass_asString(mSuperStateTTA.mAmpBClass));
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate mode variables.

   // Evaluate the superstate. Send an event accordingly.
   if (mSuperStateTTA.mOpMode != mLastSuperStateTTA.mOpMode)
   {
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_TTA_OpMode);
      tRecord->setArg1("%s", get_OpMode_asString(mSuperStateTTA.mOpMode));
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate path variables.

   // Evaluate the superstate. Send an event accordingly.
   if (mSuperStateTTA.mPreferRFPath != mLastSuperStateTTA.mPreferRFPath)
   {
      Prn::print(Prn::TTA1, "TTA Prefer RF Path ********************** %s", get_TTA_RFPath_asString(mSuperStateTTA.mPreferRFPath));
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_TTA_PreferRFPath);
      tRecord->setArg1("%s", get_TTA_RFPath_asString(mSuperStateTTA.mPreferRFPath));
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (mSuperStateTTA.mRFPath != mLastSuperStateTTA.mRFPath)
   {
      Prn::print(Prn::TTA1, "TTA RF Path ***************************** %s", get_TTA_RFPath_asString(mSuperStateTTA.mRFPath));
      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = new Evt::EventRecord(Evt::cEvt_Ident_TTA_RFPath);
      tRecord->setArg1("%s", get_TTA_RFPath_asString(mSuperStateTTA.mRFPath));
      tRecord->sendToEventLogThread();
   }

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
