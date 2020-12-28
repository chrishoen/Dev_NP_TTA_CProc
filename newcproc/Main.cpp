#include "stdafx.h"

#include "risThreadsProcess.h"
#include "MainInit.h"
#include "MainInfo.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "evtEventLogThread.h"
#include "cxTTACommThread.h"
#include "cxDACommThread.h"
#include "cxMainTimerThread.h"
#include "cxHLCTimerThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Begin program.

   main_initialize(argc, argv);
   main_info_initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   if (true)
   {
      Evt::gEventLogThread = new Evt::EventLogThread;
      Evt::gEventLogThread->launchThread();
   }

   if (true)
   {
      CX::gTTACommThread = new CX::TTACommThread;
      CX::gTTACommThread->launchThreads();
   }

   if (true)
   {
      CX::gDACommThread = new CX::DACommThread;
      CX::gDACommThread->launchThreads();
   }

   if (true)
   {
      CX::gMainTimerThread = new CX::MainTimerThread;
      CX::gMainTimerThread->launchThread();
   }

   if (true)
   {
      CX::gHLCTimerThread = new CX::HLCTimerThread;
      CX::gHLCTimerThread->launchThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show program threads.

   Ris::Threads::showCurrentThreadInfo();
   if (Evt::gEventLogThread)   Evt::gEventLogThread->showThreadInfo();
   if (CX::gTTACommThread)     CX::gTTACommThread->showThreadInfo();
   if (CX::gTTACommThread)     CX::gTTACommThread->mSerialStringThread->showThreadInfo();
   if (CX::gDACommThread)      CX::gDACommThread->showThreadInfo();
   if (CX::gDACommThread)      CX::gDACommThread->mSerialStringThread->showThreadInfo();
   if (CX::gMainTimerThread)   CX::gMainTimerThread->showThreadInfo();
   if (CX::gHLCTimerThread)    CX::gHLCTimerThread->showThreadInfo();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Execute console command line executive, wait for user to exit.

   CmdLineExec* tExec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(tExec);
   delete tExec;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Shutdown program threads.

   if (Evt::gEventLogThread)     Evt::gEventLogThread->shutdownThread();
   if (CX::gTTACommThread)       CX::gTTACommThread->shutdownThreads();
   if (CX::gDACommThread)        CX::gDACommThread->shutdownThreads();
   if (CX::gMainTimerThread)     CX::gMainTimerThread->shutdownThread();
   if (CX::gHLCTimerThread)      CX::gHLCTimerThread->shutdownThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Delete program threads.

   if (Evt::gEventLogThread)
   {
      delete Evt::gEventLogThread;
      Evt::gEventLogThread = 0;
   }

   if (CX::gTTACommThread)
   {
      delete CX::gTTACommThread;
      CX::gTTACommThread = 0;
   }

   if (CX::gDACommThread)
   {
      delete CX::gDACommThread;
      CX::gDACommThread = 0;
   }

   if (CX::gMainTimerThread)
   {
      delete CX::gMainTimerThread;
      CX::gMainTimerThread = 0;
   }

   if (CX::gHLCTimerThread)
   {
      delete CX::gHLCTimerThread;
      CX::gHLCTimerThread = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;
}
