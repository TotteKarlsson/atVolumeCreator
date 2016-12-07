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

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TCheckListBox *mZs;
	TGroupBox *GroupBox1;
	TSTDStringLabeledEdit *STDStringLabeledEdit1;
	TSTDStringLabeledEdit *mStackNameE;
	TButton *Button2;
	TMemo *infoMemo;
	TIdHTTP *IdHTTP1;
	TIntegerLabeledEdit *mZMin;
	TIntegerLabeledEdit *mZMax;
	TTimer *mShutDownTimer;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TGroupBox *GroupBox2;
	TFloatLabeledEdit *mScaleE;
	TButton *mSelectZBtn;
	TSTDStringLabeledEdit *mBaseUrlE;
	TSTDStringLabeledEdit *mOwnerE;
	TSTDStringLabeledEdit *mProjectE;
	TRadioGroup *mImageFormat;
	TSplitter *Splitter1;
	TIntegerLabeledEdit *mHeight;
	TIntegerLabeledEdit *mWidth;
	TIntegerLabeledEdit *mYCoord;
	TIntegerLabeledEdit *mXCoord;
	TProgressBar *ProgressBar1;
	TPanel *Panel1;
	TPanel *Panel2;
	void __fastcall mURLKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall mZsClick(TObject *Sender);
	void __fastcall mZMaxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
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







private:	// User declarations
	void	    	UpdateZList();
        TThreadMethod                                   logMsgMethod;
        void __fastcall                                 logMsg();
		LogFileReader                                   mLogFileReader;
		mtk::Property<mtk::LogLevel>	                mLogLevel;
		void											initForm();
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
