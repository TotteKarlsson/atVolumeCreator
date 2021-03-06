#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <string>
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
#include "atApplicationSupportFunctions.h"
#include "dslRestartApplicationUtils.h"
#include "dslVCLUtils.h"
#include "dslLogger.h"
//---------------------------------------------------------------------------
using namespace dsl;

using std::string;
USEFORM("Frames\TRenderPythonRemoteScriptFrame.cpp", RenderPythonRemoteScriptFrame); /* TFrame: File Type */
USEFORM("Forms\TOverlayedImage.cpp", OverlayedImage);
USEFORM("Forms\TSelectZsForm.cpp", SelectZsForm);
USEFORM("Frames\TAffineTransformationFrame.cpp", AffineTransformationFrame); /* TFrame: File Type */
USEFORM("P:\libs\atapi\source\vcl\frames\TSSHFrame.cpp", SSHFrame); /* TFrame: File Type */
USEFORM("P:\libs\dsl\VCL\Frames\dslTLogMemoFrame.cpp", LogMemoFrame); /* TFrame: File Type */
USEFORM("TMainForm.cpp", MainForm);
USEFORM("Forms\TImageForm.cpp", ImageForm);
//---------------------------------------------------------------------------
extern string		gAppName					= "VolumeCreator";
extern string       gLogFileName                = "VolumeCreator.log";
extern string       gAppDataLocation            = joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), gAppName);
extern string 		gApplicationRegistryRoot  	= "\\Software\\Allen Institute\\VolumeCreator\\0.5.0";
extern string       gApplicationStyle           = "Iceberg Classico";
extern string       gApplicationMutexName       = "VolumeCreatorMutex";
extern HWND         gOtherAppWindow             = NULL;
extern string       gRestartMutexName           = "VolumeCreatorRestartMutex";

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    //The app mutex is used to check for already running instances
    HANDLE appMutex;
	try
	{

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

		gApplicationStyle = readStringFromRegistry(gApplicationRegistryRoot, "", "Theme",  gApplicationStyle);
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
        Application->Icon->LoadFromFile("VolumeCreator_Icon.ico");
        setupLogging();

		TStyleManager::TrySetStyle("Carbon");
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TSSHFrame), &SSHFrame);
		Application->CreateForm(__classid(TOverlayedImage), &OverlayedImage);
		Application->CreateForm(__classid(TLogMemoFrame), &LogMemoFrame);
		Application->Run();
		writeStringToRegistry(gApplicationRegistryRoot, "", "Theme",  gApplicationStyle);
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
#pragma comment(lib, "dslFoundation")
#pragma comment(lib, "dslVCLCommon.lib")
#pragma comment(lib, "dslVCLComponents.bpi")
#pragma comment(lib, "dslVCLVisualComponents.bpi")

#pragma comment(lib, "atFoundation.lib")
#pragma comment(lib, "atVCLCommon.lib")
#pragma comment(lib, "CORE_RL_MagickWand_B.lib")
#pragma comment(lib, "poco_foundation.lib")
#pragma comment(lib, "tinyxml2.lib")
#pragma comment(lib, "libcurl_imp.lib")
//#pragma comment(lib, "Ws2_32.lib")



