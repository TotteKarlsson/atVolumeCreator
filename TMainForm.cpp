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

bool convertTiff(const string& in, const string& out);
bool addTiffToStack(const string& stackFName, const string& fName);
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner),
    mLogLevel(lAny),
    logMsgMethod(&logMsg),
    mLogFileReader(joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "volumeCreator", gLogFileName), logMsgMethod),
    mTiffCP("C:\\cygwin\\bin\\tiffcp.exe")
{
	mTiffCP.FOnStateEvent = processEvent;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ClickZ(TObject *Sender)
{
	int ii = mZs->ItemIndex;
    if(ii == -1)
    {
    	return;
    }

    int z = toInt(stdstr(mZs->Items->Strings[ii]));

    //Fetch data using URL
    RenderService rs(IdHTTP1, mBaseUrlE->getValue(), mOwnerE->getValue(), mProjectE->getValue(),
                        mStackNameE->getValue(), "tiff-image", z, mCurrentRB, mScaleE->getValue());

    //First check if we already is having this data
    try
    {
    	Log(lDebug) << "Loading z = "<<z;
    	Log(lDebug) << "URL = "<< rs.getURL();

        TMemoryStream* imageMem = rs.getImage(z);
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
	for(int i = mZMin->getValue(); i <= mZMax->getValue(); i++)
    {
		mZs->AddItem(IntToStr(i), NULL);
//        mZs->Selected[i] = true;
    }

    mZs->ItemIndex = 0;
}

void __fastcall TMainForm::mScaleEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_RETURN)
    {
        mCurrentRB = RenderBox(mXCoord->getValue(), mYCoord->getValue(), mWidth->getValue(), mHeight->getValue());
		ClickZ(Sender);
    }
}

void __fastcall TMainForm::mSelectZBtnClick(TObject *Sender)
{
	TButton* b= dynamic_cast<TButton*>(Sender);
    bool select = (b == mSelectZBtn) ? true : false;

    for(int i = 0; i< mZs->Count; i++)
    {
        mZs->Selected[i] = select;
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

void __fastcall TMainForm::IdHTTP1WorkBegin(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCountMax)
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
	if(!Drawing ||  (Button == TMouseButton::mbRight))
    {
    	return;
    }

	Drawing = false;


    //For selection
	mBottomRightSelCorner = this->Image1->ScreenToClient(Mouse->CursorPos);

	//Convert to world image coords (minus offset)
    double stretchFactor = getImageStretchFactor();
    mBottomRightSelCorner = controlToImage(mBottomRightSelCorner, mScaleE->getValue(), stretchFactor);

	//Check if selection indicate a 'reset'
	if(mBottomRightSelCorner.X - mTopLeftSelCorner.X <= 0 || mBottomRightSelCorner.Y - mTopLeftSelCorner.Y <=0)
    {
    	resetButtonClick(NULL);
		return;
    }

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

void __fastcall TMainForm::resetButtonClick(TObject *Sender)
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
    if(mZs->ItemIndex > -1 && mZs->ItemIndex < mZs->Count)
    {
    	mZs->Selected[mZs->ItemIndex] = false;
    }
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
    mZs->Selected[mZs->ItemIndex] = true;
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
    int z = toInt(stdstr(mZs->Items->Strings[0]));
	RenderService rs(IdHTTP1, mBaseUrlE->getValue(), mOwnerE->getValue(), mProjectE->getValue(),
	    mStackNameE->getValue(), "tiff-image", z, mCurrentRB, mScaleE->getValue());

    //Creaet stackFName with path
    stringstream outName;
    outName << mVolumesFolder->getValue() <<"\\" << rs.getProjectName() <<"-"<<mScaleE->getValue()<<".tif";
	string stackFName(outName.str());
    outName.str("");
	//Start a few threads
    for(int i = 0; i < mZs->Count; i++)
    {
    	if(!mZs->Selected[i])
        {
        	continue;
        }
    	z = toInt(stdstr(mZs->Items->Strings[i]));
        //First check if we already is having this data
        //This will fetch from DB, or, if present, from the cache
        TMemoryStream* imageMem = rs.getImage(z);
        if(imageMem)
        {
            Image1->Picture->Graphic->LoadFromStream(imageMem);

            //Save to local box folder
            Image1->Invalidate();
			outName.str("");
            outName << mVolumesFolder->getValue() <<"\\" << rs.getProjectName() <<"\\"<<mScaleE->getValue()<<"\\"<<padZeroes(z, 4)<<".tif";
            string in = rs.getImageLocalPathAndFileName();
            //Make sure path exists, if not create it
            if(createFolder(getFilePath(outName.str())))
            {
                if(convertTiff(in, outName.str()))
                {
                    Log(lInfo) << "Converted file: "<<in<<" to "<<outName;
                    addTiffToStack(stackFName, outName.str());
                }
            }
        }
	    rs.clearImageMemory();
        Application->ProcessMessages();
    }
}

bool __fastcall	TMainForm::addTiffToStack(const string& stackFName, const string& fName)
{
    //Call external progam to stack the tiff
    //output = call(["C:\\cygwin\\bin\\tiffcp", "-a", stackThisFile, stackFileName])
    stringstream s;
    s<<"-a "<<fName<<" "<<stackFName;

   	mTiffCP.setMessageHandling(CATCH_MESSAGE);
	mTiffCP.run(s.str());
	//check that files exists
	if(!fileExists(stackFName) || !fileExists(fName))
    {
    	Log(lError) << "File does not exist...";
        return false;
    }

	return true;
}

//---------------------------------------------------------------------------
bool convertTiff(const string& in, const string& out)
{
    MagickBooleanType status;
    MagickWand *magick_wand;

    MagickWandGenesis();
    magick_wand = NewMagickWand();
    status = MagickReadImage(magick_wand, in.c_str());
    if (status == MagickFalse)
    {
        return false;
    }

    status = MagickWriteImages(magick_wand, out.c_str(), MagickTrue);
    if (status == MagickFalse)
    {
    	//Bad
        return false;
    }
    magick_wand = DestroyMagickWand(magick_wand);
    MagickWandTerminus();
    return true;
}

void __fastcall	TMainForm::processEvent(Process* proc)
{
	vector<string> out = proc->getOutput();
    for(int i = 0; i < out.size(); i++)
    {
    	Log(lInfo) << out[i];
    }
}


int __fastcall compareStringListItems(TStringList* l, int index1, int index2)
{
	int l1 = (l->Strings[index1]).ToInt();
	int l2 = (l->Strings[index2]).ToInt();
    if(l1 < l2)
    {
    	return -1;
    }
    else
    {
	 	if(l1 > l2)
        {
        	return 1;
        }
        else
        {
        	return 0;
        }
    }
}

bool sortTListBoxNumerically(TListBox* lb)
{
	TStringList* tempList = new TStringList;
    tempList->Assign(lb->Items);
    tempList->CustomSort(compareStringListItems);
    lb->Items->Assign(tempList);
    delete tempList;

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::mMoveOutSelectedBtnClick(TObject *Sender)
{
	if(mZs->SelCount == 0)
    {
    	Log(lWarning) << "There are no items selected...";
        return;
    }

	int selectedAfter;
    for(int i = 0; i < mZs->Count; i++)
    {
    	if(mZs->Selected[i])
        {
 			String item = mZs->Items->Strings[i];
        	mDeSelectedZs->AddItem(item, NULL);
            selectedAfter = i;
        }
    }

    int selCount = mZs->SelCount;
    mZs->DeleteSelected();

    int newlySelected = selectedAfter - (selCount -1);
    if(newlySelected > -1 && newlySelected < mZs->Count)
    {
    	mZs->Selected[newlySelected] = true;
        mZs->ItemIndex = newlySelected;
        render();
    }

    sortTListBoxNumerically(mDeSelectedZs);
}


void __fastcall TMainForm::mRestoreUnselectedBtnClick(TObject *Sender)
{
    for(int i = 0; i < mDeSelectedZs->Count; i++)
    {
    	if(mDeSelectedZs->Selected[i])
        {
 			String item = mDeSelectedZs->Items->Strings[i];
        	mZs->AddItem(item, NULL);
        }
    }
    mDeSelectedZs->DeleteSelected();
    sortTListBoxNumerically(mZs);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button1Click(TObject *Sender)
{
    sortTListBoxNumerically(mZs);
}



