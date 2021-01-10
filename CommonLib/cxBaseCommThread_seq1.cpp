/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risProgramTime.h"

#include "smShare.h"
#include "sxMsgDefs.h"
#include "cxCProcParms.h"
#include "evtService.h"

#include "cxBaseCommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run sequence qcall function. Execute an infinite loop sequence that
// sends a request to the slave, waits for the response, and processes it.
// It calls one of the process subroutines, based on the state. It executes
// in the context of the long thread. The purpose of this is to provide
// long thread execution context for message processing.

void BaseCommThread::executeRunSeq1()
{
   Prn::print(0, "%sCommThread::executeRunSeq1 BEGIN", mLabel);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Initialize the synchronization objects.
   mSeqWaitable.initialize(cSlowSeqPeriod);
   mSeqWaitableSlow = true;
   mNotify.clearFlags();

   try
   {
      // Seq to transmit and receive messages.
      while (true)
      {
         //*********************************************************************
         //*********************************************************************
         //*********************************************************************
         // Wait.

         // If true then there was a serial timeout.
         bool tTimeoutFlag = false;

         // If true then there was a processing error.
         bool tProcErrorFlag = false;

         // Wait for timer or abort.
         mSeqWaitable.waitForTimerOrSemaphore();

         // Test for an abort
         if (mSeqWaitable.wasSemaphore()) throw 668;

         // Guard.
         if (!mConnectionFlag) continue;

         //*********************************************************************
         //*********************************************************************
         //*********************************************************************
         // Send a request to the slave, wait for the response and process it.

         try
         {
            // Default. This will be set by the specific subfunction.
            mSeqExitCode = cSeqExitNormal;

            // Test for a notification exception.
            // This can throw an execption if there's an abort.
            mNotify.testException();

            // Set the thread notification mask.
            mNotify.setMaskOne("RxMsg", cRxMsgNotifyCode);

            // Execute a specific subfunction based on the state.
            // This can throw an exception.
            if (!doProcess())
            {
               // There was a processing error.
               tProcErrorFlag = true;
            }
         }
         catch (int aException)
         {
            if (aException == Ris::Threads::Notify::cTimeoutException)
            {
               Prn::print(0, "EXCEPTION %sCommThread::doProcess TIMEOUT %s %d",
                  mLabel,
                  SX::get_MsgId_asString(mSeqState),
                  aException);

               Prn::print(mPF1, "EXCEPTION %s TIMEOUT %s",
                  mLabel,
                  SX::get_MsgId_asString(mSeqState));

               // Serial rx timeout.
               tTimeoutFlag = true;

               // There was a processing error.
               tProcErrorFlag = true;
            }
            else if (aException == cSeqExitError)
            {
               Prn::print(0, "EXCEPTION %sCommThread::doProcess ERROR %s %d",
                  mLabel,
                  SX::get_MsgId_asString(mSeqState),
                  aException);

               Prn::print(mPF1, "EXCEPTION %s ERROR %s",
                  mLabel,
                  SX::get_MsgId_asString(mSeqState));

               // There was a processing error.
               tProcErrorFlag = true;
            }
            else
            {
               Prn::print(0, "EXCEPTION %sCommThread::doProcess %s %d %s",
                  mLabel,
                  SX::get_MsgId_asString(mSeqState),
                  aException,
                  mNotify.mException);

               // There was a processing error.
               tProcErrorFlag = true;
            }
         }

         //*********************************************************************
         //*********************************************************************
         //*********************************************************************
         // Check errors.
         
         if (tProcErrorFlag)
         {
            // There was a processing error, set the sequence state back to
            // the top of the ladder.
            mSeqState = SX::cMsgId_tst;
         }

         //*********************************************************************
         //*********************************************************************
         //*********************************************************************
         // Events.

         if (mTTAFlag)
         {
            // Send an event accordingly.
            if (Evt::EventRecord* tRecord = Evt::trySendEvent(
               Evt::cEvt_Ident_TTA_CommLost,
               tTimeoutFlag))
            {
               tRecord->sendToEventLogThread();
            }
         }
         else
         {
            // Send an event accordingly.
            if (Evt::EventRecord* tRecord = Evt::trySendEvent(
               Evt::cEvt_Ident_DA_CommLost,
               tTimeoutFlag))
            {
               tRecord->sendToEventLogThread();
            }
         }
      }
   }
   catch (int aException)
   {
      if (aException == 668)
      {
         Prn::print(0, "EXCEPTION %sCommThread::executeRunSeq1 %d %s", 
            mLabel, 
            aException, mNotify.mException);
      }
      else
      {
         Prn::print(0, "EXCEPTION %sCommThread::executeRunSeq1 %d",
            mLabel,
            aException);
      }
   }
   catch (...)
   {
      Prn::print(0, "EXCEPTION %sCommThread::executeRunSeq1 UNKNOWN", mLabel);
   }

   // Finalize the synchronization objects.
   mSeqWaitable.finalize();
   mNotify.clearFlags();

   Prn::print(0, "%sCommThread::executeRunSeq1 END", mLabel);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a request message to the slave, wait for the response message and
// process it. Return true if successful. This is called by the process
// sequence qcall function, based on the state.

bool BaseCommThread::doProcess()
{
   // Get begin time.
   doProcess_begin();

   // Process messages based on the state.
   if (mSeqState == SX::cMsgId_tst)
   {
      if (doProcess_tst())
      {
         mSeqState = SX::cMsgId_gbc;
         setSeqWaitableSlow();
         doProcess_end();
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (mSeqState == SX::cMsgId_gbc)
   {
      if (doProcess_gbc())
      {
         mSeqState = SX::cMsgId_gsv;
         setSeqWaitableSlow();
         doProcess_end();
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (mSeqState == SX::cMsgId_gsv)
   {
      if (doProcess_gsv())
      {
         mSeqState = SX::cMsgId_gft;
         setSeqWaitableSlow();
         doProcess_end();
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (mSeqState == SX::cMsgId_gft)
   {
      if (doProcess_gft())
      {
         mSeqState = SX::cMsgId_gsx;
         setSeqWaitableFast();
         doProcess_end();
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (mSeqState == SX::cMsgId_gsx)
   {
      if (doProcess_gsx())
      {
         doProcess_end();
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (mSeqState == SX::cMsgId_gcs)
   {
      if (doProcess_gsx())
      {
         doProcess_end();
         return true;
      }
      else
      {
         return false;
      }
   }

   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Helpers to show process time duration.

void BaseCommThread::doProcess_begin()
{
   // Get start time.
   mSeqTime1 = Ris::getCurrentProgramTime();
}

void BaseCommThread::doProcess_end()
{
   // Get finish time.
   mSeqTime2 = Ris::getCurrentProgramTime();
   mSeqDuration = mSeqTime2 - mSeqTime1;
   Prn::print(mPF8, "                                                           %.3f", mSeqDuration);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace