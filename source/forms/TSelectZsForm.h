#ifndef TSelectZsFormH
#define TSelectZsFormH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include "mtkStringList.h"

using namespace mtk;

//---------------------------------------------------------------------------
class TSelectZsForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TListBox *SelectedItems;
	TListBox *UnSelectedItems;
	TSplitter *Splitter1;
	TPanel *Panel2;
	TButton *Button2;
	TButton *Button3;
	TPanel *Panel3;
	TButton *MoveToUnselectLBBtn;
	TButton *MoveToSelectedLBBtn;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	void __fastcall MoveItems(TObject *Sender);
	void __fastcall ItemsClick(TObject *Sender);
private:	// User declarations

	public:		// User declarations
		__fastcall TSelectZsForm(TComponent* Owner);
	    void	populate(TCheckListBox* b);
        StringList		getUnSelected();
};

extern PACKAGE TSelectZsForm *SelectZsForm;

#endif
