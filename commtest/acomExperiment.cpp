/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "acomCommParms.h"

#define  _ACOMEXPERIMENT_CPP_
#include "acomExperiment.h"

namespace ACom
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Experiment::Experiment()
{
   mTxCount = 0;
   mRxCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// before the thread starts running.

void Experiment::initialize()
{
   // Instance of serial port settings.
   Ris::SerialSettings tSerialSettings;

   tSerialSettings.setPortDevice(gCommParms.mCmdCommPortDevice);
   tSerialSettings.setPortSetup(gCommParms.mCmdCommPortSetup);
   tSerialSettings.mRxTimeout = gCommParms.mCmdCommPortTimeout;
   tSerialSettings.mTermMode = Ris::cSerialTermMode_CRLF;
   tSerialSettings.m485Flag = true;

   mSerialPort.initialize(tSerialSettings);
   mSerialPort.doOpen();
}

void Experiment::finalize()
{
   mSerialPort.doClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. A newline terminator
// is appended to the string before transmission. This executes in the
// context of the The calling thread.

void Experiment::sendString(const char* aString)
{
   // Metrics.
   mTxCount++;

   // Print the string.
   Prn::print(Prn::Show1, ">>>> %s", aString);

   // Send the string.
   mSerialPort.doSendLine(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. A newline terminator
// is appended to the string before transmission. This executes in the
// context of the The calling thread. The string is deleted after
// transmission.

void Experiment::sendString(std::string* aString)
{
   // Metrics.
   mTxCount++;

   // Print the string.
   Prn::print(Prn::Show1, ">>>> %s", aString->c_str());

   // Send the string.
   mSerialPort.doSendLine(aString->c_str());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace