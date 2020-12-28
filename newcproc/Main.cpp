#include "stdafx.h"

#include "risThreadsProcess.h"
#include "MainInit.h"
#include "MainInfo.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "cxTTACommThread.h"
#include "cxDACommThread.h"

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
      CX::gTTACommThread = new CX::TTACommThread;
      CX::gTTACommThread->launchThreads();
   }

   if (true)
   {
      CX::gDACommThread = new CX::DACommThread;
      CX::gDACommThread->launchThreads();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show program threads.

   Ris::Threads::showCurrentThreadInfo();
   if (CX::gTTACommThread)   CX::gTTACommThread->showThreadInfo();
   if (CX::gTTACommThread)   CX::gTTACommThread->mSerialStringThread->showThreadInfo();
   if (CX::gDACommThread)    CX::gDACommThread->showThreadInfo();
   if (CX::gDACommThread)    CX::gDACommThread->mSerialStringThread->showThreadInfo();

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

   if (CX::gTTACommThread)   CX::gTTACommThread->shutdownThreads();
   if (CX::gDACommThread)    CX::gDACommThread->shutdownThreads();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Delete program threads.

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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;
}
