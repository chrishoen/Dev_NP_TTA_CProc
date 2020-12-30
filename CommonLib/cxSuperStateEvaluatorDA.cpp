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

#include "FactoryTestRecordCUESS.h"
#include "FactoryTestRecordCUSA.h"
#include "SysInfo.h"

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
   mFirstFlag = true;
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
   // Do this first.

   // Store copies of the last and current superstate.
   if (mFirstFlag)
   {
      Prn::print(Prn::DA1, "DA  Eval         first **********************");
      // If this is the first update then set the last and the current to
      // the current from shared memory.
      mDAX = SM::gShare->mSuperStateDA;
      mLastDAX = mDAX;
   }
   else
   {
      // If this is not the first update then set the last to the previous
      // current and set the current from shared
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
      bool tCState = abs(mDAX.mAmpClass) > 0;
      int tSeverity = 0;
      switch (abs(mDAX.mAmpClass))
      {
      case 0: tSeverity = Evt::cEvt_SeverityInfo;
      case 1: tSeverity = Evt::cEvt_SeveritySevere;
      case 2: tSeverity = Evt::cEvt_SeverityCritical;
      }
      // Create new event record, set args, and send it to the event thread.
      if (Evt::EventRecord* tRecord = Evt::trySendEvent(
         Evt::cEvt_Ident_DA_AmpCurrent,
         tCState,
         tSeverity))
      {
         tRecord->setArg1("%.1f", mDAX.mAmpRegCurrent);
         tRecord->setArg2("%s", get_AmpClass_asString(mDAX.mAmpClass));
         tRecord->sendToEventLogThread();
      }
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
      if (Evt::EventRecord* tRecord = Evt::trySendEvent(Evt::cEvt_Ident_DA_OpMode))
      {
         tRecord->setArg1("%s", get_OpMode_asString(mDAX.mOpMode));
         tRecord->sendToEventLogThread();
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate path variables.

   // Evaluate the superstate. Send an event accordingly.
   if (mDAX.mOpMode != mLastDAX.mOpMode)
   {
      // Create new event record, set args, and send it to the event thread.
      if (Evt::EventRecord* tRecord = Evt::trySendEvent(Evt::cEvt_Ident_DA_RFPath))
      {
         tRecord->setArg1("%s", get_DA_RFPath_asString(mDAX.mRFPath));
         tRecord->sendToEventLogThread();
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate user attenuator.

   // Evaluate the superstate. Send an event accordingly.
   if (mDAX.mUserAtten != mLastDAX.mUserAtten)
   {
      Prn::print(Prn::TTA1, "DA  User Atten ************************** %.1f", mDAX.mUserAtten);
      // Create new event record, set args, and send it to the event thread.
      if (Evt::EventRecord* tRecord = Evt::trySendEvent(Evt::cEvt_Ident_DA_UserAtten))
      {
         tRecord->setArg1("%.1f", mDAX.mUserAtten);
         tRecord->sendToEventLogThread();
      }
   }
   
   // Update the gain calculator.
   if (mDAX.mUserAtten != mLastDAX.mUserAtten || mFirstFlag)
   {
      // Update the gain calculator.
      Prn::print(Prn::TTA1, "DA  Update       gain calc user atten");
      Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
      tCalc->doReadModifyWriteBegin();
      tCalc->mAttenSetting = mDAX.mUserAtten;
      tCalc->doReadModifyWriteEnd();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate system type.

   // Update the sysinfo and gain calc.
   if (mFirstFlag)
   {
      // Update the sysinfo.
      Prn::print(Prn::DA1, "DA  Update       sysinfo with system type %s",
         get_DA_SystemType_asString(mDAX.mSystemType));
      gSysInfo.doReadModifyWriteBegin();
      gSysInfo.mESSFlag = mDAX.mSystemType == 1;
      gSysInfo.doReadModifyWriteEnd();

      if (gSysInfo.mESSFlag)
      {
         // Update the cu factory test record.
         gFactoryTestRecordCUESS.doReadFromJsonFile();
         Prn::print(Prn::CProc1, "CU  Update       factory test record ess");

         // Update the gain calculator.
         Prn::print(Prn::CProc1, "CU  Update       gain calc with factory test record ess");
         Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
         tCalc->doReadModifyWriteBegin();
         tCalc->readFrom(&gFactoryTestRecordCUESS);
         tCalc->doReadModifyWriteEnd();
      }
      else
      {
         // Update the gain calculator json file.
         gFactoryTestRecordCUSA.doReadFromJsonFile();
         Prn::print(Prn::CProc1, "CU  Update       factory test record sa");

         // Update the gain calculator json file.
         Prn::print(Prn::CProc1, "CU  Update       gain calc with factory test record sa");
         Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
         tCalc->doReadModifyWriteBegin();
         tCalc->readFrom(&gFactoryTestRecordCUSA);
         tCalc->doReadModifyWriteEnd();
      }
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
