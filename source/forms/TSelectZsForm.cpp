#include <vcl.h>
#pragma hdrstop
#include "TSelectZsForm.h"
#include "mtkVCLUtils.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

using namespace mtk;

TSelectZsForm *SelectZsForm;
//---------------------------------------------------------------------------
__fastcall TSelectZsForm::TSelectZsForm(TComponent* Owner)
	: TForm(Owner)
{}

void TSelectZsForm::populate(TCheckListBox* s)
{
	for(int i = 0; i < s->Count; i++)
    {
        if(s->Checked[i])
        {
        	SelectedItems->Items->Add(s->Items->Strings[i]);
        }
        else
        {
	       	UnSelectedItems->Items->Add(s->Items->Strings[i]);
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TSelectZsForm::MoveItems(TObject *Sender)
{
	TButton* b = dynamic_cast<TButton*>(Sender);
    if(b == MoveToUnselectLBBtn)
    {
    	int selectedAfter;
    	for(int i = 0; i < SelectedItems->Count; i++)
    	{
    		if(SelectedItems->Selected[i])
	        {
 				String item = SelectedItems->Items->Strings[i];
        		UnSelectedItems->AddItem(item, NULL);
	            selectedAfter = i;
    	    }
    	}

    	int selCount = SelectedItems->SelCount;
	    SelectedItems->DeleteSelected();

    	int newlySelected = selectedAfter - (selCount -1);
        if(newlySelected > -1 && newlySelected < SelectedItems->Count)
        {
            SelectedItems->Selected[newlySelected] = true;
            SelectedItems->ItemIndex = newlySelected;
        }

	    sortTListBoxNumerically(UnSelectedItems);
    }
    else if(b == MoveToSelectedLBBtn)
    {
        for(int i = 0; i < UnSelectedItems->Count; i++)
        {
            if(UnSelectedItems->Selected[i])
            {
                String item = UnSelectedItems->Items->Strings[i];
                SelectedItems->AddItem(item, NULL);
            }
        }
        UnSelectedItems->DeleteSelected();
        sortTListBoxNumerically(SelectedItems);
    }

	MoveToSelectedLBBtn->Enabled = (UnSelectedItems->SelCount > 0) ? true : false;
	MoveToUnselectLBBtn->Enabled = (SelectedItems->SelCount > 0) ? true : false;
}

//---------------------------------------------------------------------------
void __fastcall TSelectZsForm::ItemsClick(TObject *Sender)
{
    TListBox* lb = dynamic_cast<TListBox*>(Sender);

    if(lb == SelectedItems)
    {
		MoveToUnselectLBBtn->Enabled = (SelectedItems->SelCount > 0) ? true : false;
    }
    else if(lb == UnSelectedItems)
    {
		MoveToSelectedLBBtn->Enabled = (UnSelectedItems->SelCount > 0) ? true : false;
    }
}

StringList TSelectZsForm::getUnSelected()
{
	StringList us;

    for(int i = 0; i < UnSelectedItems->Count; i++)
    {
    	string item = stdstr(UnSelectedItems->Items->Strings[i]);
        us.append(item);
    }
	return us;
}
