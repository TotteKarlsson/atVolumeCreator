#include <vcl.h>
#pragma hdrstop
#include "TMainForm.h"
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
#include "TImageForm.h"
using namespace mtk;

//---------------------------------------------------------------------------
void __fastcall TMainForm::Exit1Click(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mShutDownTimerTimer(TObject *Sender)
{
	mShutDownTimer->Enabled = false;

	if(mLogFileReader.isRunning())
	{
		Log(lDebug) << "Shutting down log file reader";
		mLogFileReader.stop();
	}

	if(TSSHFrame1->isConnected())
    {
		TSSHFrame1->disconnect();
	}

    Close();
}

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    IdHTTP1->Disconnect();
	Log(lInfo) << "In FormClose";
	mIniFileC->clear();
	Log(lInfo) << "In main forms destructor";

	if(gImageForm)
    {
		gImageForm->mPrepareForDeletion = true;
    	gImageForm->Close();
    }

	//Save project history
	mBottomPanelHeight          	= mBottomPanel->Height;

	mGeneralProperties.write();

	//Write to file
	mIniFileC->save();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	Log(lInfo) << "Closing down....";

	//Check if we can close.. abort all threads..
	if(mLogFileReader.isRunning())
    {
    	CanClose = false;
    }

	if(TSSHFrame1->isConnected())
    {
		CanClose = false;
    }

	if(CanClose == false)
	{
		mShutDownTimer->Enabled = true;
	}
}

