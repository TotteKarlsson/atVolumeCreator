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
#include "atRenderClient.h"
#include "atROIHistory.h"
#include "mtkProcess.h"
#include "mtkIniFileProperties.h"
#include "TRegistryProperties.h"
#include "mtkIniFileC.h"
#include "TApplicationProperties.h"
#include "atFetchImagesThread.h"
#include <Vcl.Menus.hpp>
#include "mtkIntEdit.h"

class TImageForm;
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
	TSTDStringLabeledEdit *mStackNameE;
	TButton *mGenerateTiffStackBtn;
	TMemo *infoMemo;
	TIdHTTP *IdHTTP1;
	TTimer *mShutDownTimer;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TGroupBox *ZsBG;
	TFloatLabeledEdit *mScaleE;
	TSTDStringLabeledEdit *mBaseUrlE;
	TSTDStringLabeledEdit *mOwnerE;
	TSTDStringLabeledEdit *mProjectE;
	TSplitter *Splitter1;
	TPanel *mBottomPanel;
	TGroupBox *GroupBox3;
	TButton *mResetButton;
	TButton *mHistoryBackBtn;
	TButton *mHistoryFFW;
	TPanel *Panel3;
	TPanel *mLeftPanel;
	TPaintBox *PaintBox1;
	TListBox *mZs;
	mtkIniFileC *mIniFileC;
	TPanel *Panel2;
	TButton *mCLearMemo;
	TSplitter *Splitter2;
	TPopupMenu *PopupMenu1;
	TMenuItem *CopyValidZs1;
	TGroupBox *GroupBox2;
	TButton *GetOptimalBoundsBtn;
	TButton *mGetListOfBoundsBtn;
	TButton *mZoomOutBtn;
	TButton *mZoomInBtn;
	TIntegerLabeledEdit *mWidthE;
	TIntegerLabeledEdit *mHeightE;
	TIntegerLabeledEdit *mXCoordE;
	TIntegerLabeledEdit *mYCoordE;
	TButton *mGetValidZsBtn;
	TPanel *mToppanel;
	TComboBox *mOwnersCB;
	TComboBox *mProjectsCB;
	TComboBox *mStacksCB;
	TButton *mCloseBottomPanelBtn;
	mtkIntEdit *mZoomFactor;
	TButton *mShowBottomPanelBtn;
	TGroupBox *GroupBox4;
	TFloatLabeledEdit *FloatLabeledEdit1;
	TButton *Button1;
	TIntegerLabeledEdit *IntegerLabeledEdit1;
	TIntegerLabeledEdit *IntegerLabeledEdit2;
	TIntegerLabeledEdit *IntegerLabeledEdit3;
	TIntegerLabeledEdit *IntegerLabeledEdit4;
	TTabSheet *TabSheet3;
	TButton *mBrowseForCacheFolder;
	TSTDStringLabeledEdit *mImageCacheFolderE;
	TGroupBox *GroupBox5;
	TSTDStringLabeledEdit *mVolumesFolder;
	TGroupBox *GroupBox6;
	TPanel *Panel6;
	TIntegerLabeledEdit *mZMaxE;
	TIntegerLabeledEdit *mZMinE;
	TButton *mGenerateZSerieBtn;
	TButton *mAddCustomZs;
	TIntegerLabeledEdit *mZStep;
	TPropertyCheckBox *mUseRenderBoundsCB;
	TPanel *mLogPanel;
	TPanel *Panel5;
	TIntLabel *mXC;
	TIntLabel *mYC;
	TIntLabel *mX;
	TIntLabel *mY;
	TPropertyCheckBox *mStretchCB;
	TGroupBox *GroupBox8;
	TGroupBox *GroupBox9;
	TPanel *Panel1;
	TGroupBox *GroupBox10;
	TButton *mFetchSelectedZsBtn;
	TPanel *Panel4;
	void __fastcall ClickZ(TObject *Sender);
	void __fastcall mZMaxEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall mShutDownTimerTimer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall mScaleEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

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
	void __fastcall mGetValidZsBtnClick(TObject *Sender);
	void __fastcall mGenerateZSerieBtnClick(TObject *Sender);
	void __fastcall mBrowseForCacheFolderClick(TObject *Sender);
	void __fastcall mGenerateTiffStackBtnClick(TObject *Sender);
	void __fastcall mCLearMemoClick(TObject *Sender);
	void __fastcall mUpdateZsBtnClick(TObject *Sender);
	void __fastcall CopyValidZs1Click(TObject *Sender);
	void __fastcall GetOptimalBoundsBtnClick(TObject *Sender);
	void __fastcall mValidZsLBClick(TObject *Sender);
	void __fastcall mZoomBtnClick(TObject *Sender);
	void __fastcall mOwnersCBChange(TObject *Sender);
	void __fastcall mProjectsCBChange(TObject *Sender);
	void __fastcall mDetachBtnClick(TObject *Sender);
	void __fastcall mCloseBottomPanelBtnClick(TObject *Sender);
	void __fastcall mShowBottomPanelBtnClick(TObject *Sender);
	void __fastcall mStacksCBChange(TObject *Sender);

	private:	// User declarations
		void	    									UpdateZList();
       	void __fastcall 								DrawShape(TPoint TopLeft, TPoint BottomRight, TPenMode AMode);
       	mtk::Process									mTiffCP;
        RenderClient									mRC;
        int												getCurrentZ();
		TImageForm*										mImageForm;

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
