// Thomas Goussaert
// Bart Uyttenhove 
//
#include "stdafx.h"
// Visual Leak Detector VLD
// https://github.com/KindDragon/vld/wiki/Using-Visual-Leak-Detector
#include <vld.h>

//ReportLiveObjects:
//To use any of these GUID values, you must include Initguid.h before you include DXGIDebug.h in your code.
#include <Initguid.h>
#include <dxgidebug.h>

#include "Core.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PWSTR cmdLine, int showCmd)
{

#if defined(DEBUG) | defined(_DEBUG)

  	//notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// DirectX Debug interface
	typedef HRESULT(__stdcall *fPtr)(const IID&, void**);
	HMODULE hDll = LoadLibrary(L"dxgidebug.dll");
	fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");
	IDXGIDebug* pDXGIDebug{};
	DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&pDXGIDebug);

#endif

	std::unique_ptr<Core> pGame{ std::make_unique<Core>() };
	pGame->RunGame(hInstance);
	pGame.reset(); // deletes the object

#if defined(DEBUG) | defined(_DEBUG)
	// unresolved external  
	if (pDXGIDebug) pDXGIDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
	pDXGIDebug->Release();
#endif

	return 0;
}
