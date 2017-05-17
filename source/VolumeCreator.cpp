#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <string>
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
#include "mtkRestartApplicationUtils.h"
//---------------------------------------------------------------------------

using namespace mtk;

using std::string;
USEFORM("forms\TSelectZsForm.cpp", SelectZsForm);
USEFORM("P:\libs\atapi\source\vcl\frames\TSSHFrame.cpp", SSHFrame); /* TFrame: File Type */
USEFORM("TImageControlsFrame.cpp", ImageControlsFrame); /* TFrame: File Type */
USEFORM("TMainForm.cpp", MainForm);
USEFORM("atImageForm.cpp", ImageForm);
//---------------------------------------------------------------------------
extern string		gAppName					= "VolumeCreator";
extern string       gLogFileName                = "VolumeCreator.log";
extern string       gAppDataLocation            = joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), gAppName);
extern string 		gApplicationRegistryRoot  	= "\\Software\\Allen Institute\\VolumeCreator\\0.5.0";
extern string       gApplicationStyle           = "Auric";
extern string       gApplicationMutexName       = "VolumeCreatorMutex";

static HWND         gOtherAppWindow             = NULL;

extern string       gRestartMutexName           = "VolumeCreatorRestartMutex";

void setupLogging();
void loadStyles();
void setupApplicationTheme();
BOOL CALLBACK FindOtherWindow(HWND hwnd, LPARAM lParam) ;

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    //The app mutex is used to check for already running instances
    HANDLE appMutex;

	try
	{
  		// Initialize restart code
		// Check if this instance is restarted and
		// wait while previos instance finish
		if (mtk::checkForCommandLineFlag("--Restart"))
		{
            //TODO: Fix this.. not working properly..
            //            MessageDlg("Wait...", mtWarning, TMsgDlgButtons() << mbOK, 0);
			mtk::WaitForPreviousProcessToFinish(gRestartMutexName);
            Sleep(1000);
		}

        //Look at this later... does not work yet
        appMutex = ::CreateMutexA(NULL, FALSE, gApplicationMutexName.c_str());
        if( ERROR_ALREADY_EXISTS == GetLastError() )
        {
            // The Program is already running somewhere
            MessageDlg("It seems VolumeCreator is already running!\nOnly one instance can be running at any one time..", mtWarning, TMsgDlgButtons() << mbOK, 0);
            ::EnumWindows(FindOtherWindow, NULL);

            if(gOtherAppWindow != NULL)
            {
                //Send a custom message to restore window here..
            }

            return(1); // Exit program
        }

		Application->Initialize();
		Application->MainFormOnTaskBar = true;
        setupLogging();

        //Load Styles from files
        loadStyles();
        setupApplicationTheme();

        if(gApplicationStyle != "Windows")
	    {
        	TStyleManager::TrySetStyle(gApplicationStyle.c_str());
    	}

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
void setupApplicationTheme()
{
	if(mtk::checkForCommandLineFlag("-Theme="))
	{
		string cmdLine = stdstr(GetCommandLineA());
		//User is changing the theme.
		//Parse the command line
		StringList paras(cmdLine,'-');

		//Create iniKeys for easy parsing
		for(int i = 0; i < paras.size(); i++)
		{
			string record = paras[i];
			IniKey aKey(record);
			if(aKey.mKey == "Theme")
			{
				 gApplicationStyle = aKey.mValue;
			}
		}
	}
	else
	{
		//Read from registry
		gApplicationStyle = readStringFromRegistry(gApplicationRegistryRoot, "", "Theme", "Windows");
	}

	if(gApplicationStyle.size())
	{
		try
		{
			if(gApplicationStyle != "Windows")
			{
				TStyleManager::TrySetStyle(gApplicationStyle.c_str());
			}
		}
		catch(...)
		{
			//Do nothing
		}
	}
}

void loadStyles()
{
	string themeFolder("themes");
	themeFolder = joinPath(getCWD(), themeFolder);
	if(DirectoryExists(themeFolder.c_str()))
	{
		StringList list = getFilesInDir(themeFolder, "vsf");
		for(int i = 0; i < list.size(); i++)
		{
			string styleFile(list[i]);
			try
			{
				if(TStyleManager::IsValidStyle(vclstr(styleFile)))
				{
					TStyleManager::LoadFromFile(vclstr(styleFile));
				}
			}
			catch(...)
			{
				MessageDlg("Bad theme file", mtWarning, TMsgDlgButtons() << mbOK, 0);
			}
		}
	}
}

BOOL CALLBACK FindOtherWindow(HWND hwnd, LPARAM lParam)
{
	static char buffer[50];
	GetWindowTextA(hwnd, buffer, 50);

	string wName(buffer);
	if(contains(buffer, gAppName))
	{
		// do something with hwnd here
		gOtherAppWindow = hwnd;
		return FALSE;
	}

	return TRUE;
}




//---------------------------------------------------------------------------
#pragma comment(lib, "mtkCommon")
#pragma comment(lib, "mtkMath")
#pragma comment(lib, "poco_foundation-static.lib")
#pragma comment(lib, "libcurl.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "DuneForms.bpi")

