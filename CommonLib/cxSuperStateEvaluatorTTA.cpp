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
   mFirstFlag = true;
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
   // Do this first.

   // Store copies of the last and current superstate.
   if (mFirstFlag)
   {
      Prn::print(Prn::TTA1, "TTA Eval         first **********************");
      // If this is the first update then set the last and the current to
      // the current from shared memory.
      mTTAX = SM::gShare->mSuperStateTTA;
      mLastTTAX = mTTAX;
   }
   else
   {
      // If this is not the first update then set the last to the previous
      // current and set the current from shared
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
      tRecord->setArg1("%.2f", mTTAX.mMainVoltage);
      tRecord->sendToEventLogThread();
   }

   // Evaluate the superstate. Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_TTA_MainCurrent,
      mTTAX.mMainCurrent < cTTA_MainCurrent_ThreshLo))
   {
      tRecord->setArg1("%.0f", mTTAX.mMainCurrent * 1000);
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate calculated variables.

   // Line resistiance.
   float tLineResistance = 0.0;

   // Nickname.
   SuperStateDA& tDAX = SM::gShare->mSuperStateDA;

   // If tta and da superstates are valid.
   if (tDAX.mValidFlag)
   {
      // Guard.
      bool tGoing = true;
      if (fabs(tDAX.mTowerVoltage) < cAD_Guard_ThreshLo) tGoing = false;
      if (fabs(tDAX.mTowerCurrent) < cAD_Guard_ThreshLo) tGoing = false;
      if (fabs(mTTAX.mMainVoltage) < cAD_Guard_ThreshLo) tGoing = false;
      if (fabs(mTTAX.mMainCurrent) < cAD_Guard_ThreshLo) tGoing = false;

      if (tGoing)
      {
         // Calculate line resistance.
         tLineResistance =
            (tDAX.mTowerVoltage - mTTAX.mMainVoltage) / tDAX.mTowerCurrent;
         if (tLineResistance < 0) tLineResistance = 0;
      }
   }

   // Evaluate the superstate. Send an event accordingly.
   mTTAX.mLineResistance = tLineResistance;
   SM::gShare->mSuperStateTTA.mLineResistance = tLineResistance;

   // Send an event accordingly.
   if (Evt::EventRecord* tRecord = Evt::trySendEvent(
      Evt::cEvt_Ident_TTA_LineResistance,
      mTTAX.mLineResistance > cTTA_LineResistance_ThreshHi))
   {
      tRecord->setArg1("%.2f", mTTAX.mLineResistance);
      tRecord->sendToEventLogThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate amp class variables.

   // Evaluate the superstate. Send an event accordingly.
   if (mTTAX.mAmpAClass != mLastTTAX.mAmpAClass)
   {
      Prn::print(Prn::TTA1, "TTA AmpAClass**************************** %s",
         get_AmpClass_asString(mTTAX.mAmpAClass));

      bool tCState = abs(mTTAX.mAmpAClass) > 0;
      int tSeverity = 0;
      switch (abs(mTTAX.mAmpAClass))
      {
      case  0: tSeverity = Evt::cEvt_SeverityInfo;
      case  1: tSeverity = Evt::cEvt_SeveritySevere;
      case  2: tSeverity = Evt::cEvt_SeverityCritical;
      }
      // Create new event record, set args, and send it to the event thread.
      if (Evt::EventRecord* tRecord = Evt::trySendEvent(
         Evt::cEvt_Ident_TTA_AmpACurrent,
         tCState,
         tSeverity))
      {
         tRecord->setArg1("%.0f", mTTAX.mAmpCurrentA * 1000);
         tRecord->setArg2("%s", get_AmpClass_asString(mTTAX.mAmpAClass));
         tRecord->sendToEventLogThread();
      }
   }

   // Evaluate the superstate. Send an event accordingly.
   if (mTTAX.mAmpBClass != mLastTTAX.mAmpBClass)
   {
      Prn::print(Prn::TTA1, "TTA AmpBClass**************************** %s",
         get_AmpClass_asString(mTTAX.mAmpBClass));

      bool tCState = abs(mTTAX.mAmpBClass) > 0;
      int tSeverity = 0;
      switch (abs(mTTAX.mAmpBClass))
      {
      case 0: tSeverity = Evt::cEvt_SeverityInfo;
      case 1: tSeverity = Evt::cEvt_SeveritySevere;
      case 2: tSeverity = Evt::cEvt_SeverityCritical;
      }
      // Create new event record, set args, and send it to the event thread.
      if (Evt::EventRecord* tRecord = Evt::trySendEvent(
         Evt::cEvt_Ident_TTA_AmpBCurrent,
         tCState,
         tSeverity))
      {
         tRecord->setArg1("%.0f", mTTAX.mAmpCurrentB*1000);
         tRecord->setArg2("%s", get_AmpClass_asString(mTTAX.mAmpBClass));
         tRecord->sendToEventLogThread();
      }
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
      Prn::print(Prn::TTA1, "TTA OpMode******************************* %s",
         get_OpMode_asString(mTTAX.mOpMode));

      // Create new event record, set args, and send it to the event thread.
      if (Evt::EventRecord* tRecord = Evt::trySendEvent(Evt::cEvt_Ident_TTA_OpMode))
      {
         tRecord->setArg1("%s", get_OpMode_asString(mTTAX.mOpMode));
         tRecord->sendToEventLogThread();
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate path variables.

   // Evaluate the superstate. Send an event accordingly.
   if (mTTAX.mPreferRFPath != mLastTTAX.mPreferRFPath)
   {
      Prn::print(Prn::TTA1, "TTA Prefer RF Path ********************** %s",
         get_TTA_RFPath_asString(mTTAX.mPreferRFPath));

      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = Evt::trySendEvent(Evt::cEvt_Ident_TTA_PreferRFPath);
      if (tRecord)
      {
         tRecord->setArg1("%s", get_TTA_RFPath_asString(mTTAX.mPreferRFPath));
         tRecord->sendToEventLogThread();
      }
   }

   // Evaluate the superstate. Send an event accordingly.
   if (mTTAX.mRFPath != mLastTTAX.mRFPath)
   {
      Prn::print(Prn::TTA1, "TTA RF Path ***************************** %s",
         get_TTA_RFPath_asString(mTTAX.mRFPath));

      // Create new event record, set args, and send it to the event thread.
      Evt::EventRecord* tRecord = Evt::trySendEvent(Evt::cEvt_Ident_TTA_RFPath);
      if (tRecord)
      {
         tRecord->setArg1("%s", get_TTA_RFPath_asString(mTTAX.mRFPath));
         tRecord->sendToEventLogThread();
      }
   }

   // Update the gain calculator, but not if there's a test mode.
   if ((mTTAX.mRFPath != mLastTTAX.mRFPath || mFirstFlag) &&
        (mTTAX.mRFPath < 2 && mLastTTAX.mRFPath < 2))
   {
      // Update the gain calculator.
      Prn::print(Prn::TTA1, "TTA Update       gain calc with rf path");
      Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
      tCalc->doReadModifyWriteBegin();
      tCalc->mRFPath = mTTAX.mRFPath;
      tCalc->doReadModifyWriteEnd();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this last.

   mFirstFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
