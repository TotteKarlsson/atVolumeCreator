#include <vcl.h>
#pragma hdrstop
#include "TMainForm.h"
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFloatLabeledEdit"
#pragma link "TIntegerLabeledEdit"
#pragma link "TSTDStringLabeledEdit"
#pragma resource "*.dfm"
TMainForm *MainForm;
using namespace mtk;
extern string gLogFileName;

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner),
    mLogLevel(lAny),
    logMsgMethod(&logMsg),
    mLogFileReader(joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "volumeCreator", gLogFileName), logMsgMethod)
{}

void __fastcall TMainForm::logMsg()
{
    infoMemo->Lines->Add(vclstr(mLogFileReader.getData()));
    mLogFileReader.purge();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mURLKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_RETURN)
    {

    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mZsClick(TObject *Sender)
{
    if(mZs->ItemIndex == -1)
    {
    	return;
    }
    string z = stdstr(mZs->Items->Strings[mZs->ItemIndex]);
    //Fetch data using URL
	stringstream sUrl;//("http://ibs-forrestc-ux1.corp.alleninstitute.org:8080/render-ws/v1/owner/Sharmishtaas/project/M259292_Scnn1aTg2_1/stack/{0}/
    //z/{1}/box/5000,9000,1300,1300,{2}/tiff-image");
    sUrl << stdstr(mBaseUrlE->Text);
    sUrl << "/" << stdstr(mOwnerE->Text);
	sUrl << "/project/";
    sUrl << stdstr(mProjectE->Text);
    sUrl << "/stack/"<<mStackNameE->getValue();
    sUrl << "/z/"<<z;
    sUrl << "/box/"<<mXCoord->getValue()<<","<<mYCoord->getValue();
    sUrl << "," << mWidth->getValue() << ","<<mHeight->getValue() << ","<<mScaleE->getValue();
    sUrl << "/tiff-image";

  	String outFile = "test.tiff";

	TMemoryStream* Stream = new TMemoryStream;
    try
    {
    	Log(lDebug) << "Loading z = "<<z;
    	Log(lDebug) << "URL = "<<sUrl.str();
    	IdHTTP1->Get(sUrl.str().c_str(), Stream);

        Image1->Picture->Graphic->LoadFromStream(Stream);
        Image1->Invalidate();
    }
    __finally
    {
    	delete Stream;
    }
}


void __fastcall TMainForm::mZMaxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if(Key == VK_RETURN)
    {
    	UpdateZList();
    }
}

//---------------------------------------------------------------------------
void  TMainForm::UpdateZList()
{
	mZs->Clear();
	for(int i = mZMin->getValue(); i < mZMax->getValue(); i++)
    {
		mZs->AddItem(IntToStr(i), NULL);
        mZs->Checked[i] = true;
    }


    mZs->ItemIndex = 0;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	mLogFileReader.start(true);
    initForm();

}

void TMainForm::initForm()
{
	UpdateZList();
	mZsClick(NULL);
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
	CanClose = mLogFileReader.isRunning();

	if(CanClose == false)
	{
		mShutDownTimer->Enabled = true;
	}
}


void __fastcall TMainForm::mScaleEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_RETURN)
    {
		mZsClick(Sender);
    }
}

void __fastcall TMainForm::mSelectZBtnClick(TObject *Sender)
{
;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::IdHTTP1Work(TObject *ASender, TWorkMode AWorkMode,
          __int64 AWorkCount)
{
	Log(lInfo) << "Pos: " << AWorkCount;
	ProgressBar1->Position = AWorkCount;
	this->Update();
}


void __fastcall TMainForm::IdHTTP1WorkBegin(TObject *ASender, TWorkMode AWorkMode,
          __int64 AWorkCountMax)
{
	ProgressBar1->Max = AWorkCountMax;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::IdHTTP1Status(TObject *ASender, const TIdStatus AStatus,
          const UnicodeString AStatusText)
{
	Log(lInfo) << stdstr(AStatusText);
}


TPoint controlToImage(const TPoint& p)
{

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	TPoint p = Mouse->CursorPos;
	p = this->Image1->ScreenToClient(p);
	Log(lInfo) << "X =   " << p.x ;
	Log(lInfo) << "Y =   " << p.y ;

    p = controlToImage(p);
	Log(lInfo) << "X1 =   " << p.x ;
	Log(lInfo) << "Y2 =   " << p.y ;

}


