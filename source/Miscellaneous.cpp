#include <vcl.h>
#pragma hdrstop
#include "TMainForm.h"
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
#include "atRenderClient.h"
#include "atROIHistory.h"
#include "mtkRestartApplicationUtils.h"

using namespace mtk;
extern string gAppName;
extern string gApplicationStyle;
extern string gRestartMutexName;


void __fastcall TMainForm::logMsg()
{
    infoMemo->Lines->Add(vclstr(mLogFileReader.getData()));
    mLogFileReader.purge();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::About1Click(TObject *Sender)
{
	MessageDlg("About Volume Creator", mtWarning, TMsgDlgButtons() << mbOK, 0);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mShutDownTimerTimer(TObject *Sender)
{
	mShutDownTimer->Enabled = false;

	if(mLogFileReader.isRunning())
	{
		Log(lDebug) << "Shutting down log file reader";
		mLogFileReader.stop();
	}

	if(TSSHFrame1->isConnected())
    {
		TSSHFrame1->disconnect();
	}

    Close();
}

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    IdHTTP1->Disconnect();
	Log(lInfo) << "In FormClose";
	mIniFileC->clear();
	Log(lInfo) << "In main forms destructor";

	//Save project history
	mBottomPanelHeight          	= mBottomPanel->Height;

	mGeneralProperties.write();

	//Write to file
	mIniFileC->save();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	Log(lInfo) << "Closing down....";

	//Check if we can close.. abort all threads..
	if(mLogFileReader.isRunning())
    {
    	CanClose = false;
    }

	if(TSSHFrame1->isConnected())
    {
		CanClose = false;
    }

	if(CanClose == false)
	{
		mShutDownTimer->Enabled = true;
	}
}



//---------------------------------------------------------------------------
void __fastcall TMainForm::populateStyleMenu()
{
    //Populate styles menu
    string themeFolder("themes");
    themeFolder = joinPath(getCWD(), themeFolder);

    //Populate menu with styles in the style manager
    //Add to menu
    System::DynamicArray<System::UnicodeString> aList = TStyleManager::StyleNames;
    String activeStyle = TStyleManager::ActiveStyle->Name;
    for(int i = 0; i < aList.Length; i++)
    {
        String name = TStyleManager::StyleNames[i];

        TMenuItem *Item = new TMenuItem(ThemesMenu);
        Item->Caption = name;
        Item->OnClick = ThemesMenuClick;
        ThemesMenu->Add(Item);
        if(Item->Caption == activeStyle)
        {
            Item->Checked = true;
        }
    }
}

void __fastcall TMainForm::ThemesMenuClick(TObject *Sender)
{
    TMenuItem* anItem = dynamic_cast<TMenuItem*>(Sender);
    if(!anItem)
    {
        return;
    }

    TReplaceFlags rFlags(rfIgnoreCase|rfReplaceAll);
    String styleName = StringReplace(anItem->Caption, "&", "", rFlags);

    int mrResult = MessageDlg("Changing theme require restart of the Application.\nRestart?", mtCustom, TMsgDlgButtons() << mbOK<<mbCancel, 0);

    if(mrResult == mrOk)
    {
        if (!ActivateApplicationStyleChange(gRestartMutexName, stdstr(styleName)))
        {
            ::MessageBox(NULL, TEXT("Something Wrong"),
                         TEXT("Restart App"),
                         MB_OK|MB_ICONEXCLAMATION);
            return ;
        }

        gApplicationStyle = stdstr(styleName);
        //Write to registry
        writeStringToRegistry(gApplicationRegistryRoot, "", "Theme", gApplicationStyle);

        // Terminate application.
        Close();
    }
}



