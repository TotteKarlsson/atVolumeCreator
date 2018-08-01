#ifndef TAffineTransformationFrameH
#define TAffineTransformationFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "dslTIntegerLabeledEdit.h"
#include "dslTSTDStringLabeledEdit.h"
#include "TRenderPythonRemoteScriptFrame.h"
#include <Vcl.CheckLst.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "dslTFloatLabeledEdit.h"
#include "dslTPropertyCheckBox.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------

class TAffineTransformationFrame : public TRenderPythonRemoteScriptFrame
{
    __published:	// IDE-managed Components
        TPropertyCheckBox *AppendToCurrentStackCB;
	TFloatLabeledEdit *RotationE;
	TIntegerLabeledEdit *TranslateXE;
	TIntegerLabeledEdit *TranslateYE;
	TPopupMenu *StacksPopup;
	TActionList *StacksActions;
	TAction *DeleteStackA;
	TMenuItem *DeleteStack1;
        void __fastcall ExecuteBtnClick(TObject *Sender);
	void __fastcall DeleteStackAExecute(TObject *Sender);
	void __fastcall StacksPopupPopup(TObject *Sender);

    private:

    public:
        __fastcall TAffineTransformationFrame(TComponent* Owner);
};

extern PACKAGE TAffineTransformationFrame *AffineTransformationFrame;
#endif
