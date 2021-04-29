#pragma once
#include "BaseWin.h"
#include <Windows.h>
#include <d2d1.h>
#include <tchar.h>
#include <mutex>

constexpr float conv = 180.f / 3.14159265358979323846f; //von Rad zu Degree

class Visu : public BaseWindow<Visu> { 
public:
	ID2D1Factory * pFactory; //Factory um direct2d objekte zu erstellen
	ID2D1HwndRenderTarget * pRenderTarget; //wohin gerendert werden soll
	ID2D1SolidColorBrush * pBrush; //Pinsel um zu zeichnen
	std::mutex * pMutex; //Mutex für Winkel
	float * fAngle; //Zugriff auf winkel



	Visu() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL) {};

	void setVar(std::mutex * Mute, float * pFlot) { pMutex = Mute; fAngle = pFlot; };

	PCWSTR ClassName() const { return L"Visulator"; }

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	HRESULT CreateGraphicResources();

	void OnPaint();

	void DiscardGraphicResources();

};