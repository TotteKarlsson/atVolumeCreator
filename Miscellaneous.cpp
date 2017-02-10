#include <vcl.h>
#pragma hdrstop
#include "TMainForm.h"
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
#include "atRenderClient.h"
#include "atROIHistory.h"

using namespace mtk;
extern string gAppName;
void __fastcall TMainForm::logMsg()
{
    infoMemo->Lines->Add(vclstr(mLogFileReader.getData()));
    mLogFileReader.purge();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	gLogger.setLogLevel(mLogLevel);
	mLogFileReader.start(true);
    mOriginalRB.setX1(mXCoordE->getValue());
    mOriginalRB.setY1(mYCoordE->getValue());
    mOriginalRB.setWidth(mWidthE->getValue());
    mOriginalRB.setHeight(mHeightE->getValue());
    mCurrentRB = mOriginalRB;
    mROIHistory.add(mOriginalRB);
	UpdateZList();
	ClickZ(NULL);
    this->DoubleBuffered=true;
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
	mIniFileC->clear();
	Log(lInfo) << "In main forms destructor";

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

	if(CanClose == false)
	{
		mShutDownTimer->Enabled = true;
	}
}


void TMainForm::setupIniFile()
{
	string fldr = getSpecialFolder(CSIDL_LOCAL_APPDATA);
	fldr =  joinPath(fldr, gAppName);

	if(!folderExists(fldr))
	{
		CreateDir(fldr.c_str());
	}

	mIniFileC->init(fldr);

	//For convenience and for option form, populate appProperties container
	mAppProperties.append(&mGeneralProperties);

}

bool TMainForm::setupAndReadIniParameters()
{
	mIniFileC->load();
	mGeneralProperties.setIniFile(mIniFileC->getIniFile());

	//Setup parameters
	mGeneralProperties.setSection("GENERAL");
	mGeneralProperties.add((BaseProperty*)  &mBottomPanelHeight.setup( 	            	"HEIGHT_OF_BOTTOM_PANEL",    	    205));
	mGeneralProperties.add((BaseProperty*)  &mLogLevel.setup( 	                    	"LOG_LEVEL",    	                lAny));

    mGeneralProperties.add((BaseProperty*)  &mBaseUrlE->getProperty()->setup(	        "BASE_URL", 	"http://ibs-forrestc-ux1.corp.alleninstitute.org:8080/render-ws/v1/owner"));
    mGeneralProperties.add((BaseProperty*)  &mOwnerE->getProperty()->setup(		        "OWNER", 		"Sharmishtaas"));
    mGeneralProperties.add((BaseProperty*)  &mProjectE->getProperty()->setup(	        "PROJECT", 		"M270907_Scnn1aTg2Tdt_13"));
    mGeneralProperties.add((BaseProperty*)  &mStackNameE->getProperty()->setup(	        "STACK_NAME", 	"ALIGNEDSTACK_DEC12"));

    mGeneralProperties.add((BaseProperty*)  &mScaleE->getProperty()->setup(		        "SCALE", 		0.2));
    mGeneralProperties.add((BaseProperty*)  &mXCoordE->getProperty()->setup(	        "VIEW_X_COORD",    	0));
    mGeneralProperties.add((BaseProperty*)  &mYCoordE->getProperty()->setup(	        "VIEW_Y_COORD",    	0));
    mGeneralProperties.add((BaseProperty*)  &mWidthE->getProperty()->setup(		        "VIEW_WIDTH", 		0));
    mGeneralProperties.add((BaseProperty*)  &mHeightE->getProperty()->setup(	        "VIEW_HEIGHT", 		0));

    mGeneralProperties.add((BaseProperty*)  &mZMinE->getProperty()->setup(		        "Z_MIN", 		0));
    mGeneralProperties.add((BaseProperty*)  &mZMaxE->getProperty()->setup(		        "Z_MAX", 		100));

    mGeneralProperties.add((BaseProperty*)  &mCustomZsE->getProperty()->setup(	        "CUSTOM_Zs",  	""));
	mGeneralProperties.add((BaseProperty*)  &mImageCacheFolderE->getProperty()->setup(	"IMAGE_CACHE_FOLDER",  	"C:\\ImageCache"));

	//Read from file. Create if file do not exist
	mGeneralProperties.read();

	//Update UI
    mBaseUrlE->update();
	mOwnerE->update();
	mProjectE->update();
    mStackNameE->update();

    mScaleE->update();
    mXCoordE->update();
    mYCoordE->update();
    mWidthE->update();
    mHeightE->update();

    mZMinE->update();
    mZMaxE->update();
    mCustomZsE->update();
	mImageCacheFolderE->update();

	return true;
}

