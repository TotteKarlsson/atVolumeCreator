#ifndef TParaConverterFrameH
#define TParaConverterFrameH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TIntegerLabeledEdit.h"
#include "TSSHFrame.h"
#include "TSTDStringLabeledEdit.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class PACKAGE TParaConverterFrame : public TFrame
{
    __published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TIntegerLabeledEdit *NumberOfProcessesE;
        TIntegerLabeledEdit *BoxHeightE;
        TIntegerLabeledEdit *BoxWidthE;
        TIntegerLabeledEdit *BoxDepthE;
        TGroupBox *GroupBox2;
        TSTDStringLabeledEdit *InputFolderE;
        TGroupBox *GroupBox3;
        TSTDStringLabeledEdit *OutputFolderE;
//        TSSHFrame *TSSHFrame1;
        TGroupBox *RunGB;
        TButton *Button1;
	TSSHFrame *TSSHFrame1;
        void __fastcall TSSHFrame1ConnectBtnClick(TObject *Sender);
    private:

    public:
                            __fastcall TParaConverterFrame(TComponent* Owner);

};

extern PACKAGE TParaConverterFrame *ParaConverterFrame;
#endif
