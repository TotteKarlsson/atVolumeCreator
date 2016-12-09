#include <vcl.h>
#pragma hdrstop
#include "TMainForm.h"
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
#include <vector>
#include "atRenderService.h"
#include "MagickWand/MagickWand.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFloatLabeledEdit"
#pragma link "TIntegerLabeledEdit"
#pragma link "TSTDStringLabeledEdit"
#pragma link "TIntLabel"
#pragma link "TPropertyCheckBox"
#pragma resource "*.dfm"
TMainForm *MainForm;
using namespace mtk;
using namespace std;
extern string gLogFileName;

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner),
    mLogLevel(lAny),
    logMsgMethod(&logMsg),
    mLogFileReader(joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "volumeCreator", gLogFileName), logMsgMethod)
{}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ClickZ(TObject *Sender)
{
    if(mZs->ItemIndex == -1)
    {
    	return;
    }

    int z = toInt(stdstr(mZs->Items->Strings[mZs->ItemIndex]));

    //Fetch data using URL
    RenderService rs(IdHTTP1, mBaseUrlE->getValue(), mOwnerE->getValue(), mProjectE->getValue(),
                        mStackNameE->getValue(), "tiff-image", z, mCurrentRB, mScaleE->getValue());

    //First check if we already is having this data
    try
    {
    	Log(lDebug) << "Loading z = "<<z;
    	Log(lDebug) << "URL = "<< rs.getURL();

        TMemoryStream* imageMem = rs.getImage();
        if(imageMem)
        {
        	Image1->Picture->Graphic->LoadFromStream(imageMem);
	        Image1->Invalidate();
        }

        Log(lInfo) << "WxH = " <<Image1->Picture->Width << "x" << Image1->Picture->Height;
    }
    __finally
    {
    	rs.clearImageMemory();
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

void __fastcall TMainForm::mScaleEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_RETURN)
    {
		ClickZ(Sender);
    }
}

void __fastcall TMainForm::mSelectZBtnClick(TObject *Sender)
{
	TButton* b= dynamic_cast<TButton*>(Sender);
    bool select = (b == mSelectZBtn) ? true : false;

    for(int i = 0; i< mZs->Count; i++)
    {
        mZs->Checked[i] = select;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::IdHTTP1Work(TObject *ASender, TWorkMode AWorkMode,
          __int64 AWorkCount)
{
//	Log(lInfo) << "Pos: " << AWorkCount;
//	ProgressBar1->Position = AWorkCount;
//	this->Update();
}


void __fastcall TMainForm::IdHTTP1WorkBegin(TObject *ASender, TWorkMode AWorkMode,
          __int64 AWorkCountMax)
{
//	ProgressBar1->Max = AWorkCountMax;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::IdHTTP1Status(TObject *ASender, const TIdStatus AStatus,
          const UnicodeString AStatusText)
{
	Log(lInfo) << stdstr(AStatusText);
}

TPoint controlToImage(const TPoint& p, double scale, double stretchFactor)
{
	TPoint pt;
    pt.X = (p.X / scale) / stretchFactor;
    pt.Y = (p.Y / scale) / stretchFactor;

	return pt;
}

double TMainForm::getImageStretchFactor()
{
	if((mScaleE->getValue() * mHeight->getValue() * mWidth->getValue()) == 0)
    {
    	Log(lError) << "Tried to divide by zero!";
    	return 1;
    }

    if(Image1->Height < Image1->Width)
    {
    	return Image1->Height / (mScaleE->getValue() * mHeight->getValue());
    }
    else
    {
		return Image1->Width / (mScaleE->getValue() * mWidth->getValue());
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mStretchCBClick(TObject *Sender)
{
	Image1->Stretch = mStretchCB->Checked;
}

TCanvas* TMainForm::getCanvas()
{
	return PaintBox1->Canvas;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	if(Button == TMouseButton::mbRight)
    {
		mResetButton->Click();
        return;
    }

    Drawing = true;
    getCanvas()->MoveTo(X , Y);
    Origin = Point(X, Y);
    MovePt = Origin;

    //For selection
	mTopLeftSelCorner = Mouse->CursorPos;
	mTopLeftSelCorner = this->Image1->ScreenToClient(mTopLeftSelCorner);

	//Convert to world image coords (minus offset)
    double stretchFactor = getImageStretchFactor();
    mTopLeftSelCorner = controlToImage(mTopLeftSelCorner, mScaleE->getValue(), stretchFactor);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	TPoint p = Mouse->CursorPos;
	p = this->Image1->ScreenToClient(p);
	mXC->SetValue(p.X);
	mYC->SetValue(p.Y);

	//Convert to world image coords (minus offset)
    double stretchFactor = getImageStretchFactor();
    if(stretchFactor)
    {
	    p = controlToImage(p, mScaleE->getValue(), stretchFactor);
	  	mX->SetValue(p.X);
		mY->SetValue(p.Y);
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	if(!Drawing)
    {
    	return;
    }
	if(Button == TMouseButton::mbRight)
    {
		return;

    }

	//DrawShape(Origin, Point(Panel5->Width + X, Y), pmCopy);
	Drawing = false;

    //For selection
	mBottomRightSelCorner = Mouse->CursorPos;
	mBottomRightSelCorner = this->Image1->ScreenToClient(mBottomRightSelCorner);

	//Convert to world image coords (minus offset)
    double stretchFactor = getImageStretchFactor();
    mBottomRightSelCorner = controlToImage(mBottomRightSelCorner, mScaleE->getValue(), stretchFactor);

	mXCoord->setValue(mXCoord->getValue() + mTopLeftSelCorner.X);
	mYCoord->setValue(mYCoord->getValue() + mTopLeftSelCorner.Y);

    mWidth->setValue(mBottomRightSelCorner.X - mTopLeftSelCorner.X);
    mHeight->setValue(mBottomRightSelCorner.Y - mTopLeftSelCorner.Y);

    //Add to render history
    mCurrentRB = RenderBox(mXCoord->getValue(), mYCoord->getValue(), mWidth->getValue(), mHeight->getValue());
    mROIHistory.add(mCurrentRB);
	ClickZ(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
	if(Drawing)
  	{
		DrawShape(Origin, MovePt, pmNotXor);
		MovePt = Point(X, Y);
		DrawShape(Origin, MovePt, pmNotXor);
  	}

  Image1MouseMove(Sender, Shift, X, Y);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::DrawShape(TPoint TopLeft, TPoint BottomRight, TPenMode AMode)
{
  	getCanvas()->Pen->Mode = AMode;
	getCanvas()->Rectangle(TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y);
}

void __fastcall TMainForm::mResetButtonClick(TObject *Sender)
{
	mCurrentRB = mOriginalRB;
    render(&mCurrentRB);
    mROIHistory.add(mCurrentRB);
}

void TMainForm::render(RenderBox* box)
{
	if(box)
    {
        mCurrentRB = *(box);
        mXCoord->setValue(mCurrentRB.X);
        mYCoord->setValue(mCurrentRB.Y);
        mWidth->setValue(mCurrentRB.Width);
        mHeight->setValue(mCurrentRB.Height);
    }

	ClickZ(NULL);
}

void __fastcall TMainForm::historyBtnClick(TObject *Sender)
{
    TButton* b = dynamic_cast<TButton*>(Sender);
    if(b == mHistoryFFW)
    {
        RenderBox* rb = mROIHistory.next();
        if(rb)
        {
            render(rb);
        }
    }
    else if(b== mHistoryBackBtn)
    {
        RenderBox* rb = mROIHistory.previous();
        if(rb)
        {
            render(rb);
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TraverseZClick(TObject *Sender)
{
	TButton* b = dynamic_cast<TButton*>(Sender);
    if(b == mPrevZ)
    {
    	if(	mZs->ItemIndex > 0)
        {
			mZs->ItemIndex--;
        }
    }
    else if(b == mNextZ)
    {
    	if(	mZs->ItemIndex <  mZs->Count -1)
        {
			mZs->ItemIndex++;
        }
    }
    render();
}

string padZeroes(int z, int digits)
{
	string val(mtk::toString(z));
    if(val.size() >= digits)
    {
    	return val;
    }
    else
    {
		string prefix(digits - val.size(), '0');
        return prefix + val;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::mFetchSelectedZsBtnClick(TObject *Sender)
{
	//Start a few threads
    for(int i = 0; i < mZs->Count; i++)
    {
    	if(!mZs->Checked[i])
        {
        	continue;
        }
        int z = toInt(stdstr(mZs->Items->Strings[i]));

        //Fetch data using URL
        RenderService rs(IdHTTP1, mBaseUrlE->getValue(), mOwnerE->getValue(), mProjectE->getValue(),
                            mStackNameE->getValue(), "tiff-image", z, mCurrentRB, mScaleE->getValue());

        //First check if we already is having this data
        try
        {
            Log(lDebug) << "Loading z = "<<z;
            Log(lDebug) << "URL = "<< rs.getURL();

            //This will fetch from DB, or, if present, from the cache
            TMemoryStream* imageMem = rs.getImage();
            if(imageMem)
            {
                Image1->Picture->Graphic->LoadFromStream(imageMem);
                //Save to local box folder
                Image1->Invalidate();

//                stringstream outName;
//				outName << "p:\\cubes\\" << padZeroes(z, 4)<<"-raw.tif";

                //Get cached image and convert to 8-bit
//                Image1->Picture->SaveToFile(outName.str().c_str());

            }
        }
        __finally
        {
            rs.clearImageMemory();
        }

    }
//	//Fetch selections to local cache
//    Image1->Picture->SaveToFile("p:\\temp.tif");
//
//    MagickBooleanType status;
//    MagickWand *magick_wand;
//
//    /*
//    Read an image.
//    */
//    MagickWandGenesis();
//    magick_wand = NewMagickWand();
//    status = MagickReadImage(magick_wand,"p:\\temp.tif");
//    if (status == MagickFalse)
//    {
//    //    ThrowWandException(magick_wand);
//    }
//
//    status = MagickWriteImages(magick_wand,"p:\\temp2.tif",MagickTrue);
//    if (status == MagickFalse)
//    {
//    //Bad
//    }
//    magick_wand = DestroyMagickWand(magick_wand);
//    MagickWandTerminus();
}


