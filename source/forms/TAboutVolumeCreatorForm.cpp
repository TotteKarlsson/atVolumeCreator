#include <vcl.h>
#pragma hdrstop
#include "TAboutVolumeCreatorForm.h"
#include "mtkApplicationInfo.h"
#include "mtkLogger.h"
#include <sstream>
#include "mtkVersion.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TAboutVolumeCreator *AboutVolumeCreator;
using namespace std;
using namespace mtk;

//---------------------------------------------------------------------------
__fastcall TAboutVolumeCreator::TAboutVolumeCreator(TComponent* Owner)
	: TForm(Owner)
{}

//---------------------------------------------------------------------------
void __fastcall TAboutVolumeCreator::FormShow(TObject *Sender)
{
    stringstream ss;
    mtkApplicationInfo appInfo(Application);

    //Current Version Info
    Version version(stdstr(appInfo.mVersion));
    ss <<version.getMajor()<<"."<<version.getMinor()<<"."<<version.getPatch();
    String versionMajorMinorPatch(ss.str().c_str());
    versionLabel->Caption = String("Version: ") + versionMajorMinorPatch;
    Memo1->Lines->LoadFromFile("CHANGELOG.txt");
}

//---------------------------------------------------------------------------
void __fastcall TAboutVolumeCreator::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == vkEscape)
    {
    	this->Close();
    }
}


