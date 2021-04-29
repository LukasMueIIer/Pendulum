#include <Windows.h>
#include <d2d1.h>
#include <tchar.h>
#include <mutex>
#include <math.h>
#include <thread>

#include "BaseWin.h"
#include "Sync_Thread.h"
#include "Visualisation.h"

#pragma comment(lib, "d2d1")  //included die .lib datei

constexpr float I = 1.f; //Flächenträgheitsmoment(incl Steiner anteil)
constexpr float l = 1.f; //länge
constexpr float a = 0.5f; //halbe länge
constexpr float alpha0 = 0; //initial angle
constexpr float w0 = 0; // initiale Winkelgeschw.
constexpr float dw0 = 0; //ableitung winkelgeschw.
constexpr float g = 9.81; //Gravitation
constexpr float m = 1.0f; //Masse
constexpr float conv = 180.f / 3.14159265358979323846f; //von Rad zu Degree

constexpr float cPow(float a, float b) { //wrapper um schon währen compiling auszuwerten
	return pow(a, b);
}


float alpha; //enthält den aktuellen winkel des Pendels für Zeichnung
std::mutex mute1; //Mutex for data swap;

void DataProducer(HWND mHwnd) {
	float alo = alpha0; //lokal angle
	float w = 0; //Winkelgeschw.
	float dw = 0; //abl. winkelgeschw.
	float fx, fy; //Forces from Hinge
	float fUnten, Salo,Calo;//
	float t0,t,dt;
	SYSTEMTIME st;

	GetSystemTime(&st); //startzeit
	t0 = st.wSecond + 0.001 * st.wMilliseconds;

	while (true) { //data calc loop as fast as posible
		//Calculate Force
		Salo = sin(alo);
		Calo = cos(alo);

		fUnten = cPow(a, 4) * cPow(m, 2) * Salo * Calo * (Calo + Salo) + cPow(a, 2) * I * m * (Calo + pow(Salo, 2)) + cPow(I, 2);
		if (fUnten == 0) { //über eventuelle singularitäten hinweg schwingen
			fx = 0;
			fy = 0;
		}
		else {
			fx = (cPow(a, 2) * m * Calo * (a*(cPow(a, 2) * m * (Calo - Salo)) + I) - g * I * m) / fUnten;
			fy = (m * (cPow(a, 3) * (cPow(a, 2) * m + I)) * Salo * pow(w, 2) + (cPow(a, 2) * m* pow(Salo, 2) + I)*g*I) / fUnten;
		}
		dw = fy * Calo * a - fx * Salo * a;

		//delta t berechnen
		GetSystemTime(&st);
		t = st.wSecond + 0.001 * st.wMilliseconds;
		dt = t - t0;
		std::swap(t, t0); //t auf t0 setzen

		//integrieren
		alo = alo + w * dt + 0.5 * dw * dt * dt;
		w = w + dw * dt;

		//neues alpha pushen
		mute1.lock();
		alpha = alo;
		mute1.unlock();

		SendMessage(mHwnd, WM_PAINT, 0, 0);

	}
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Visu MainVis;

	alpha = alpha0; //initial condition


	if (!MainVis.Create(L"Pendulum", WS_OVERLAPPEDWINDOW)) { //erstelle das Fenster, registrieren , etc.
		return -1;
	}
	ShowWindow(MainVis.Window(), nCmdShow); //Zeige das Fenster an
	
	MainVis.setMutex(&mute1); //Mutex an Window Stuckt übergeben

	std::thread t1 = std::thread(DataProducer, MainVis.Window()); //Starte den Data Producer Process
	t1.detach();


	//Msg loop were in the endgame now
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
};