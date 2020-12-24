//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"
#include "my_functions.h"

#include "risCmdLineFile.h"

#define  _ACOMCOMMPARMS_CPP_
#include "acomCommParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ACom
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

CommParms::CommParms()
{
   reset();
}

void CommParms::reset()
{
   BaseClass::reset();
   if (Ris::portableIsWindows())
   {
      BaseClass::setFilePath("C:/aaa_prime/CommTest/Comm_Parms.txt");
   }
   else
   {
      BaseClass::setFilePath_RelativeToBaseDir("files/Comm_Parms.txt");
   }

   mCmdCommPortDevice[0] = 0;
   mCmdCommPortSetup[0] = 0;
   mCmdCommPortTimeout = 0;
   mCmdCommPort485Flag = false;
   mDelay = 0;
   mTxCommand[0] = 0;
   mSerialStringPrintLevel = 0;
   mTestSeqShortPrintLevel = 0;
   mTestSeqLongPrintLevel = 0;
   mPrintViewIPAddress[0] = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void CommParms::show()
{
   printf("\n");
   printf("CommParms******************************************* %s\n", mTargetSection);

   printf("\n");
   printf("CmdCommPortDevice       %-12s\n", mCmdCommPortDevice);
   printf("CmdCommPortSetup        %-12s\n", mCmdCommPortSetup);
   printf("CmdCommPortTimeout      %12d\n",  mCmdCommPortTimeout);
   printf("CmdCommPort485Flag      %12s\n",  my_string_from_bool(mCmdCommPort485Flag));

   printf("\n");
   printf("SerialStringPrintLevel  %12d\n",  mSerialStringPrintLevel);
   printf("TestSeqShortPrintLevel  %12d\n",  mTestSeqShortPrintLevel);
   printf("TestSeqLongPrintLevel   %12d\n",  mTestSeqLongPrintLevel);

   printf("\n");
   printf("Delay                   %12d\n", mDelay);
   printf("TxCommand               %12s\n", mTxCommand);
   printf("PrintViewIPAdds         %-12s\n", mPrintViewIPAddress);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void CommParms::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("CmdCommPortDevice"))      aCmd->copyArgString(1, mCmdCommPortDevice, cMaxStringSize);
   if (aCmd->isCmd("CmdCommPortSetup"))       aCmd->copyArgString(1, mCmdCommPortSetup, cMaxStringSize);
   if (aCmd->isCmd("CmdCommPortTimeout"))     mCmdCommPortTimeout = aCmd->argInt(1);
   if (aCmd->isCmd("CmdCommPort485Flag"))     mCmdCommPort485Flag = aCmd->argBool(1);

   if (aCmd->isCmd("SerialStringPrintLevel")) mSerialStringPrintLevel = aCmd->argInt(1);
   if (aCmd->isCmd("TestSeqShortPrintLevel")) mTestSeqShortPrintLevel = aCmd->argInt(1);
   if (aCmd->isCmd("TestSeqLongPrintLevel"))  mTestSeqLongPrintLevel = aCmd->argInt(1);

   if (aCmd->isCmd("Delay"))                  mDelay = aCmd->argInt(1);
   if (aCmd->isCmd("TxCommand"))              aCmd->copyArgString(1, mTxCommand, cMaxStringSize);
   if (aCmd->isCmd("PrintViewIPAddress"))     aCmd->copyArgString(1, mPrintViewIPAddress, cMaxStringSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void CommParms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace