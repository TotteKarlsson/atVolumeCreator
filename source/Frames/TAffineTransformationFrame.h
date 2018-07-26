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
//---------------------------------------------------------------------------

class TAffineTransformationFrame : public TRenderPythonRemoteScriptFrame
{
    __published:	// IDE-managed Components
        TPropertyCheckBox *AppendToCurrentStackCB;
        TSTDStringLabeledEdit *TransformID;
	TIntegerLabeledEdit *PoolSizeE;
	TFloatLabeledEdit *RotationE;
	TIntegerLabeledEdit *TranslateXE;
	TIntegerLabeledEdit *TranslateYE;
        void __fastcall ExecuteBtnClick(TObject *Sender);

    private:

    public:
        __fastcall TAffineTransformationFrame(TComponent* Owner);
};

extern PACKAGE TAffineTransformationFrame *AffineTransformationFrame;
#endif
