#ifndef atApplicationSupportFunctionsH
#define atApplicationSupportFunctionsH
#include <windows.h>
//---------------------------------------------------------------------------

namespace System
{
	namespace Types
    {
		class TPoint;
	}
}

using System::Types::TPoint;

TPoint 			controlToImage(const TPoint& p, double scale, double stretchFactor);
void 			setupLogging();
void 			loadStyles();
void 			setupApplicationTheme();
int __stdcall 	FindOtherWindow(HWND hwnd, LPARAM lParam) ;


#endif
