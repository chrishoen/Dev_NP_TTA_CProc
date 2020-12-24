//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "evtMsgDefs.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Evt
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return an event record variable as a string.

char* get_EvtType_asString(int aType)
{
   switch (aType)
   {
   case cEvt_Type1: return "Type1";
   case cEvt_Type2: return "Type2";
   }
   return "UNKNOWN";
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return an event record variable as a string.

char* get_EvtSeverity_asString(int aSeverity)
{
   switch (aSeverity)
   {
   case cEvt_SeverityUseDefault: return "UseDefault";
   case cEvt_SeverityInfo: return "Info";
   case cEvt_SeverityWarning: return "Warning";
   case cEvt_SeveritySevere: return "Severe";
   case cEvt_SeverityCritical: return "Critical";
   }
   return "UNKNOWN";
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return true if the event can cause an alarm.

bool isAlarmable(int aSeverity)
{
   switch (aSeverity)
   {
   case cEvt_SeverityUseDefault: return false;
   case cEvt_SeverityInfo: return false;
   case cEvt_SeverityWarning: return false;
   case cEvt_SeveritySevere: return true;
   case cEvt_SeverityCritical: return true;
   }
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a partition code as a string.

char* get_EvtSource_asString(int aSource)
{
   switch (aSource)
   {
   case cEvt_SourceTest: return "Test";
   case cEvt_SourceTTA: return "TTA";
   case cEvt_SourceDA: return "DA";
   case cEvt_SourceCU: return "CU";
   case cEvt_SourceHLC: return "HLC";
   case cEvt_SourceAux: return "Aux";
   }
   return "UNKNOWN";
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get the timestamp as a string. This is passed in a local buffer.

char* get_timespec_asString(timespec aTimeSpec, char* aBuffer)
{
   char tTemp[40];
   strftime(tTemp, 40, "%F %T", localtime(&aTimeSpec.tv_sec));
   sprintf(aBuffer, "%s.%03ld", tTemp, aTimeSpec.tv_nsec / 1000000);

   return aBuffer;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
