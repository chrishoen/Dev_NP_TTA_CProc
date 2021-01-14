//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"

#define  _CXCPROCPARMS_CPP_
#include "cxCProcParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

CProcParms::CProcParms()
{
   reset();
}

void CProcParms::reset()
{
   BaseClass::reset();
   BaseClass::setFilePath("/opt/files/CProc_Parms.txt");

   mAuxOverrideEnable = false;
   mHLCOverrideEnable = false;
   mHLCSimSinEnable = false;
   mHLCSimSinPeriod = 10;
   mHLCSimSinAmpHi = -50;
   mHLCSimSinAmpLo = -55;

   mHLCOffsetESS = 0;
   mHLCOffsetSA = 0;
   mHLCThreshLo = 0;

   mHLCTimerPeriod = 0;
   mHLCStepTime = 0;

   mTestCode = 0;
   mPrintViewEnable = false;
   mPrintViewIPAddress[0] = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void CProcParms::show()
{
   printf("\n");
   printf("CProcParms************************************************ %s\n", mTargetSection);

   printf("\n");
   printf("AuxOverrideEnable          %-10s\n", my_string_from_bool(mAuxOverrideEnable));
   printf("HLCOverrideEnable          %-10s\n", my_string_from_bool(mHLCOverrideEnable));
   printf("HLCSimSinEnable            %-10s\n", my_string_from_bool(mHLCSimSinEnable));
   printf("HLCSimSinPeriod            %-10.2f\n", mHLCSimSinPeriod);
   printf("HLCSimSinAmpHi             %-10.2f\n", mHLCSimSinAmpHi);
   printf("HLCSimSinAmpLo             %-10.2f\n", mHLCSimSinAmpLo);
   printf("\n");
   printf("HLCOffsetESS               %-10.2f\n", mHLCOffsetESS);
   printf("HLCOffsetSA                %-10.2f\n", mHLCOffsetSA);
   printf("HLCThreshLo                %-10.2f\n", mHLCThreshLo);
   printf("\n");
   printf("HLCTimerPeriod             %-10d\n",   mHLCTimerPeriod);
   printf("HLCStepTime                %-10.2f\n", mHLCStepTime);
   printf("\n");
   printf("TestCode                   %-10d\n", mTestCode);
   printf("\n");
   printf("PrintViewEnable            %-12s\n", my_string_from_bool(mPrintViewEnable));
   printf("PrintViewIPAddress         %-12s\n", mPrintViewIPAddress);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void CProcParms::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("AuxOverrideEnable"))       mAuxOverrideEnable = aCmd->argBool(1);

   if (aCmd->isCmd("HLCOverrideEnable"))       mHLCOverrideEnable = aCmd->argBool(1);
   if (aCmd->isCmd("HLCSimSinEnable"))         mHLCSimSinEnable = aCmd->argBool(1);
   if (aCmd->isCmd("HLCSimSinPeriod"))         mHLCSimSinPeriod = aCmd->argDouble(1);
   if (aCmd->isCmd("HLCSimSinAmpHi"))          mHLCSimSinAmpHi = aCmd->argDouble(1);
   if (aCmd->isCmd("HLCSimSinAmpLo"))          mHLCSimSinAmpLo = aCmd->argDouble(1);

   if (aCmd->isCmd("HLCOffsetESS"))            mHLCOffsetESS = aCmd->argDouble(1);
   if (aCmd->isCmd("HLCOffsetSA"))             mHLCOffsetSA = aCmd->argDouble(1);
   if (aCmd->isCmd("HLCThreshLo"))             mHLCThreshLo = aCmd->argDouble(1);

   if (aCmd->isCmd("HLCTimerPeriod"))          mHLCTimerPeriod = aCmd->argInt(1);
   if (aCmd->isCmd("HLCStepTime"))             mHLCStepTime = aCmd->argDouble(1);

   if (aCmd->isCmd("TestCode"))                mTestCode = aCmd->argInt(1);
   if (aCmd->isCmd("PrintViewEnable"))         mPrintViewEnable = aCmd->argBool(1);
   if (aCmd->isCmd("PrintViewIPAddress"))      aCmd->copyArgString(1, mPrintViewIPAddress, cMaxStringSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void CProcParms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace