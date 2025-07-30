#include "pch.h"
#include "includes.h"

int windowHeight, windowWidth;

BOOL CALLBACK enumWind(HWND handle, LPARAM lp)
{
	DWORD procId;
	GetWindowThreadProcessId(handle, &procId);
	if (GetCurrentProcessId() != procId)
	{
		// keep enumerating
		return TRUE;
	}
	window = handle;
	// end the enumeration
	return FALSE;
}

HWND GetProcessWindow()
{
	window = NULL;
	// Enumerates all top-level windows on the screen by passing the handle to each window, in turn, to an application-defined callback function.
	// // EnumWindows continues until the last top-level window is enumerated or the callback function returns FALSE.
	EnumWindows(enumWind, NULL);
	RECT size;
	GetWindowRect(window, &size);
	windowWidth = size.right - size.left - 5;
	windowHeight = size.bottom - size.top - 29;

	return window;
}

// parameters
// pTable: pointer to a vTable
// size: size of the vTable in bytes
bool GetD3D9Device(void** pTable, size_t size)
{
	if (!pTable)
		return false;

	// Creates an IDirect3D9 interface
	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return false;

	IDirect3DDevice9* pDummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false; // attmepts fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // discard old frames
	d3dpp.hDeviceWindow = GetProcessWindow(); // window handle for the Direct3D device

	HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		d3dpp.hDeviceWindow,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&pDummyDevice
	);

	// Fallback to Windowed Mode if Fullscreen Fails
	if (dummyDevCreated != S_OK)
	{
		d3dpp.Windowed = !d3dpp.Windowed;
		HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			d3dpp.hDeviceWindow,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&pDummyDevice
		);

		if (dummyDevCreated != S_OK)
		{
			pD3D->Release();
			return false;
		}
	}

	// void*: member functions
	// void**: vTable
	// void***: pointer to the vTable
	memcpy(pTable, *(void***)(pDummyDevice), size);

	pDummyDevice->Release();
	pD3D->Release();
	return true;
}
