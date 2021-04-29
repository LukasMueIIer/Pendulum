#include "Visualisation.h"
LRESULT Visu::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) { //Erstellt die Factory
			return -1;
		}
		CreateGraphicResources();
		return 0;
		break;
	case WM_PAINT:
		OnPaint();
		return 0;
	default:
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	break;
	}
};

HRESULT  Visu::CreateGraphicResources() {  //Hier alle Resourcen erstellen die man später nurnoch verwendet
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL) { //Nur ausführen wenn Render Target noch nicht existiert
		RECT rc; //Rechteck -> Soll Fenster bereich darstellen
		GetClientRect(m_hwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		hr = pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, size), &pRenderTarget); // erstelle das Render Target
		if (SUCCEEDED(hr)) { //nur weiter machen wenn alles geklappt hat
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0.f); //Farbe für den Brush
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
		}

	}
	return hr;
};

void  Visu::OnPaint() {

	//Setup
	HRESULT hr; //standard um erfolg etc zu checken
	D2D1_SIZE_F size = pRenderTarget->GetSize();
	PAINTSTRUCT ps; //Braucht man hald irgendwie lul
	BeginPaint(m_hwnd, &ps);

	//Drawing Part
	pRenderTarget->BeginDraw(); //Los gehts mit Zeichnen yayyy
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::AntiqueWhite)); //Macht das ganze Fenster AntiqueWhite

	//Cleanup
	hr = pRenderTarget->EndDraw();
	if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
		DiscardGraphicResources(); // Window ist invalide und jetzt muss man alles was von der Factory erstellt wurde discarden
	}
	EndPaint(m_hwnd, &ps);
};

void  Visu::DiscardGraphicResources() {
	SafeRelease(&pRenderTarget); //sagt dem system es kann diese Objekte Löschen
	SafeRelease(&pBrush);    //danach werden Pointer invalide -> safe release checkt dies um keine exception 
}