#include <vcl.h>
#pragma hdrstop
#include "atImageForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TImageForm *ImageForm;
//---------------------------------------------------------------------------

//  TRegistryForm(const string& regRoot, const string& formName, TComponent* Owner);
__fastcall TImageForm::TImageForm(const string& regRoot, const string& fName, TComponent* Owner)
	: TRegistryForm(regRoot, fName, Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TImageForm::FormClose(TObject *Sender, TCloseAction &Action)

{
	Action = caFree;
}
//---------------------------------------------------------------------------

