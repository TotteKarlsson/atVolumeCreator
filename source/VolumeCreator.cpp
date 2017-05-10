#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <string>
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
//---------------------------------------------------------------------------

using namespace mtk;
using std::string;
USEFORM("P:\libs\atapi\source\vcl\frames\TSSHFrame.cpp", SSHFrame); /* TFrame: File Type */
USEFORM("TMainForm.cpp", MainForm);
USEFORM("atImageForm.cpp", ImageForm);
USEFORM("forms\TSelectZsForm.cpp", SelectZsForm);
//---------------------------------------------------------------------------
extern string		gAppName					= "VolumeCreator";
extern string       gLogFileName                = "VolumeCreator.log";
extern string       gAppDataLocation            = joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), gAppName);
extern string 		gApplicationRegistryRoot  	= "\\Software\\Allen Institute\\VolumeCreator\\0.5.0";
void setupLogging();

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
        setupLogging();
		TStyleManager::TrySetStyle("Auric");
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TSSHFrame), &SSHFrame);
		Application->CreateForm(__classid(TSelectZsForm), &SelectZsForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}

void setupLogging()
{
	if(!folderExists(gAppDataLocation))
	{
		createFolder(gAppDataLocation);
	}

	string fullLogFileName(joinPath(gAppDataLocation, gLogFileName));
	clearFile(fullLogFileName);
	mtk::gLogger.logToFile(fullLogFileName);
	LogOutput::mShowLogLevel = true;
	LogOutput::mShowLogTime = false;
	LogOutput::mUseLogTabs = false;
	Log(lInfo) << "Logger was setup";
}

//---------------------------------------------------------------------------
#pragma comment(lib, "mtkCommon")
#pragma comment(lib, "mtkMath")
#pragma comment(lib, "poco_foundation-static.lib")
#pragma comment(lib, "libcurl.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "DuneForms.bpi")

