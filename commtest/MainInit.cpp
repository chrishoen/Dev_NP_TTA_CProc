#include "stdafx.h"

#include "risThreadsProcess.h"

#include "risBaseDir.h"
#include "risPortableCalls.h"
#include "smShare.h"
#include "acomCommParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize program resources.

void main_initialize(int argc,char** argv)
{
   printf("CommTest Program********************************************BEGIN\n");
   printf("CommTest Program********************************************BEGIN\n");
   printf("CommTest Program********************************************BEGIN\n\n");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Set program directory.

   // Set the base directory global variable to the cproc directory path.
   if (Ris::portableIsWindows())
   {
      Ris::setBaseDirectory("c:/aaa_prime");
   }
   else
   {
      Ris::setBaseDirectory("/opt/prime");
   }

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
   TS::setProgramName("CommTest");
   TS::setProgramPrintLevel(3);
   TS::initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************

   // Read parameters files.
   ACom::gCommParms.reset();
   ACom::gCommParms.readSection("default");

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
   Prn::resetPrint(ACom::gCommParms.mPrintViewIPAddress);
   Prn::useConsole(1);
   Prn::useConsole(2);
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::View11, true,  1);
   Prn::setFilter(Prn::View12, false, 1);
   Prn::setFilter(Prn::View13, false, 1);
   Prn::setFilter(Prn::View14, false, 1);

   Prn::setFilter(Prn::Show1, true, 1);
   Prn::setFilter(Prn::Show2, true, 2);
   Prn::setFilter(Prn::Show3, false, 1);
   Prn::setFilter(Prn::Show4, false, 1);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize program resources.

   Prn::print(Prn::View11,"CommTest Program********************************************BEGIN");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize program resourcs.

void main_finalize()
{
   // Finalize print facility.
   Prn::finalizePrint();

   // Exit process.
   Ris::Threads::exitProcess();

   // Finalize thread services.
   TS::finalize();

   // Done.
   printf("\n");
   printf("CommTest Program********************************************END\n\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
