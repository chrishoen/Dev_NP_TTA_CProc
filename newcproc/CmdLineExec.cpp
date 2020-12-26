#include "stdafx.h"

#include "sxTTATxMsgProc.h"
#include "sxMsgDefs.h"

#include "cxCProcParms.h"
#include "cxTTACommThread.h"

#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
}

void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class is the program command line executive. It processes user
// command line inputs and executes them. It inherits from the command line
// command executive base class, which provides an interface for executing
// command line commands. It provides an override execute function that is
// called by a console executive when it receives a console command line input.
// The execute function then executes the command.

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("RESET"))    reset();
   if (aCmd->isCmd("TEST"))     executeTest(aCmd);
   if (aCmd->isCmd("A"))        executeAbort(aCmd);
   if (aCmd->isCmd("TXCODE"))   executeTxCode(aCmd);

   if (aCmd->isCmd("GO1"))      executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))      executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))      executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))      executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))      executeGo5(aCmd);
   if (aCmd->isCmd("PARMS"))    executeParms(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest(Ris::CmdLineCmd* aCmd)
{
   CX::gCProcParms.reset();
   CX::gCProcParms.readSection("default");

   aCmd->setArgDefault(1, 1);
   CX::gTTACommThread->mRunTest1QCall(aCmd->argInt(1));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTxCode(Ris::CmdLineCmd* aCmd)
{
   CX::gTTACommThread->mTxCode = SX::get_MsgId_asInt(aCmd->argString(1));
   Prn::print(0, "%s", SX::get_MsgId_asString(CX::gTTACommThread->mTxCode));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeAbort(Ris::CmdLineCmd* aCmd)
{
   CX::gTTACommThread->mAbortTestQCall();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   CX::gCProcParms.reset();
   CX::gCProcParms.readSection("default");

   aCmd->setArgDefault(1, 1);
   CX::gTTACommThread->mRunTest1QCall(10000000);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   SX::TTATxMsgProc tTxProc;
   tTxProc.buildMsg(SX::cMsgId_gsx, "abcdefg");
   Prn::print(0, "%s", tTxProc.mTxBuffer);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   CX::gCProcParms.reset();
   CX::gCProcParms.readSection("default");
   CX::gCProcParms.show();
}

