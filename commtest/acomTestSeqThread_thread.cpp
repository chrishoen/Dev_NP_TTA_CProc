/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "cmnPriorities.h"
#include "cmnShare.h"
#include "acomCommParms.h"

#define  _ACOMTESTSEQTHREAD_CPP_
#include "acomTestSeqThread.h"

namespace ACom
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TestSeqThread::TestSeqThread()
   : mCmdAckNotify(&mNotify, cCmdAckNotifyCode)
{
   using namespace std::placeholders;

   // Set base class thread variables.
   BaseClass::mShortThread->setThreadName("TestSeqShort");
   BaseClass::mShortThread->setThreadPriority(Cmn::gPriorities.mTestSeqShort);
   BaseClass::mShortThread->setThreadPrintLevel(gCommParms.mTestSeqShortPrintLevel);

   BaseClass::mLongThread->setThreadName("TestSeqLong");
   BaseClass::mLongThread->setThreadPriority(Cmn::gPriorities.mTestSeqLong);
   BaseClass::mLongThread->setThreadPrintLevel(gCommParms.mTestSeqLongPrintLevel);

   // Set base class call pointers.
   BaseClass::mShortThread->mThreadInitCallPointer           = std::bind(&TestSeqThread::threadInitFunction, this);
   BaseClass::mShortThread->mThreadExitCallPointer           = std::bind(&TestSeqThread::threadExitFunction, this);
   BaseClass::mShortThread->mThreadExecuteOnTimerCallPointer = std::bind(&TestSeqThread::executeOnTimer, this, _1);

   // Set qcalls.
   mRunTest1QCall.bind(this->mLongThread, this, &TestSeqThread::executeRunTest1);
   mAbortTestQCall.bind(this->mShortThread, this, &TestSeqThread::executeAbortTest);
   mSessionQCall.bind(this->mShortThread, this, &TestSeqThread::executeSession);
   mRxStringQCall.bind(this->mShortThread, this, &TestSeqThread::executeRxString);

   // Set member variables.
   mLoopExitCode = 0;
   mTxCount = 0;
   mRxCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// before the thread starts running.

void TestSeqThread::threadInitFunction()
{
   using namespace std::placeholders;

   // Instance of serial port settings.
   Ris::SerialSettings tSerialSettings;

   tSerialSettings.setPortDevice(gCommParms.mCmdCommPortDevice);
   tSerialSettings.setPortSetup(gCommParms.mCmdCommPortSetup);
   tSerialSettings.mRxTimeout = gCommParms.mCmdCommPortTimeout;
   tSerialSettings.mTermMode = Ris::cSerialTermMode_CRLF;
   tSerialSettings.mThreadPriority = Cmn::gPriorities.mSerialString;
   tSerialSettings.mPrintLevel = gCommParms.mSerialStringPrintLevel;
   tSerialSettings.mSessionQCall = mSessionQCall;
   tSerialSettings.mRxStringQCall = mRxStringQCall;
   tSerialSettings.m485Flag = gCommParms.mCmdCommPort485Flag;

   // Create the child thread.
   mSerialStringThread = new Ris::SerialStringThread(tSerialSettings);

   // Launch the child thread.
   mSerialStringThread->launchThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately 
// after  the thread starts running.

void TestSeqThread::threadExitFunction()
{
   // Shutdown the child thread
   mSerialStringThread->shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This shuts down the two threads.

void TestSeqThread::shutdownThreads()
{
   // Abort the long thread.
   BaseClass::mNotify.abort();

   // Shutdown the two threads.
   BaseClass::shutdownThreads();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort a running grid or test qcall.
//
void TestSeqThread::executeAbortTest()
{
   Prn::print(Prn::View01, "ABORT TEST>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
   Prn::print(Prn::View01, "ABORT TEST>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
   Prn::print(Prn::View01, "ABORT TEST>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

   // Abort the long thread.
   BaseClass::mNotify.abort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Session qcall function. This is invoked by the child thread when 
// the serial port is closed because of an error or when it is reopened
// correctly.

void TestSeqThread::executeSession(bool aConnected)
{
   if (aConnected)
   {
      Prn::print(0, "TestSeqThread serial port open  CONNECTED");
   }
   else
   {
      Prn::print(0, "TestSeqThread serial port error DISCONNECTED");
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace