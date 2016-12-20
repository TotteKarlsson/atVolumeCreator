#ifndef TMainFormH
#define TMainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include "TFloatLabeledEdit.h"
#include "TIntegerLabeledEdit.h"
#include "TSTDStringLabeledEdit.h"
#include <Vcl.CheckLst.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include "mtkLogFileReader.h"
#include "mtkLogLevel.h"
#include <Vcl.ComCtrls.hpp>
#include "TIntLabel.h"
#include "TPropertyCheckBox.h"
#include "atRenderService.h"
#include "atROIHistory.h"
#include "mtkProcess.h"
#include "mtkIniFileProperties.h"
#include "TRegistryProperties.h"
#include "mtkIniFileC.h"
#include "TApplicationProperties.h"
#include "atFetchImagesThread.h"

using mtk::Process;
//---------------------------------------------------------------------------
using mtk::IniFileProperties;
using mtk::TRegistryProperties;

extern string gApplicationRegistryRoot;
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TGroupBox *StackGenGB;
	TSTDStringLabeledEdit *mVolumesFolder;
	TSTDStringLabeledEdit *mStackNameE;
	TButton *mGenerateTiffStackBtn;
	TMemo *infoMemo;
	TIdHTTP *IdHTTP1;
	TIntegerLabeledEdit *mZMinE;
	TIntegerLabeledEdit *mZMaxE;
	TTimer *mShutDownTimer;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TGroupBox *ZsBG;
	TFloatLabeledEdit *mScaleE;
	TSTDStringLabeledEdit *mBaseUrlE;
	TSTDStringLabeledEdit *mOwnerE;
	TSTDStringLabeledEdit *mProjectE;
	TRadioGroup *mImageFormat;
	TSplitter *Splitter1;
	TIntegerLabeledEdit *mHeightE;
	TIntegerLabeledEdit *mWidthE;
	TIntegerLabeledEdit *mYCoordE;
	TIntegerLabeledEdit *mXCoordE;
	TProgressBar *ProgressBar1;
	TPanel *mBottomPanel;
	TGroupBox *GroupBox3;
	TButton *mResetButton;
	TButton *mHistoryBackBtn;
	TButton *mHistoryFFW;
	TPanel *Panel3;
	TPanel *Panel4;
	TButton *mNextZ;
	TButton *mPrevZ;
	TPanel *Panel5;
	TPaintBox *PaintBox1;
	TListBox *mDeSelectedZs;
	TPanel *Panel6;
	TButton *mMoveOutSelectedBtn;
	TButton *mRestoreUnselectedBtn;
	TListBox *mZs;
	TPanel *Panel7;
	TPanel *Panel8;
	TPanel *Panel9;
	mtkIniFileC *mIniFileC;
	TButton *mGenerateZSerieBtn;
	TSTDStringLabeledEdit *mCustomZsE;
	TButton *mAddCustomZs;
	TGroupBox *CacheGB;
	TButton *mFetchSelectedZsBtn;
	TPanel *Panel1;
	TIntLabel *mXC;
	TIntLabel *mYC;
	TIntLabel *mX;
	TIntLabel *mY;
	TPropertyCheckBox *mStretchCB;
	TSTDStringLabeledEdit *mImageCacheFolderE;
	TButton *mBrowseForCacheFolder;
	TIntegerLabeledEdit *mZStep;
	void __fastcall ClickZ(TObject *Sender);
	void __fastcall mZMaxEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall mShutDownTimerTimer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall mScaleEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall mSelectZBtnClick(TObject *Sender);
	void __fastcall IdHTTP1Work(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCount);
	void __fastcall IdHTTP1WorkBegin(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCountMax);
	void __fastcall IdHTTP1Status(TObject *ASender, const TIdStatus AStatus, const UnicodeString AStatusText);
	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall mStretchCBClick(TObject *Sender);

	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall resetButtonClick(TObject *Sender);
	void __fastcall historyBtnClick(TObject *Sender);
	void __fastcall TraverseZClick(TObject *Sender);
	void __fastcall mFetchSelectedZsBtnClick(TObject *Sender);
	void __fastcall mMoveOutSelectedBtnClick(TObject *Sender);
	void __fastcall mRestoreUnselectedBtnClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall mGenerateZSerieBtnClick(TObject *Sender);
	void __fastcall mBrowseForCacheFolderClick(TObject *Sender);
	void __fastcall mGenerateTiffStackBtnClick(TObject *Sender);

	private:	// User declarations
		void	    									UpdateZList();
       	void __fastcall 								DrawShape(TPoint TopLeft, TPoint BottomRight, TPenMode AMode);
       	mtk::Process									mTiffCP;
        TThreadMethod                                   logMsgMethod;
        void __fastcall                                 logMsg();
		LogFileReader                                   mLogFileReader;

        TApplicationProperties                          mAppProperties;
        IniFileProperties	      	                    mGeneralProperties;
        mtk::Property<int>	                            mBottomPanelHeight;

		mtk::Property<mtk::LogLevel>	                mLogLevel;

		bool 		__fastcall							addTiffToStack(const string& stackFName, const string& fName);
        bool                                            setupAndReadIniParameters();
        void                                            setupIniFile();
		void 		__fastcall 							processEvent(Process* proc);
		double 											getImageStretchFactor();

        //Drawing stuff
        TBrushStyle 									BrushStyle;
        TPenStyle 										PenStyle;
        int 											PenWide;
        bool 											Drawing;
        TPoint 											Origin;
        TPoint 											MovePt;
        TPoint											mTopLeftSelCorner;
        TPoint											mBottomRightSelCorner;
		void								            render(RenderBox* box = NULL);

        //Render areas history
	    ROIHistory										mROIHistory;
        RenderBox										mOriginalRB;
		RenderBox										mLastRB;
		RenderBox										mCurrentRB;
      	TCanvas*										getCanvas();

		FetchImagesThread								mCreateCacheThread;

public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};

extern PACKAGE TMainForm *MainForm;
#endif
