#include "stdafx.h"

#include "risThreadsProcess.h"

#include "risBaseDir.h"
#include "risPortableCalls.h"
#include "smShare.h"
#include "cxCProcParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize program resources.

void main_initialize(int argc,char** argv)
{
   printf("NewCProc Program********************************************BEGIN\n");
   printf("NewCProc Program********************************************BEGIN\n");
   printf("NewCProc Program********************************************BEGIN\n\n");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Set program directory.

   // Set the base directory global variable to the cproc directory path.
   Ris::setBaseDirectory("/opt/prime");

   // Set the program working directory to the cproc directory path.
   Ris::chdirToBaseDirectory();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Enter process.

   // Set program process for high priority.
   Ris::Threads::enterProcessHigh();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize thread services.

   TS::reset();
   TS::setProgramName("NewCProc");
   TS::setProgramPrintLevel(3);
   TS::initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************

   // Read parameters files.
   CX::gCProcParms.reset();
   CX::gCProcParms.readSection("default");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize program resources.

   // Initialize shared memory.
   SM::initializeShare();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize print facility.

   // Initialize print.
   Prn::resetPrint(CX::gCProcParms.mPrintViewIPAddress);
   Prn::useConsole(1);
   Prn::useConsole(2);
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::TTA1, false, 1);
   Prn::setFilter(Prn::TTA2, false, 2);
   Prn::setFilter(Prn::TTA3, false, 1);
   Prn::setFilter(Prn::TTA4, false, 1);
   Prn::setFilter(Prn::TTA5, true, 1);
   Prn::setFilter(Prn::TTA6, true, 2);
   Prn::setFilter(Prn::TTA7, false, 1);
   Prn::setFilter(Prn::TTA8, false, 1);

   Prn::setFilter(Prn::DA1, false, 1);
   Prn::setFilter(Prn::DA2, false, 2);
   Prn::setFilter(Prn::DA3, false, 1);
   Prn::setFilter(Prn::DA4, false, 1);
   Prn::setFilter(Prn::DA5, true, 1);
   Prn::setFilter(Prn::DA6, true, 2);
   Prn::setFilter(Prn::DA7, false, 1);
   Prn::setFilter(Prn::DA8, false, 1);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize program resources.

   Prn::print(Prn::TTA1, "NewCProc Program********************************************BEGIN");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize program resourcs.

void main_finalize()
{
   Prn::print(Prn::TTA1, "NewCProc Program********************************************END");

   // Finalize print facility.
   Prn::finalizePrint();

   // Exit process.
   Ris::Threads::exitProcess();

   // Finalize thread services.
   TS::finalize();

   // Done.
   printf("\n");
   printf("NewCProc Program********************************************END\n\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
