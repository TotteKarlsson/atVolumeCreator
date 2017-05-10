#ifndef TMainFormH
#define TMainFormH
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
#include "TSSHFrame.h"
#include "TImageControlsFrame.h"

class TImageForm;
using mtk::Process;
//---------------------------------------------------------------------------
using mtk::IniFileProperties;
using mtk::TRegistryProperties;
extern string gApplicationRegistryRoot;
void brightnessContrast(TImage *imageSelected);

class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TMemo *infoMemo;
	TIdHTTP *IdHTTP1;
	TTimer *mShutDownTimer;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TGroupBox *Zs_GB;
	TFloatLabeledEdit *mScaleE;
	TSplitter *Splitter1;
	TPanel *mBottomPanel;
	TGroupBox *imageParasGB;
	TButton *mResetButton;
	TButton *mHistoryBackBtn;
	TButton *mHistoryFFW;
	TPanel *mLeftPanel;
	TPaintBox *PaintBox1;
	mtkIniFileC *mIniFileC;
	TPanel *Panel2;
	TButton *mCLearMemo;
	TSplitter *Splitter2;
	TPopupMenu *ZsPopUpMenu;
	TMenuItem *CopyValidZs1;
	TButton *mZoomOutBtn;
	TButton *mZoomInBtn;
	TIntegerLabeledEdit *Width;
	TIntegerLabeledEdit *Height;
	TIntegerLabeledEdit *XCoord;
	TIntegerLabeledEdit *YCoord;
	TButton *mCloseBottomPanelBtn;
	mtkIntEdit *mZoomFactor;
	TButton *mShowBottomPanelBtn;
	TTabSheet *TabSheet3;
	TButton *mBrowseForCacheFolder;
	TSTDStringLabeledEdit *mImageCacheFolderE;
	TGroupBox *GroupBox6;
	TPanel *mLogPanel;
	TPanel *Panel5;
	TIntLabel *mXC;
	TIntLabel *mYC;
	TIntLabel *mX;
	TIntLabel *mY;
	TGroupBox *GroupBox8;
	TGroupBox *GroupBox9;
	TButton *mFetchSelectedZsBtn;
	TSSHFrame *TSSHFrame1;
	TTabSheet *TabSheet4;
	TGroupBox *StackGenerationGB;
	TButton *Run;
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *Help1;
	TMenuItem *About1;
	TGroupBox *GroupBox4;
	TSTDStringLabeledEdit *mBaseUrlE;
	TTabSheet *TabSheet5;
	TMemo *BashScriptMemo;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TComboBox *OwnerCB;
	TComboBox *ProjectCB;
	TComboBox *StackCB;
	TGroupBox *TestSSHGB;
	TButton *CMDButton;
	TEdit *mCMD;
	TTimer *CreateCacheTimer;
	TIntegerLabeledEdit *MaxIntensity;
	TIntegerLabeledEdit *MinIntensity;
	TScrollBox *ScrollBox1;
	TGroupBox *CacheGB;
	TGroupBox *PostProcessingGB;
	TCheckBox *ApplyPostProcCB;
	TCheckBox *IMContrastControl;
	TCheckListBox *StacksForProjectCB;
	TGroupBox *MultiStackCreationGB;
	TPanel *Panel3;
	TPropertyCheckBox *BoundsCB;
	TFloatLabeledEdit *VolumesScaleE;
	TSTDStringLabeledEdit *VolumesFolder;
	TSTDStringLabeledEdit *SubFolder1;
	TGroupBox *GroupBox2;
	TButton *OpenInNDVIZBtn;
	TPopupMenu *ImagePopup;
	TMenuItem *ParseNDVIZURL1;
	TButton *OpenFromNDVIZBtn;
	TCheckListBox *mZs;
	TMenuItem *CheckAll1;
	TMenuItem *UncheckAll1;
	TMenuItem *Checkrange1;
	TMenuItem *Exit1;
	TGroupBox *GroupBox3;
	TPropertyCheckBox *AcqFilterCB;
	TPropertyCheckBox *FlatFieldFilterCB;
	TPropertyCheckBox *StitchedFilterCB;
	TPropertyCheckBox *RegisteredFilterCB;
	TPropertyCheckBox *RoughAlignedFilterCB;
	TPropertyCheckBox *FineAlignedFilterCB;
	void __fastcall ClickZ(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall mShutDownTimerTimer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall mScaleEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
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
	void __fastcall mGetValidZsBtnClick(TObject *Sender);
	void __fastcall mBrowseForCacheFolderClick(TObject *Sender);
	void __fastcall mCLearMemoClick(TObject *Sender);
	void __fastcall mUpdateZsBtnClick(TObject *Sender);
	void __fastcall CopyValidZs1Click(TObject *Sender);
	void __fastcall GetOptimalBoundsBtnClick(TObject *Sender);
	void __fastcall mZoomBtnClick(TObject *Sender);
	void __fastcall OwnerCBChange(TObject *Sender);
	void __fastcall ProjectCBChange(TObject *Sender);
	void __fastcall mDetachBtnClick(TObject *Sender);
	void __fastcall mCloseBottomPanelBtnClick(TObject *Sender);
	void __fastcall mShowBottomPanelBtnClick(TObject *Sender);
	void __fastcall StackCBChange(TObject *Sender);
	void __fastcall TSSHFrame1ScSSHShell1AsyncReceive(TObject *Sender);
	void __fastcall CMDButtonClick(TObject *Sender);
	void __fastcall RunClick(TObject *Sender);
	void __fastcall About1Click(TObject *Sender);
	void __fastcall TSSHFrame1ScSSHClientAfterConnect(TObject *Sender);
	void __fastcall TSSHFrame1ScSSHClientAfterDisconnect(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CreateCacheTimerTimer(TObject *Sender);
	void __fastcall OpenInNDVIZBtnClick(TObject *Sender);
	void __fastcall IntensityKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ParseNDVIZURL1Click(TObject *Sender);
	void __fastcall CreateNDVIZURL1Click(TObject *Sender);
	void __fastcall CheckAll1Click(TObject *Sender);
	void __fastcall UncheckAll1Click(TObject *Sender);
	void __fastcall Checkrange1Click(TObject *Sender);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall StackFilterCBClick(TObject *Sender);

	private:	// User declarations
       	void __fastcall 								DrawShape(TPoint TopLeft, TPoint BottomRight, TPenMode AMode);
        RenderClient									mRC;
        int												getCurrentZ();
		TImageForm*										mImageForm;
		bool        									mRenderEnabled;

        void __fastcall                                 logMsg();
		LogFileReader                                   mLogFileReader;

        TApplicationProperties                          mAppProperties;
        IniFileProperties	      	                    mGeneralProperties;
        mtk::Property<int>	                            mBottomPanelHeight;
		mtk::Property<mtk::LogLevel>	                mLogLevel;
		mtk::Property<string>	 		                mCurrentProject;
		mtk::Property<string>	 		                mCurrentOwner;
		mtk::Property<string>	 		                mCurrentStack;


        bool                                            setupAndReadIniParameters();
        void                                            setupIniFile();
		double 											getImageStretchFactor();
        void											updateScale();

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
        string											createNDVIZURL();
		void __fastcall 								onImage();

        //Remote jobs
		string 											createRemoteCommand(const string& remoteScript, const string& stack);
		string 											createEmptyScriptFileOnServer(const string& stack);
		bool                							populateRemoteScript(const string& script);
        void 											runJob(const string& job);

public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};

extern PACKAGE TMainForm *MainForm;
#endif
