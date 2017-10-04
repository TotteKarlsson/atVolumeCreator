#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <string>
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
#include "atApplicationSupportFunctions.h"
#include "mtkRestartApplicationUtils.h"
//---------------------------------------------------------------------------

using namespace mtk;

using std::string;
USEFORM("Forms\TSelectZsForm.cpp", SelectZsForm);
USEFORM("Frames\TParaConverterFrame.cpp", ParaConverterFrame); /* TFrame: File Type */
USEFORM("Forms\TImageForm.cpp", ImageForm);
USEFORM("Forms\TOverlayedImage.cpp", OverlayedImage);
USEFORM("TMainForm.cpp", MainForm);
USEFORM("P:\libs\atapi\source\vcl\frames\TSSHFrame.cpp", SSHFrame); /* TFrame: File Type */
//---------------------------------------------------------------------------
extern string		gAppName					= "VolumeCreator";
extern string       gLogFileName                = "VolumeCreator.log";
extern string       gAppDataLocation            = joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), gAppName);
extern string 		gApplicationRegistryRoot  	= "\\Software\\Allen Institute\\VolumeCreator\\0.5.0";
extern string       gApplicationStyle           = "Cobalt XEMedia";
extern string       gApplicationMutexName       = "VolumeCreatorMutex";
extern HWND         gOtherAppWindow             = NULL;
extern string       gRestartMutexName           = "VolumeCreatorRestartMutex";

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
        if(loadStyles())
        {
        	setupApplicationTheme();
        }

		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TSSHFrame), &SSHFrame);
		Application->CreateForm(__classid(TOverlayedImage), &OverlayedImage);
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

//---------------------------------------------------------------------------
#pragma comment(lib, "mtkCommon")
#pragma comment(lib, "mtkMath")
#pragma comment(lib, "poco_foundation-static.lib")
#pragma comment(lib, "libcurl.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "DuneForms.bpi")
#pragma comment(lib, "atCore.lib")
#pragma comment(lib, "tinyxml2-static.lib")

