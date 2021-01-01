#include "stdafx.h"

#include "sxMsgDefs.h"

#include "cxCProcParms.h"
#include "cxTTACommThread.h"
#include "cxDACommThread.h"
#include "cxOverrides.h"
#include "cmnPriorities.h"

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
   if (aCmd->isCmd("LOOP"))     executeLoopState(aCmd);
   if (aCmd->isCmd("OVER"))     executeOverrides(aCmd);

   if (aCmd->isCmd("GO1"))      executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))      executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))      executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))      executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))      executeGo5(aCmd);
   if (aCmd->isCmd("SHOW"))     executeParms(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeLoopState(Ris::CmdLineCmd* aCmd)
{
   CX::gDACommThread->mLoopState = SX::get_MsgId_asInt(aCmd->argString(1));
   Prn::print(0, "%s", SX::get_MsgId_asString(CX::gDACommThread->mLoopState));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeOverrides(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->numArg() == 0)
   {
      Prn::print(0, " over int float");
      Prn::print(0, "");
      Prn::print(0, " 1 mTTATemperature        float");
      Prn::print(0, " 2 mTTAMainVoltage        float");
      Prn::print(0, " 3 mTTAMainCurrent        float");
      Prn::print(0, " 4 mDATemperature         float");
      Prn::print(0, " 5 mDAMainInputVoltage    float");
      Prn::print(0, " 6 mDAMainInputCurrent    float");
      Prn::print(0, " 7 mDATowerVoltage        float");
      Prn::print(0, " 8 mDATowerCurrent        float");
      Prn::print(0, "");
      Prn::print(0, " over reset");
      Prn::print(0, " oover showver show");
      Prn::print(0, "");
      return;
   }

   if (aCmd->isArgString(1,"reset"))
   {
      CX::gOverrides.reset();
      Prn::print(0, "reset");
      return;
   }

   if (aCmd->isArgString(1, "show"))
   {
      CX::gOverrides.show();
      return;
   }

   switch (aCmd->argInt(1))
   {
   case 1: CX::gOverrides.mTTATemperature = aCmd->argFloat(2); break;
   case 2: CX::gOverrides.mTTAMainVoltage = aCmd->argFloat(2); break;
   case 3: CX::gOverrides.mTTAMainCurrent = aCmd->argFloat(2); break;
   case 4: CX::gOverrides.mDATemperature = aCmd->argFloat(2); break;
   case 5: CX::gOverrides.mDAMainInputVoltage = aCmd->argFloat(2); break;
   case 6: CX::gOverrides.mDAMainInputCurrent = aCmd->argFloat(2); break;
   case 7: CX::gOverrides.mDATowerVoltage = aCmd->argFloat(2); break;
   case 8: CX::gOverrides.mDATowerCurrent = aCmd->argFloat(2); break;
   }

   CX::gOverrides.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0, "mMainTimer.mPriority   %d", Cmn::gPriorities.mMainTimer.mPriority);
   Prn::print(0, "mMainTimer.mProcessor  %d", Cmn::gPriorities.mMainTimer.mProcessor);
   Prn::print(0, "mHLCTimer.mPriority    %d", Cmn::gPriorities.mHLCTimer.mPriority);
   Prn::print(0, "mHLCTimer.mProcessor   %d", Cmn::gPriorities.mHLCTimer.mProcessor);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
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

