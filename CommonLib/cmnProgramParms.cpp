//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"
#include "my_functions.h"

#include "risCmdLineFile.h"

#define  _CMNPROGRAMPARMS_CPP_
#include "cmnProgramParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Cmn
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

ProgramParms::ProgramParms()
{
   reset();
}

void ProgramParms::reset()
{
   BaseClass::reset();
   BaseClass::setFilePath_RelativeToBaseDir("files/ProgramParms.txt");
   BaseClass::setFilePath("/opt/prime/files/ProgramParms.txt");

   for (int i = 0; i < 5; i++)
   {
      mDeviceName[0][0] = 0;
   }

   mEnableAll = false;
   mMainACM = 0;
   mDelay = 0;
   mPrintViewEnable = false;
   mPrintViewIPAddress[0] = 0;
   mSerialStringPrintLevel = 0;
   mCommSeqShortPrintLevel = 0;
   mCommSeqLongPrintLevel = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void ProgramParms::show()
{
   printf("\n");
   printf("ProgramParms******************************************* %s\n", mTargetSection);

   printf("\n");
   for (int i = 0; i < 5; i++)
   {
   printf("DeviceName%d              %-12s\n", i, mDeviceName[i]);
   }

   printf("\n");
   printf("EnableAll                 %-12s\n", my_string_from_bool(mEnableAll));
   printf("MainACM                   %12d\n",  mMainACM);
   printf("Delay                     %12d\n",  mDelay);

   printf("\n");
   printf("PrintViewEnable           %-12s\n",  my_string_from_bool(mPrintViewEnable));
   printf("PrintViewIPAddress        %-12s\n",  mPrintViewIPAddress);

   printf("\n");
   printf("SerialStringPrintLevel    %12d\n", mSerialStringPrintLevel);
   printf("CommSeqShortPrintLevel    %12d\n", mCommSeqShortPrintLevel);
   printf("CommSeqLongPrintLevel     %12d\n", mCommSeqLongPrintLevel);

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void ProgramParms::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("DeviceName0"))            aCmd->copyArgString(1, mDeviceName[0], cMaxStringSize);
   if (aCmd->isCmd("DeviceName1"))            aCmd->copyArgString(1, mDeviceName[1], cMaxStringSize);
   if (aCmd->isCmd("DeviceName2"))            aCmd->copyArgString(1, mDeviceName[2], cMaxStringSize);
   if (aCmd->isCmd("DeviceName3"))            aCmd->copyArgString(1, mDeviceName[3], cMaxStringSize);
   if (aCmd->isCmd("DeviceName4"))            aCmd->copyArgString(1, mDeviceName[4], cMaxStringSize);

   if (aCmd->isCmd("SerialStringPrintLevel")) mSerialStringPrintLevel = aCmd->argInt(1);
   if (aCmd->isCmd("CommSeqShortPrintLevel")) mCommSeqShortPrintLevel = aCmd->argInt(1);
   if (aCmd->isCmd("CommSeqLongPrintLevel"))  mCommSeqLongPrintLevel = aCmd->argInt(1);

   if (aCmd->isCmd("EnableAll"))              mEnableAll = aCmd->argBool(1);
   if (aCmd->isCmd("MainACM"))                mMainACM = aCmd->argInt(1);
   if (aCmd->isCmd("Delay"))                  mDelay = aCmd->argInt(1);

   if (aCmd->isCmd("PrintViewEnable"))        mPrintViewEnable = aCmd->argBool(1);
   if (aCmd->isCmd("PrintViewIPAddress"))     aCmd->copyArgString(1, mPrintViewIPAddress, cMaxStringSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void ProgramParms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace