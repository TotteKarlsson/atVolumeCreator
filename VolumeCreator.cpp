#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <string>
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
//---------------------------------------------------------------------------

using namespace mtk;
using std::string;
USEFORM("TMainForm.cpp", MainForm);
//---------------------------------------------------------------------------
extern string		gAppName					= "VolumeCreator";
extern string       gLogFileLocation            = "";
extern string       gLogFileName                = "volume_creator.log";
extern string 		gApplicationRegistryRoot  	= "\\Software\\Allen Institute\\VolumeCreator\\0.5.0";
void setupLogging();

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
        setupLogging();
		Application->CreateForm(__classid(TMainForm), &MainForm);
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
	//Get Application folder
	string fldr =  joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), gAppName);
	if(!folderExists(fldr))
	{
		createFolder(fldr);
	}

	gLogFileLocation = fldr;
	string fullLogFileName(joinPath(gLogFileLocation, gLogFileName));
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

