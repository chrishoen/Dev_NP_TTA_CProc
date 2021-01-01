//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risFileFunctions.h"
#include "smSynch.h"
#include "SuperStateDefs.h"

#define  _CPROCINFO_CPP_
#include "CProcInfo.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

CProcInfo::CProcInfo()
{
   initialize();
}

void CProcInfo::initialize()
{
   mValidFlag = false;

   strcpy(mFilePath, "/opt/files/cprocinfo.json");
   strcpy(mLockFileName, "cprocinfo");

   mTTAPreferRFPath = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

// Lock the json file and read from it.
void CProcInfo::doReadModifyWriteBegin()
{
   SM::gSynch.mCProcInfoLockFile = Ris::doLockFile_OpenAndLock(mLockFileName);
   doRead();
}

// Perform the calculation, write to the json file, and unlock it.
void CProcInfo::doReadModifyWriteEnd()
{
   doWrite();
   Ris::doLockFile_UnlockAndClose(SM::gSynch.mCProcInfoLockFile);
}

// Lock the json file, read from it, and unlock it.
void CProcInfo::doProtectedRead()
{
   SM::gSynch.mCProcInfoLockFile = Ris::doLockFile_OpenAndLock(mLockFileName);
   doRead();
   Ris::doLockFile_UnlockAndClose(SM::gSynch.mCProcInfoLockFile);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a json value for all of the variables.

Json::Value CProcInfo::getJsonValue()
{
   Json::Value tAlarmEnable;
   Json::Value tInput;

   // Json variables.
   Json::Value tJsonValue;
   char tBuffer[40];

   // Copy members.
   tJsonValue["TTAPreferRFPath"] = mTTAPreferRFPath;

   // Done.
   return tJsonValue;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a string with json for all of the variables.

std::string CProcInfo::getJsonString()
{
   // If this is not valid then read from the json file.
   if (!mValidFlag)
   {
      doProtectedRead();
   }

   // Json variable.
   Json::Value tJsonValue = getJsonValue();

   std::string tString;
   Json::FastWriter tWriter;
   tString = tWriter.write(tJsonValue);
   return tString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the json file.

void CProcInfo::doWrite()
{
   // Json variable.
   Json::Value tJsonValue = getJsonValue();

   // Write to json file.
   Ris::doWriteJsonToFile(
      tJsonValue,           // Input
      mFilePath);           // Input
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read from the json file.

void CProcInfo::doRead()
{
   // Json variables.
   Json::Value tJsonValue;

   // If the json file doesn't exist.
   if (!Ris::portableFilePathExists(mFilePath))
   {
      // Write to the file, so that it exists.
      initialize();
      doWrite();
      mValidFlag = true;
      return;
   }

   try
   {
      // Read from json file.
      Ris::doReadJsonFromFile(
         tJsonValue,           // Output
         mFilePath);           // Input

      // Copy members.
      mTTAPreferRFPath = tJsonValue["TTAPreferRFPath"].asInt();
   }
   catch (...)
   {
   }

   mValidFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set variable.

void CProcInfo::setTTAPreferRFPath(int aTTAPreferRFPath)
{
   // Set the variable.
   mTTAPreferRFPath = aTTAPreferRFPath;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void CProcInfo::show(int aPF)
{
   if (!Prn::getFilter(aPF)) return;
   Prn::print(aPF, "");
   Prn::print(aPF, "FilePath %s", mFilePath);

   Prn::print(aPF, "tTTAPreferRFPath     %s", get_TTA_RFPath_asString(mTTAPreferRFPath));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
