#ifndef atApplicationSupportFunctionsH
#define atApplicationSupportFunctionsH
#include <windows.h>
//---------------------------------------------------------------------------

void 			setupLogging();
void 			loadStyles();
void 			setupApplicationTheme();
int __stdcall 	FindOtherWindow(HWND hwnd, LPARAM lParam) ;


#endif
