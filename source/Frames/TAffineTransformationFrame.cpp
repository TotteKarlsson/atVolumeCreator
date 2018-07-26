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

    string inputStack(stacks[0]);

    stringstream cmd;
    mScripter.setRemoteCommand("docker exec renderapps_develop python -m");
    mScripter.clearArguments();

    mScripter.addArgument("--render.host " 		        + RenderHostE->getValue());
	mScripter.addArgument("--render.port " 		        + dsl::toString(RenderHostPortE->getValue()));
	mScripter.addArgument("--render.owner " 	        + RenderOwnerE->getValue());
	mScripter.addArgument("--render.project " 	        + RenderProjectE->getValue());
	mScripter.addArgument("--render.client_scripts " 	+ RenderClientScriptsFolderE->getValue());


	mScripter.addArgument("--input_stack " 				+ inputStack);

    if(AppendToCurrentStackCB->Checked)
    {
		mScripter.addArgument("--output_stack " 	   	+ inputStack);
    }
    else
    {
		mScripter.addArgument("--output_stack " 	   	+ inputStack + "_Rotated");
    }

	mScripter.addArgument("--transformId " 				+ TransformID->getValue());
	mScripter.addArgument("--pool_size " 				+ dsl::toString(PoolSizeE->getValue()));
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

