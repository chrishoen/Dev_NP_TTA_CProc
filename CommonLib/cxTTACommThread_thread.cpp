/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "cmnPriorities.h"
#include "cxCProcParms.h"

#define  _CXTTACOMMTHREAD_CPP_
#include "cxTTACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TTACommThread::TTACommThread()
   : mCmdAckNotify(&mNotify, cCmdAckNotifyCode)
{
   using namespace std::placeholders;

   // Set base class thread variables.
   BaseClass::mShortThread->setThreadName("TestSeqShort");
   BaseClass::mShortThread->setThreadPriority(Cmn::gPriorities.mCommShort);
   BaseClass::mShortThread->setThreadPrintLevel(3);

   BaseClass::mLongThread->setThreadName("TestSeqLong");
   BaseClass::mLongThread->setThreadPriority(Cmn::gPriorities.mCommLong);
   BaseClass::mLongThread->setThreadPrintLevel(3);

   // Set base class call pointers.
   BaseClass::mShortThread->mThreadInitCallPointer           = std::bind(&TTACommThread::threadInitFunction, this);
   BaseClass::mShortThread->mThreadExitCallPointer           = std::bind(&TTACommThread::threadExitFunction, this);
   BaseClass::mShortThread->mThreadExecuteOnTimerCallPointer = std::bind(&TTACommThread::executeOnTimer, this, _1);

   // Set qcalls.
   mRunTest1QCall.bind(this->mLongThread, this, &TTACommThread::executeRunTest1);
   mAbortTestQCall.bind(this->mShortThread, this, &TTACommThread::executeAbortTest);
   mSessionQCall.bind(this->mShortThread, this, &TTACommThread::executeSession);
   mRxStringQCall.bind(this->mShortThread, this, &TTACommThread::executeRxString);

   // Set member variables.
   mLoopExitCode = 0;
   mTxCount = gCProcParms.mTestCode;
   mRxCount = 0;
   mTxCode = 2;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// before the thread starts running.

void TTACommThread::threadInitFunction()
{
   using namespace std::placeholders;

   // Instance of serial port settings.
   Ris::SerialSettings tSerialSettings;

   tSerialSettings.setPortDevice("/dev/ttyO4");
   tSerialSettings.setPortSetup("38400,N,8,1");
   tSerialSettings.mRxTimeout = 0;
   tSerialSettings.mTermMode = Ris::cSerialTermMode_CRLF;
   tSerialSettings.mThreadPriority = Cmn::gPriorities.mSerialString;
   tSerialSettings.mPrintLevel = 0;
   tSerialSettings.mSessionQCall = mSessionQCall;
   tSerialSettings.mRxStringQCall = mRxStringQCall;
   tSerialSettings.m485Flag = true;

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

void TTACommThread::threadExitFunction()
{
   // Shutdown the child thread
   mSerialStringThread->shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This shuts down the two threads.

void TTACommThread::shutdownThreads()
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
void TTACommThread::executeAbortTest()
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

void TTACommThread::executeSession(bool aConnected)
{
   if (aConnected)
   {
      Prn::print(0, "TTACommThread serial port open  CONNECTED");
   }
   else
   {
      Prn::print(0, "TTACommThread serial port error DISCONNECTED");
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace