#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risThreadsNotify.h"
#include "risSerialStringThread.h"

#include "sxTTATxMsgProc.h"
#include "sxTTARxMsgProc.h"

namespace CX
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an example master thread that sends commands to a slave thread
// and waits for responses.

class TTACommThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants:

   // Timer periods..
   static const int cSlowTimerPeriod = 2000;
   static const int cFastTimerPeriod = 2000;

   // Wait timeouts.
   static const int cCmdAckTimeout = 2000;

   // Notification codes.
   static const int cCmdAckNotifyCode = 11;

   static const int cFlushCmdAckNotifyCode = 17;

   // Loop exit status codes.
   static const int cProcExitNormal    = 0;
   static const int cProcExitAborted   = 1;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial  string port child thread. It provides the thread execution
   // context for a serial string port and uses it to provide string
   // communication.
   Ris::SerialStringThread* mSerialStringThread;

   // If true then the serial port is open.
   bool mConnectionFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Notifications.
   Ris::Threads::Notify mNotify;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Run test exit code.
   int mProcExitCode;

   // Tx message code.
   int mTxCode;

   SX::TTATxMsgProc mTxMsgProc;
   SX::TTARxMsgProc mRxMsgProc;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Metrics.
   int mTxCount;
   int mRxCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   TTACommThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immediately 
   // before the thread starts running.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immediately 
   // after the thread starts running.
   void threadExitFunction() override;

   // Thread shutdown function. This shuts down the two threads.
   void shutdownThread() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Inheritor overloads.

   // This is called periodically by the base class threadRunFunction.
   void executeOnTimer(int aTimerCount) override;

   // Send a request to the slave, wait for the response and process it.
   // Return true if successful. This is called by the above execute on
   // timer, based on the state.
   bool doProcess_gcs(int aTimerCount);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. 

   // Session callback qcall.
   Ris::Threads::QCall1<bool> mSessionQCall;

   // Session qcall function. This is invoked by the child thread when 
   // the serial port is closed because of an error or when it is reopened
   // correctly.
   void executeSession(bool aConnected);

   // Receive string callback qcall.
   Ris::Threads::QCall1<std::string*> mRxStringQCall;

   // Receive string qcall function. This is invoked by the child thread 
   // when a string is received and it processes the received string
   // in the context of the short thread.
   void executeRxString(std::string* aString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a null terminated string via the serial port. A newline terminator
   // is appended to the string before transmission. This executes in the
   // context of the The calling thread.
   void sendString(const char* aString);

   // Send a null terminated string via the serial port. A newline terminator
   // is appended to the string before transmission. This executes in the
   // context of the The calling thread. The string is deleted after
   // transmission.
   void sendString(std::string* aString);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef    _CXTTACOMMTHREAD_CPP_
          TTACommThread* gTTACommThread = 0;
#else
   extern TTACommThread* gTTACommThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
