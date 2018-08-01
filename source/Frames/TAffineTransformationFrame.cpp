#include <vcl.h>
#pragma hdrstop
#include "TAffineTransformationFrame.h"
#include <sstream>
#include "dslStringUtils.h"
#include "dslLogger.h"
#include "dslMathUtils.h"
#include "dslVCLUtils.h"
//-----------------------------------------------------------------
#pragma package(smart_init)
#pragma link "dslTIntegerLabeledEdit"
#pragma link "dslTSTDStringLabeledEdit"
#pragma link "TRenderPythonRemoteScriptFrame"
#pragma link "dslTFloatLabeledEdit"
#pragma link "dslTPropertyCheckBox"
#pragma resource "*.dfm"
TAffineTransformationFrame *AffineTransformationFrame;

using namespace dsl;
//---------------------------------------------------------------------------
__fastcall TAffineTransformationFrame::TAffineTransformationFrame(TComponent* Owner)
	: TRenderPythonRemoteScriptFrame("renderapps.stack.apply_global_affine_to_stack", Owner)
{}


//---------------------------------------------------------------------------
void __fastcall TAffineTransformationFrame::ExecuteBtnClick(TObject *Sender)
{
    //Create command string for current UI parameters
    StringList stacks = getCheckedItems(StacksForProjectCB);
    if(!stacks.count())
    {
        Log(lError) << "You must select at least one stack to transform";
        return;
    }

    stringstream cmd;
    mScripter.setRemoteCommand("docker exec renderapps_develop python -m");
    mScripter.clearArguments();

    mScripter.addArgument("--render.host " 		        + RenderHostE->getValue());
	mScripter.addArgument("--render.port " 		        + dsl::toString(RenderHostPortE->getValue()));
	mScripter.addArgument("--render.owner " 	        + RenderOwnerE->getValue());
	mScripter.addArgument("--render.project " 	        + RenderProjectE->getValue());
	mScripter.addArgument("--render.client_scripts " 	+ RenderClientScriptsFolderE->getValue());

//    stringstream stacksArg;
//    stacksArg << "[";
//
//
//    for(int i = 0; i < stacks.count(); i++)
//    {
//        stacksArg << "\"" << stacks[i] << "\"";
//        if(i < stacks.count() - 1)
//        {
//            stacksArg << ",";
//        }
//    }
//
//    stacksArg << "]";

//	mScripter.addArgument("--input_stacks " 				+ stacksArg.str());
	mScripter.addArgument("--input_stack " 				+ stacks[0]);

    if(AppendToCurrentStackCB->Checked)
    {
   		mScripter.addArgument("--output_stack " 	   	+ stacks[0]);
    }
    else
    {
		mScripter.addArgument("--output_stack " 	   	+ stacks[0] + "_Rotated");
    }

    string dateTime(getFormattedDateTimeString("Affine_%Y_%b_%d_%R:%S"));
	mScripter.addArgument("--transformId " 				+ dateTime);
	mScripter.addArgument("--pool_size " 				+ dsl::toString(stacks.count()));
	mScripter.addArgument("--B0 " 						+ dsl::toString(TranslateXE->getValue()));
	mScripter.addArgument("--B1 " 						+ dsl::toString(TranslateYE->getValue()));

    //Setup rotation matrix
    double theta(toRadians(RotationE->getValue()));
	mScripter.addArgument("--M00 " 						+ dsl::toString(cos(theta)));
	mScripter.addArgument("--M10 " 						+ dsl::toString(sin(theta)));
	mScripter.addArgument("--M01 " 						+ dsl::toString(sin(theta) * -1.0));
	mScripter.addArgument("--M11 " 						+ dsl::toString(cos(theta)));

    cmd << mScripter.createFullRemoteCommand() << endl;

    Log(lInfo) << "Remote command: " << cmd.str();
    mScripter.run();
}

//---------------------------------------------------------------------------
void __fastcall TAffineTransformationFrame::DeleteStackAExecute(TObject *Sender)
{

	if(StacksForProjectCB->ItemIndex != -1)
    {
	    //Just append _hidden to stack name
	    string currentStackName(stdstr(StacksForProjectCB->Items->Strings[StacksForProjectCB->ItemIndex]));
        Log(lInfo) << "Deleting stack: " << currentStackName;
	    string newStackName("HIDDEN_" + currentStackName);
	    mRC.renameStack(currentStackName, newStackName);

    }
    else
    {
        Log(lWarning) << "No stack is selected";
    }

}

//---------------------------------------------------------------------------
void __fastcall TAffineTransformationFrame::StacksPopupPopup(TObject *Sender)

{
    //Figure wich item was clicked
    //Only opup if an item is selected

}


