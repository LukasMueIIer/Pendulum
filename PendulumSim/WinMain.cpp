#include <Windows.h>
#include <d2d1.h>
#include <tchar.h>

#include "BaseWin.h"
#include "Sync_Thread.h"
#include "Visualisation.h"

#pragma comment(lib, "d2d1")  //included die .lib datei

#define m 1; //masse
#define I 1; //Flächenträgheitsmoment(incl Steiner anteil)


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Visu MainVis;

	if (!MainVis.Create(L"Pendulum", WS_OVERLAPPEDWINDOW)) { //erstelle das Fenster, registrieren , etc.
		return -1;
	}
	ShowWindow(MainVis.Window(), nCmdShow); //Zeige das Fenster an
	

	//Msg loop were in the endgame now
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
};