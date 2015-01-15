// migong11.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include <COMMCTRL.H>

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	//Enable IPAddress¡¢Calendar.etc
	InitCommonControls();

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, Main_Proc);
	return 0;
}



