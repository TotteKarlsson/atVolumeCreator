#include <vcl.h>
#pragma hdrstop
#include "TMainForm.h"
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
#include <vector>
#include "atRenderClient.h"
#include "MagickWand/MagickWand.h"
#include "mtkExeFile.h"
#include "mtkMathUtils.h"
#include "atImageForm.h"
#include "TMemoLogger.h"
#include "TSelectZsForm.h"
#define QuantumScale  ((MagickRealType) 1.0/(MagickRealType) QuantumRange)
#define SigmoidalContrast(x) \
  (QuantumRange*(1.0/(1+exp(10.0*(0.5-QuantumScale*x)))-0.0066928509)*1.0092503)
#define ThrowWandException(wand) \
{ \
  char \
    *description; \
 \
  ExceptionType \
    severity; \
 \
  description=MagickGetException(wand,&severity); \
  (void) fprintf(stderr,"%s %s %lu %s\n",GetMagickModule(),description); \
  description=(char *) MagickRelinquishMemory(description); \
  exit(-1); \
}

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFloatLabeledEdit"
#pragma link "TIntegerLabeledEdit"
#pragma link "TSTDStringLabeledEdit"
#pragma link "TIntLabel"
#pragma link "TPropertyCheckBox"
#pragma link "mtkIniFileC"
#pragma link "mtkIntEdit"
#pragma link "TSSHFrame"
#pragma link "TImageControlsFrame"
#pragma resource "*.dfm"
TMainForm *MainForm;
using namespace mtk;
using namespace std;
extern string gLogFileName;

TImage *CurrImage;
extern string gAppDataLocation;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner),
    mLogLevel(lAny),
    mLogFileReader(joinPath(gAppDataLocation, gLogFileName), logMsg),
    mBottomPanelHeight(205),
	mCreateCacheThread(),
    mRC(IdHTTP1),
    mImageForm(NULL),
    mRenderEnabled(false),
    mCurrentProject(""),
    mCurrentOwner(""),
    mCurrentStack("")
{
    setupIniFile();
    setupAndReadIniParameters();

    mCreateCacheThread.setCacheRoot(mImageCacheFolderE->getValue());
  	TMemoLogger::mMemoIsEnabled = true;
	CurrImage = Image1;
    mRC.assignOnImageCallback(onImage);
}

void __fastcall TMainForm::onImage()
{
	//This is called from a thread and need to be synchronized with the UI main thread
	TMemoryStream* imageMem = mRC.getImageMemory();
    if(imageMem)
    {
        if(fileExists(mRC.getImageLocalPathAndFileName()))
        {
           	const char* pic = mRC.getImageLocalPathAndFileName().c_str();
	    	Image1->Picture->Graphic->LoadFromFile(pic);
	        Image1->Invalidate();
		    Log(lInfo) << "WxH = " <<Image1->Picture->Width << "x" << Image1->Picture->Height;

        }
        else
        {
		    Log(lInfo) << "BAD FILE: " <<mRC.getImageLocalPathAndFileName();
        }
        this->Image1->Cursor = crDefault;
    }
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
	mRC.setLocalCacheFolder(mImageCacheFolderE->getValue());
	mRC.init(mCurrentOwner.getValue(), mCurrentProject.getValue(), mCurrentStack.getValue(), "jpeg-image", z, mCurrentRB, mScaleE->getValue(), MinIntensity->getValue(), MaxIntensity->getValue());

	this->Image1->Cursor = crHourGlass;
    mRC.getImageInThread(z);
}

void __fastcall TMainForm::mScaleEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_RETURN)
    {
        mCurrentRB = RenderBox(XCoord->getValue(), YCoord->getValue(), Width->getValue(), Height->getValue());
		ClickZ(Sender);
    }
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
	if((mScaleE->getValue() * Height->getValue() * Width->getValue()) == 0)
    {
    	Log(lError) << "Tried to divide by zero!";
    	return 1;
    }

    if(Image1->Height < Image1->Width)
    {
    	return Image1->Height / (mScaleE->getValue() * Height->getValue());
    }
    else
    {
		return Image1->Width / (mScaleE->getValue() * Width->getValue());
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mStretchCBClick(TObject *Sender)
{
//	Image1->Stretch = mStretchCB->Checked;
}

TCanvas* TMainForm::getCanvas()
{
	return PaintBox1->Canvas;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	if(mRenderEnabled == false)
    {
    	return;
    }

	if(Button == TMouseButton::mbRight)
    {
    	//Open popup

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
	TPoint p = this->Image1->ScreenToClient(Mouse->CursorPos);
	mXC->setValue(p.X);
	mYC->setValue(p.Y);

	//Convert to world image coords (minus offset)
    double stretchFactor = getImageStretchFactor();
    if(stretchFactor)
    {
	    p = controlToImage(p, mScaleE->getValue(), stretchFactor);
	  	mX->setValue(p.X);
		mY->setValue(p.Y);
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

	XCoord->setValue(XCoord->getValue() + mTopLeftSelCorner.X);
	YCoord->setValue(YCoord->getValue() + mTopLeftSelCorner.Y);

    Width->setValue(mBottomRightSelCorner.X - mTopLeftSelCorner.X);
    Height->setValue(mBottomRightSelCorner.Y - mTopLeftSelCorner.Y);

    //Add to render history
    mCurrentRB = RenderBox(XCoord->getValue(), YCoord->getValue(), Width->getValue(), Height->getValue());
    mROIHistory.add(mCurrentRB);

    updateScale();
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

int	TMainForm::getCurrentZ()
{
	int ii = mZs->ItemIndex;
    if(ii == -1)
    {
    	return -1;
    }

    return toInt(stdstr(mZs->Items->Strings[ii]));
}

void __fastcall TMainForm::resetButtonClick(TObject *Sender)
{
    //Get saved renderbox for current slice
	try
    {
	    mScaleE->setValue(0.05);
        mCurrentRB = mRC.getBoxForZ(getCurrentZ());
        render(&mCurrentRB);
        mROIHistory.clear();
        mROIHistory.add(mCurrentRB);
    }
    catch(...)
    {}
}

void TMainForm::render(RenderBox* box)
{
	if(box)
    {
        mCurrentRB = *(box);
        XCoord->setValue(mCurrentRB.getX1());
        YCoord->setValue(mCurrentRB.getY1());
        Width->setValue(mCurrentRB.getWidth());
        Height->setValue(mCurrentRB.getHeight());
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
    mZs->Selected[mZs->ItemIndex] = true;
    render();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mFetchSelectedZsBtnClick(TObject *Sender)
{

	if(mCreateCacheThread.isRunning())
    {
		mCreateCacheThread.stop();
    }
    else
    {
        int z = toInt(stdstr(mZs->Items->Strings[0]));
        RenderClient rs(IdHTTP1, mBaseUrlE->getValue(), mCurrentOwner.getValue(), mCurrentProject.getValue(),
            mCurrentStack.getValue(), "jpeg-image", z, mCurrentRB, mScaleE->getValue(), MinIntensity->getValue(), MaxIntensity->getValue(), mImageCacheFolderE->getValue());

        //Create image URLs
        StringList urls;
        for(int i = 0; i < mZs->Count; i++)
        {
            int	z = toInt(stdstr(mZs->Items->Strings[i]));
            urls.append(rs.getURLForZ(z));
        }

        mCreateCacheThread.setup(urls, mImageCacheFolderE->getValue());
        mCreateCacheThread.start();
        CreateCacheTimer->Enabled = true;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mBrowseForCacheFolderClick(TObject *Sender)
{
	//Browse for folder
	string res = browseForFolder(mImageCacheFolderE->getValue());
    if(folderExists(res))
    {
		mImageCacheFolderE->setValue(res);
    }
    else
    {
    	Log(lWarning) << "Cache folder was not set..";
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mGetValidZsBtnClick(TObject *Sender)
{
	//Fetch valid zs for current project
   	RenderClient rs(IdHTTP1, mBaseUrlE->getValue(), mCurrentOwner.getValue(), mCurrentProject.getValue(),	mCurrentStack.getValue());
    StringList zs = rs.getValidZs();

	Log(lInfo) << "Fetched "<<zs.count()<<" valid z's";

	Zs_GB->Caption = " Z Values (" + IntToStr((int) zs.count()) + ") ";
    //Populate list box
	populateCheckListBox(zs, mZs);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mCLearMemoClick(TObject *Sender)
{
	infoMemo->Clear();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mUpdateZsBtnClick(TObject *Sender)
{
	//Fetch valid zs for current project
   	RenderClient rs(IdHTTP1, mBaseUrlE->getValue(), mCurrentOwner.getValue(), mCurrentProject.getValue(),	mCurrentStack.getValue());
    StringList zs = rs.getZs();

    if(zs.size() > 1)
    {
    	//Populate list boxes
	    Log(lInfo) << "Valid Z's: "<<zs[0];
    	Log(lInfo) << "Missing Z's: "<<zs[1];
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CopyValidZs1Click(TObject *Sender)
{
	//Figure out wich listbox called

	TCheckListBox* lb = dynamic_cast<TCheckListBox*>(ZsPopUpMenu->PopupComponent);

    if(!lb)
    {
    	return;
    }

    stringstream zs;
    for(int i = 0; i < lb->Count; i++)
    {
    	zs << stdstr(lb->Items->Strings[i]);
        if(i < (lb->Count -1))
        {
        	zs <<",";
        }
    }
	sendToClipBoard(zs.str());
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::GetOptimalBoundsBtnClick(TObject *Sender)
{
	RenderClient rs(IdHTTP1, mBaseUrlE->getValue(), mCurrentOwner.getValue(), mCurrentProject.getValue(),	mCurrentStack.getValue());

    vector<int> zs = rs.getValidZs();
    RenderBox box = rs.getOptimalXYBoxForZs(zs);
    Log(lInfo) << "XMin = " << box.getX1();
    Log(lInfo) << "XMax = " << box.getX2();
    Log(lInfo) << "YMin = " << box.getY1();
    Log(lInfo) << "YMax = " << box.getY2();

   	vector<RenderBox> bounds = rs.getBounds();
    for(int i = 0; i < bounds.size(); i++)
    {
	    Log(lInfo) <<bounds[i].getZ()<<","<<bounds[i].getX1()<<","<<bounds[i].getX2()<<","<<bounds[i].getY1()<<","<<bounds[i].getY2();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mZoomBtnClick(TObject *Sender)
{
	TButton* b = dynamic_cast<TButton*>(Sender);

	double zoomFactor = mZoomFactor->getValue();
    if(b == mZoomOutBtn)
    {
		zoomFactor *= (-1.0);
    }

	//Modify bounding box with x%
    mCurrentRB = RenderBox(XCoord->getValue(), YCoord->getValue(), Width->getValue(), Height->getValue());
    mCurrentRB.zoom(zoomFactor);

	XCoord->setValue(mCurrentRB.getX1());
    YCoord->setValue(mCurrentRB.getY1());
    Width->setValue( mCurrentRB.getWidth());
    Height->setValue(mCurrentRB.getHeight());

    updateScale();
	ClickZ(Sender);
}

void TMainForm::updateScale()
{
    mCurrentRB = RenderBox(XCoord->getValue(), YCoord->getValue(), Width->getValue(), Height->getValue());

    //Scale the scaling
    double scale  = (double) Image1->Height / (double) mCurrentRB.getHeight();
    Log(lInfo) << "Scaling is: " << scale;
	if(scale < 0.005)
    {
    	scale = 0.009;
    }
    else if(scale > 1.0)
    {
    	scale = 1.0;
    }
	mScaleE->setValue(scale);
}

//--------------------------------------------------------------------------
void __fastcall TMainForm::OwnerCBChange(TObject *Sender)
{
	//Update Projects CB
    //Get selected owner
    if(OwnerCB->ItemIndex == -1)
    {
		return;
    }

    //Disable uninitialized sections of the UI
	enableDisableGroupBox(imageParasGB, false);
	enableDisableGroupBox(Zs_GB, false);
    enableDisableGroupBox(StackGenerationGB, false);
   	mRenderEnabled = false;
	StackCB->Clear();

    string owner = stdstr(OwnerCB->Items->Strings[OwnerCB->ItemIndex]);
    mCurrentOwner.setValue(owner);

    //Populate projects
    StringList p = mRC.getProjectsForOwner(mCurrentOwner.getValue());
    if(p.size())
    {
		populateDropDown(p, ProjectCB);
    }
}

void __fastcall TMainForm::ProjectCBChange(TObject *Sender)
{
	//Update Stacks CB
    //Get selected owner
    if(ProjectCB->ItemIndex == -1)
    {
		return;
    }

    //Disable uninitialized sections of the UI
	enableDisableGroupBox(imageParasGB, false);
	enableDisableGroupBox(Zs_GB, false);
    enableDisableGroupBox(StackGenerationGB, false);
	mRenderEnabled = false;
    string owner = stdstr(OwnerCB->Items->Strings[OwnerCB->ItemIndex]);
    string project = stdstr(ProjectCB->Items->Strings[ProjectCB->ItemIndex]);
    mCurrentProject.setValue(project);

    //Populate stacks
    StringList s = mRC.getStacksForProject(owner, mCurrentProject.getValue());
    if(s.size())
    {
		populateDropDown(s, StackCB);
		populateCheckListBox(s, StacksForProjectCB);
    }
}

void __fastcall TMainForm::StackCBChange(TObject *Sender)
{
    if(StackCB->ItemIndex == -1)
    {
		return;
    }

    string stack = stdstr(StackCB->Items->Strings[StackCB->ItemIndex]);
	mCurrentStack.setValue(stack);

	mRC.getProject().setupForStack(mCurrentOwner.getValue(), mCurrentProject.getValue(), mCurrentStack.getValue());

   	mGetValidZsBtnClick(NULL);
//    resetButtonClick(NULL);
	ClickZ(NULL);

    //Update stack generation page
	//User changed stack.. Clear check list box and select current one
    for(int i = 0; i < StacksForProjectCB->Items->Count; i++)
    {
        if(StacksForProjectCB->Items->Strings[i] == StackCB->Text)
        {
	    	StacksForProjectCB->Checked[i] = true;
        }
        else
        {
    		StacksForProjectCB->Checked[i] = false;
        }
    }

    //Disable uninitialized sections of the UI
	enableDisableGroupBox(imageParasGB, true);
	enableDisableGroupBox(Zs_GB, true);
	mRenderEnabled = true;
}


//---------------------------------------------------------------------------
void __fastcall TMainForm::mDetachBtnClick(TObject *Sender)
{
	//Open image form
//    if(!mImageForm)
    {
    	mImageForm = new TImageForm(gApplicationRegistryRoot, "ImageForm", this);

    }
	mImageForm->Show();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mCloseBottomPanelBtnClick(TObject *Sender)
{
	mBottomPanel->Visible = false;
    mShowBottomPanelBtn->Visible = true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mShowBottomPanelBtnClick(TObject *Sender)
{
	mBottomPanel->Visible = true;
    mShowBottomPanelBtn->Visible = false;
    Splitter2->Top = mBottomPanel->Top - 1;
}


void __fastcall TMainForm::TSSHFrame1ScSSHShell1AsyncReceive(TObject *Sender)
{
	//Parse messages from the server
    string line(stdstr(TSSHFrame1->ScSSHShell1->ReadString()));
//	if(contains("$",line) || contains("echo",line) || contains("[main]", line))
//    {
//
//    }
//    else
    {
 		Log(lInfo) << line;
    }
}

void __fastcall TMainForm::CMDButtonClick(TObject *Sender)
{
	stringstream cmd;
    cmd << stdstr(mCMD->Text) << endl;
    TSSHFrame1->ScSSHShell1->WriteString(vclstr(cmd.str()));
}

string escape(const string& before)
{
    string escaped(before);
    //Pretty bisarre syntax.. see http://stackoverflow.com/questions/1250079/how-to-escape-single-quotes-within-single-quoted-strings
    escaped = replaceSubstring("'", "'\"'\"'", escaped);
    return "'" + escaped + "'";
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::RunClick(TObject *Sender)
{
	//Create remote jobs by script
    string scriptName("runner.sh");

    //Create empty, runnable script on server
	string remoteScriptName = createEmptyScriptFileOnServer(scriptName);

	//Populate remote script
	if(!populateRemoteScript(remoteScriptName))
    {
    	Log(lError) << "Failed to populate remote script.. bailing";
    	return;
    }

    //Create command lines (jobs)
   	vector<string> commands;
    StringList stacks = getCheckedItems(StacksForProjectCB);
	for(int i = 0; i < stacks.size() ; i++)
    {
		commands.push_back(createRemoteCommand(remoteScriptName, stacks[i]));
        Log(lDebug) << "Command "<<i<<" :"<<commands[i];
    }

	for(int i = 0; i < commands.size(); i++)
    {
    	runJob(commands[i]);
    }
}

string TMainForm::createEmptyScriptFileOnServer(const string& scriptName)
{
    stringstream cmd;

	//First create remote folders
    string folders(joinPath(stdstr(VolumesFolder->Text), stdstr(SubFolder1->Text), '/'));
    cmd << "mkdir -p "<<folders;

    string remoteScript(joinPath(folders, scriptName, '/'));

	cmd << " && touch "<< remoteScript << endl;
    TSSHFrame1->ScSSHShell1->WriteString(vclstr(cmd.str()));
	cmd.str("");

    //Make executable
	cmd << "chmod +x "<< remoteScript << endl;
    TSSHFrame1->ScSSHShell1->WriteString(vclstr(cmd.str()));
	return remoteScript;
}

bool TMainForm::populateRemoteScript(const string& remoteScriptName)
{
    stringstream cmd;
    StringList lines(getStrings(BashScriptMemo));
    if(lines.size() <= 1)
    {
    	Log(lError) << "Can't populate remote script. Script Memo is Empty??";
        return false;
    }

	cmd << "echo \"\" > " <<remoteScriptName << endl;
    //Copy content of memo
    for(int i = 0 ; i < lines.size(); i++)
    {
		//Log(lInfo) << "echo "<<escape(lines[i])<< " >> " << remoteScript << endl;
		cmd << "echo "<< escape(lines[i])<< " >> " << remoteScriptName << endl;
    }

    TSSHFrame1->ScSSHShell1->WriteString(vclstr(cmd.str()));
    cmd.str("");
    return true;
}

void TMainForm::runJob(const string& job)
{
    TSSHFrame1->ScSSHShell1->WriteString(vclstr(job));
}

string TMainForm::createRemoteCommand(const string& remoteScript, const string& stack)
{
	stringstream cmd;

	//Create commandline for remote bash script
	cmd << remoteScript;

    //First argument is number of sections
    cmd<<" "<<getNumberOfCheckedItems(mZs);

    //Second argument is section numbers
    cmd << " '";
	for(int i = 0; i < mZs->Count; i++)
    {
    	if(mZs->Checked[i] == true)
        {
            cmd << mZs->Items->Strings[i].ToInt();
            if(i < mZs->Count -1)
            {
                cmd << " ";
            }
        }
    }
    cmd <<"'";

    //Third argument is root outputfolder
    cmd <<" "<<stdstr(VolumesFolder->Text);

    //Fourth arg is custom outputfolder
	cmd <<" "<<stdstr(SubFolder1->Text);

    //Fifth arg is custom outputfolder
	cmd <<" "<<stack;

	//Sixth is owner
    cmd <<" "<<mCurrentOwner;

    //7th - project
    cmd <<" "<<mCurrentProject;

    //8th - scale
	cmd <<" "<<stdstr(VolumesScaleE->Text);

	//9th - static bounds?
    cmd <<" "<<mtk::toString(BoundsCB->Checked);

    if(BoundsCB->Checked)
    {
    	//Pass bounds, xmin, xmax, ymin,ymax
        cmd <<" '"
        	<<XCoord->getValue()<<","
        	<<XCoord->getValue() + Width->getValue()<<","
            <<YCoord->getValue()<<","
            <<YCoord->getValue() + Height->getValue()<<"'";
    }

    cmd << " &" ;
    cmd << endl;

//    //Now execute..
//    TSSHFrame1->ScSSHShell1->WriteString(vclstr(cmd.str()));
	return cmd.str();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TSSHFrame1ScSSHClientAfterConnect(TObject *Sender)
{
	enableDisableGroupBox(StackGenerationGB, true);
	enableDisableGroupBox(TestSSHGB, true);

  	TSSHFrame1->ScSSHClientAfterConnect(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TSSHFrame1ScSSHClientAfterDisconnect(TObject *Sender)
{
	TSSHFrame1->ScSSHClientAfterDisconnect(Sender);
	enableDisableGroupBox(StackGenerationGB, false);
	enableDisableGroupBox(TestSSHGB, false);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_ESCAPE)
    {
        Close();
    }
}

void __fastcall TMainForm::CreateCacheTimerTimer(TObject *Sender)
{
	if(mCreateCacheThread.isRunning())
    {
		mFetchSelectedZsBtn->Caption = "Stop Cache Creation";
    }
    else
    {
		CreateCacheTimer->Enabled = false;
		mFetchSelectedZsBtn->Caption = "Generate Cache";
    }
}

void __fastcall TMainForm::OpenInNDVIZBtnClick(TObject *Sender)
{
	TButton* b = dynamic_cast<TButton*>(Sender);

    if(b == OpenInNDVIZBtn)
    {
	    string   url(createNDVIZURL());
		ShellExecuteA(0,0, "chrome.exe", url.c_str(),0,SW_SHOWMAXIMIZED);
		return;
    }

    if(b == OpenFromNDVIZBtn)
    {
		ParseNDVIZURL1Click(NULL);
		return;
    }

    MagickBooleanType status;
    PixelInfo pixel;
    MagickWand *contrast_wand, *image_wand;
    PixelIterator *contrast_iterator, *iterator;
    PixelWand **contrast_pixels, **pixels;
    register ssize_t x;
    size_t width;
    ssize_t y;

    MagickWandGenesis();
    image_wand=NewMagickWand();
    string currImage = mRC.getImageLocalPathAndFileName();

    status=MagickReadImage(image_wand, currImage.c_str());
    if (status == MagickFalse)
    {
    	ThrowWandException(image_wand);
    }

    contrast_wand = CloneMagickWand(image_wand);

    /*
    Sigmoidal non-linearity contrast control.
    */
    iterator=NewPixelIterator(image_wand);
    contrast_iterator=NewPixelIterator(contrast_wand);
    if ((iterator == (PixelIterator *) NULL) || (contrast_iterator == (PixelIterator *) NULL))
    {
    	ThrowWandException(image_wand);
    }

    for (y=0; y < (ssize_t) MagickGetImageHeight(image_wand); y++)
    {
	    pixels=PixelGetNextIteratorRow(iterator,&width);
    	contrast_pixels=PixelGetNextIteratorRow(contrast_iterator,&width);
    	if ((pixels == (PixelWand **) NULL) || (contrast_pixels == (PixelWand **) NULL))
        {
      		break;
        }

        for (x=0; x < (ssize_t) width; x++)
        {
        	PixelGetMagickColor(pixels[x],&pixel);
          	pixel.red=SigmoidalContrast(pixel.red);
          	pixel.green=SigmoidalContrast(pixel.green);
          	pixel.blue=SigmoidalContrast(pixel.blue);
          	pixel.index=SigmoidalContrast(pixel.index);
        	//      PixelSetMagickColor(contrast_pixels[x],&pixel);
          	PixelSetPixelColor(contrast_pixels[x],&pixel);
        }

    	(void) PixelSyncIterator(contrast_iterator);
    }

    if (y < (ssize_t) MagickGetImageHeight(image_wand))
    {
        ThrowWandException(image_wand);
    }

    contrast_iterator=DestroyPixelIterator(contrast_iterator);
    iterator=DestroyPixelIterator(iterator);
    image_wand=DestroyMagickWand(image_wand);

    /*
    Write the image then destroy it.
    */
    status = MagickWriteImages(contrast_wand,currImage.c_str(),MagickTrue);
    if (status == MagickFalse)
    {
    	ThrowWandException(image_wand);
    }

    contrast_wand = DestroyMagickWand(contrast_wand);
    MagickWandTerminus();

    //Reload
    ClickZ(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::IntensityKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if(Key != VK_RETURN)
    {
    	return;
    }
    int minInt = MinIntensity->getValue();
    int maxInt = MaxIntensity->getValue();
    int ii = mZs->ItemIndex;
    if(ii == -1)
    {
        return;
    }

    int z = toInt(stdstr(mZs->Items->Strings[ii]));

    //Fetch data using URL
    mRC.setLocalCacheFolder(mImageCacheFolderE->getValue());
    mRC.init(mCurrentOwner.getValue(), mCurrentProject.getValue(), mCurrentStack.getValue(), "jpeg-image", z, mCurrentRB, mScaleE->getValue(), minInt, maxInt);

    //First check if we already is having this data
    try
    {
        try
        {
            Log(lDebug) << "Loading z = "<<z;
            Log(lDebug) << "URL = "<< mRC.getURL();

            TMemoryStream* imageMem = mRC.reloadImage(z);
            if(imageMem)
            {
                Image1->Picture->Graphic->LoadFromStream(imageMem);
                Image1->Invalidate();
            }

            Log(lInfo) << "WxH = " <<Image1->Picture->Width << "x" << Image1->Picture->Height;
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            Log(lError) << "There was a memory problem..";
        }
    }
    __finally
    {
        mRC.clearImageMemory();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FilterStacksEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	String filter = FilterStacksEdit->Text;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ParseNDVIZURL1Click(TObject *Sender)
{
	Log(lInfo) << "Parsing clip board data";
	string cb = getClipBoardText();
    Log(lInfo) <<"ClipBoard: "<<cb;
    StringList url(cb, '{');
    //Structure of ndviz URL
	//http://ibs-forrestc-ux1.corp.alleninstitute.org:8001/#!
    //{
    //	'layers':
    //	{
    //		'ACQGephyrin':
	//		{
    //			'type':'image'_'source':'render://http://ibs-forrestc-ux1.corp.alleninstitute.org/Forrest/H16_03_005_HSV_HEF1AG65_R2An15dTom/ACQGephyrin'_'max':0.15259
    //		}
    //	}
    //_'navigation':
    //	{
    //		'pose':
    //		{
    //			'position':
    //			{
    //				'voxelSize':[1_1_1]_'voxelCoordinates':[4341.58935546875_680.30517578125_2]
    //			}
    //		}
    //	_'zoomFactor':1.8876945060824133
    //	}
    //}

    if(url.size() < 5)
    {
	   	Log(lError) <<"Failed to parse render URL.";
        return;
    }

    // '
    //	type
    //'
    //:
    //'
    //image
    //'
    //_
    //'
    //source
    //'
    //:
    //'
    //render://http://ibs-forrestc-ux1.corp.alleninstitute.org/Forrest/H16_03_005_HSV_HEF1AG65_R2An15dTom/ACQGephyrin
    //
    //'
    //_
    //'
    //max
    //'
    //:0.15259}}_
    //'
    //navigation
    //':
	//Now extract line with 'render'
    //Extract line with render URL
    string rurl1 = url.getLineContaining("render");
    if(!rurl1.size())
    {
    	Log(lError) <<"Failed to parse render URL";
        return;
    }

	Log(lInfo) <<rurl1;

    //Cut string on '\''
    StringList url_items(rurl1, '\'');

	Log(lInfo) <<url_items.getLineContaining("render");
	StringList pass3(url_items.getLineContaining("render"), '/');

    Log(lInfo) << "Pass3" << pass3;
	//Element 5,6 and 7 are owner, project and stack
    if(url_items.size() < 8)
    {
    	Log(lError) <<"Failed to parse render URL";
        return;
    }
    string owner(pass3[5]);
    string project(pass3[6]);
    string stack(pass3[7]);

    Log(lInfo) <<"Owner: "<<owner;
    Log(lInfo) <<"Project: "<<project;
    Log(lInfo) <<"Stack: "<<stack;

	//Now get z, x,y and zoom factor
    //				'voxelSize':[1_1_1]_'voxelCoordinates':[4341.58935546875_680.30517578125_2]
    string rurl2 = url.getLineContaining("voxel");
    if(rurl2.size() == 0)
    {
    	Log(lError) <<"Failed to parse render URL";
        return;
    }

    StringList pass4(rurl2, '[');
    Log(lDebug) << pass4;
    if(pass4.size() < 3)
    {
    	Log(lError) <<"Failed to parse render URL";
        return;
    }
    string nrs(pass4[2]);
    Log(lDebug) << nrs;
    StringList pass5(nrs, '_');
	Log(lDebug) << pass5;
    double x = toDouble(pass5[0]);
    double y = toDouble(pass5[1]);
    int z = toInt(pass5[2]);
    Log(lDebug) << "x = "<<x <<", y = "<<y<<", z = "<<z;
    //Zoomfactor
    StringList pass6(pass5[3], ':');
    Log(lDebug) << pass6;
    double zf = toDouble(pass6[1]);
    Log(lDebug) << "ZoomFactor: "<<zf;

    //Assume we got proper numbers, populate application
    int i = OwnerCB->Items->IndexOf(owner.c_str());
    if(i < 0)
    {
    	Log(lError) <<"Failed to parse render URL";
        return;
    }

	OwnerCB->ItemIndex = i;
    OwnerCB->OnChange(NULL);
    i = ProjectCB->Items->IndexOf(project.c_str());
    if(i < 0)
    {
    	Log(lError) <<"Failed to parse render URL";
        return;
    }

	ProjectCB->ItemIndex = i;
    ProjectCB->OnChange(NULL);

    i = StackCB->Items->IndexOf(stack.c_str());
    if(i < 0)
    {
    	Log(lError) <<"Failed to parse render URL";
        return;
    }

	StackCB->ItemIndex = i;
    StackCB->OnChange(NULL);

    XCoord->setValue(x);
    YCoord->setValue(y);

    i = mZs->Items->IndexOf(mtk::toString(z).c_str());
    if(i < 0)
    {
    	Log(lError) <<"Failed to parse render URL";
        return;
    }
	mZs->ItemIndex = i;
    mZs->Selected[i] = true;
    mZs->OnClick(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CreateNDVIZURL1Click(TObject *Sender)
{
    string url(createNDVIZURL());
    Log(lInfo) << url;
}

string TMainForm::createNDVIZURL()
{
	string URL("http://ibs-forrestc-ux1.corp.alleninstitute.org:8001/#!{'layers':{'STACK':{'type':'image'_'source':'render://http://ibs-forrestc-ux1.corp.alleninstitute.org/OWNER/PROJECT/STACK'_'max':MAX_INTENSITY}}_'navigation':{'pose':{'position':{'voxelSize':[1_1_1]_'voxelCoordinates':[X_CENTER_Y_CENTER_Z_VALUE]}}_'zoomFactor':ZOOM_FACTOR}}");
    Log(lInfo) << URL;

    double xCenter = XCoord->getValue() + Width->getValue()/2.;
	double yCenter = YCoord->getValue() + Height->getValue()/2.;
    URL = replaceSubstring("STACK", 	        stdstr(StackCB->Text), 	                                URL);
    URL = replaceSubstring("OWNER", 	        stdstr(OwnerCB->Text), 	                                URL);
    URL = replaceSubstring("PROJECT", 	        stdstr(ProjectCB->Text), 	                                URL);
    URL = replaceSubstring("MAX_INTENSITY", 	mtk::toString(2.0 * (MaxIntensity->getValue()/65535.0)), 	URL);
    URL = replaceSubstring("X_CENTER", 			mtk::toString(xCenter), 					                URL);
    URL = replaceSubstring("Y_CENTER", 			mtk::toString(yCenter), 					                URL);
    URL = replaceSubstring("Z_VALUE", 			mtk::toString(getCurrentZ()), 	 			                URL);
    URL = replaceSubstring("ZOOM_FACTOR", 		mtk::toString(0.5*(1.0/mScaleE->getValue())), 	 			URL);
	return URL;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CheckAll1Click(TObject *Sender)
{
	TCheckListBox* lb = dynamic_cast<TCheckListBox*>(ZsPopUpMenu->PopupComponent);

    if(!lb)
    {
    	return;
    }

    for(int i = 0; i < lb->Count; i++)
    {
    	lb->Checked[i] = true;
    }
}


void __fastcall TMainForm::UncheckAll1Click(TObject *Sender)
{
	TCheckListBox* lb = dynamic_cast<TCheckListBox*>(ZsPopUpMenu->PopupComponent);

    if(!lb)
    {
    	return;
    }

    for(int i = 0; i < lb->Count; i++)
    {
    	lb->Checked[i] = false;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Checkrange1Click(TObject *Sender)
{
	TSelectZsForm* sz = new TSelectZsForm(this);
    sz->populate(mZs);
    int mr = sz->ShowModal();

    if(mr == mrOk)
    {
    	//Check/Uncheck items
    	//First check all
        for(int i = 0; i < mZs->Count; i++)
	    {
    		mZs->Checked[i] = true;
	    }

		StringList us(sz->getUnSelected());

        //Then unselect the unselected ones
        for(int i = 0; i < mZs->Count; i++)
        {
        	string val(stdstr(mZs->Items->Strings[i]));
        	if(us.contains(val))
            {
				mZs->Checked[i] = false;
            }
        }
    }
    delete sz;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Exit1Click(TObject *Sender)
{
	Close();
}


