#include <vcl.h>
#pragma hdrstop
#include "TParaConverterFrame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TIntegerLabeledEdit"
#pragma link "TSSHFrame"
#pragma link "TSTDStringLabeledEdit"
#pragma resource "*.dfm"
TParaConverterFrame *ParaConverterFrame;

//---------------------------------------------------------------------------
__fastcall TParaConverterFrame::TParaConverterFrame(TComponent* Owner)
	: TFrame(Owner)
{}

//---------------------------------------------------------------------------
void __fastcall TParaConverterFrame::TSSHFrame1ConnectBtnClick(TObject *Sender)
{
  TSSHFrame1->ConnectBtnClick(Sender);

}

