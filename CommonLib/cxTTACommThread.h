#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsTwoThread.h"
#include "risThreadsSynch.h"
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

class TTACommThread : public Ris::Threads::BaseTwoThread
{
public:
   typedef Ris::Threads::BaseTwoThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants:

   // Wait timeouts.
   static const int cCmdAckTimeout = 2000;

   // Notification codes.
   static const int cCmdAckNotifyCode = 11;

   static const int cFlushCmdAckNotifyCode = 17;

   // Loop exit status codes.
   static const int cLoopExitNormal    = 0;
   static const int cLoopExitAborted   = 1;

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
   Ris::Threads::NotifyWrapper mCmdAckNotify;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Run test exit code.
   int mLoopExitCode;

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
   void shutdownThreads() override;

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
   // Methods. qcalls.

   // Run test qcall. It is invoked by the command line executive.
   Ris::Threads::QCall1<int> mRunTest1QCall;

   // Run test function. This is bound to the qcall. This runs the last 
   // test1 file that was generated by the file manager. It is passed a 
   // resume test flag. If the flag is false then the test is new and
   // the state should be reset and the test file reader should start at
   // the beginning of the file. if the flag is true then the test is being
   // resumed from a test suspend and the state should be restored and
   // the test reader should start where it left off when the suspend
   // ocurred. True means run the test file. False means resume the test
   // file.
   // 
   // This is used for running or resuming zip test files.
   void executeRunTest1(int aNumTx);

   // Execute test commands.
   void doRunCmd_Send();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods: QCalls: These are used to send commands to the thread.

   // Abort a running grid or test qcall.

   // The qcall. This is a call that is queued to this thread.
   Ris::Threads::QCall0 mAbortTestQCall;

   // Execute the call in the context of this thread.
   void executeAbortTest();

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
