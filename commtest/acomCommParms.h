#pragma once

/*==============================================================================
Prototype communications message classes.
Parameters class whose values are read from a command file. 
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risCmdLineParms.h"
#include "tsDefs.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ACom
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a class that contains parameter member variables. The values of
// the parameters are set by reading a text file that contains command lines.
// Each command line is of the form "command argument1 argument2 ...".
// 
// The command files are partitioned into different sections and only one
// section can be read at a time to set member variables that are specified
// in it.
//
// The command files are managed by a CmdLineFile object. This opens the 
// file, reads each line in it, parses the line into a CmdLineCmd command 
// object, passes the command object to this object for command execution,
// and then closes the file. 
//
// This class inherits from BaseCmdLineParms, which inherits from 
// BaseCmdLineExec. BaseCmdLineParms provides a method that uses a
// CmdLineFile object to read and process the file. BaseCmdLineExec provides
// an abstract execute(cmd) method to which inheritors provide an overload
// that is called by the CmdLineFile object for each command in the file.
// This execute method then sets a member variables, according to the
// command.
// 
// This class can contain member variables that also inherit from
// BaseCmdLineExec. This provides for command files that have a nested
// structure. If so, then this class is the root.
// 

class CommParms : public Ris::BaseCmdLineParms
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 100;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // PortDevice: "COM1".
   // PortSetup:  "9600,N,8,1", If empty then use system defaults.
   // Port receive timeout ms, zero is infinite.
   char mCmdCommPortDevice[cMaxStringSize];
   char mCmdCommPortSetup[cMaxStringSize];
   int  mCmdCommPortTimeout;
   bool mCmdCommPort485Flag;

   // Thread print and log levels.
   int mSerialStringPrintLevel;
   int mTestSeqShortPrintLevel;
   int mTestSeqLongPrintLevel;

   // Print view ip address.
   char mPrintViewIPAddress[30];


   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Delay between transmits.
   int mDelay;

   // Command to transmit.
   char mTxCommand[cMaxStringSize];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Expanded members that are not read from the parms file.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infrastucture.

   // Constructor,
   typedef Ris::BaseCmdLineParms BaseClass;
   CommParms();
   void reset();
   void show();

   // Base class override: Execute a command from the command file to set a 
   // member variable. This is called by the associated command file object
   // for each command in the file.
   void execute(Ris::CmdLineCmd* aCmd) override;

   // Calculate expanded member variables. This is called after the entire
   // section of the command file has been processed.
   void expand() override;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _ACOMCOMMPARMS_CPP_
   CommParms gCommParms;
#else
   extern CommParms gCommParms;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace