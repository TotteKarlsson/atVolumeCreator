#include <vcl.h>
#pragma hdrstop
#include "TMainForm.h"
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
#include "atRenderService.h"
#include "atROIHistory.h"
using namespace mtk;


void __fastcall TMainForm::logMsg()
{
    infoMemo->Lines->Add(vclstr(mLogFileReader.getData()));
    mLogFileReader.purge();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	mLogFileReader.start(true);
    mOriginalRB.X 		= mXCoord->getValue();
    mOriginalRB.Y 		= mYCoord->getValue();
    mOriginalRB.Width 	= mWidth->getValue();
    mOriginalRB.Height	= mHeight->getValue();
    mCurrentRB = mOriginalRB;
    mROIHistory.add(mOriginalRB);
	UpdateZList();
	ClickZ(NULL);
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

    Close();
}

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    IdHTTP1->Disconnect();
	Log(lInfo) << "In FormClose";
//	mIniFileC->clear();
	Log(lInfo) << "In main forms destructor";

//	//Save project history
//	mBottomPanelHeight          	= BottomPanel->Height;
//    mDustAssayResultImageHeight 	= mResultImagePanel->Height;
//    mDustAssayBackGroundImageWidth 	= mBackgroundImagePanel->Width;
//
//	mGeneralProperties.write();
//
//	//Write to file
//	mIniFileC->save();
//
//	//Registry settings
//	mSplashProperties.write();
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

	if(CanClose == false)
	{
		mShutDownTimer->Enabled = true;
	}
}

