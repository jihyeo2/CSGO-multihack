#pragma once

// HRESULT: represents a Windows Runtime error.
// If the method succeeds, the return value is D3D_OK.
typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);

bool GetD3D9Device(void** pTable, size_t size);

BOOL CALLBACK enumWind(HWND handle, LPARAM lp);

static HWND window;

HWND GetProcessWindow();

extern int windowHeight, windowWidth;

extern LPDIRECT3DDEVICE9 pDevice;

