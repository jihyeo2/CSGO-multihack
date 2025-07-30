#include "pch.h"
#include "includes.h"

extern Hack* hack;

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DRECT rect = { x, y, x + w, y + h };
	// IDirect3DDevice9::Clear
	// : Clears one or more surfaces such as a render target
	// 1st arg (DWORD): number of rectangles
	// 2nd arg (const D3DRECT): pointer to an array of D3DRECT structures
	// 3rd arg (DWORD): D3DCLEAR flags that specify the surface(s) that will be cleared (D3DCLEAR_TARGET clears just the color buffer--not depth or stencil)
	// 4th arg (D3DCOLOR)
	// 5th arg (float): Clear the depth buffer to this new z value which ranges from 0 to 1
	// 6th arg (DWORD): Clear the stencil buffer to this new value

	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);

	/*
	* If we want a crosshair

	D3DRECT crosshair[4] =
	{
		{ screenWidth / 2 - 5, screenHeight / 2, screenWidth / 2 + 5, screenHeight / 2 + 1 }, // Horizontal line
		{ screenWidth / 2, screenHeight / 2 - 5, screenWidth / 2 + 1, screenHeight / 2 + 5 } // Vertical line
	};

	for (int i = 0; i < 2; ++i
	{
		pDevice->Clear(1, &crosshair[i], D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 0, 0), 0, 0); // Red color
	}
	*/
}

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color) {
	if (!hack->LineL)
		D3DXCreateLine(pDevice, &hack->LineL);

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	hack->LineL->SetWidth(thickness);
	hack->LineL->Draw(Line, 2, color); // Draw() takes care of both Begin() and End(); 2nd arg: number of points
	//LineL->Release(); 
}

void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color)
{
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawEspBox2D(Vec2 top, Vec2 bot, int thickness, D3DCOLOR color)
{
	int height = ABS(top.y - bot.y);
	Vec2 tl, tr;
	tl.x = top.x - height / 4; // width = height / 4; this esp box is a 1:2 rectangle
	tr.x = top.x + height / 4;
	tl.y = tr.y = top.y;

	Vec2 bl, br;
	bl.x = bot.x - height / 4;
	br.x = bot.x + height / 4;
	bl.y = br.y = bot.y;

	DrawLine(tl, tr, thickness, color);
	DrawLine(bl, br, thickness, color);
	DrawLine(tl, bl, thickness, color);
	DrawLine(tr, br, thickness, color);
}

void DrawEspBox3D(Vec3 top, Vec3 bot, float a, int width, int thickness, D3DCOLOR color)
{
	int height3D = ABS(top.z - bot.z);
	Vec3 b1, b2, b3, b4, t1, t2, t3, t4;
	b1.z = b2.z = b3.z = b4.z = bot.z;

	b1.x = bot.x + (cos(TORAD(a + 45)) * width);
	b1.y = bot.y + (sin(TORAD(a + 45)) * width);

	b2.x = bot.x + (cos(TORAD(a + 135)) * width);
	b2.y = bot.y + (sin(TORAD(a + 135)) * width);

	b3.x = bot.x + (cos(TORAD(a + 225)) * width);
	b3.y = bot.y + (sin(TORAD(a + 225)) * width);

	b4.x = bot.x + (cos(TORAD(a + 315)) * width);
	b4.y = bot.y + (sin(TORAD(a + 315)) * width);

	t1.x = b1.x;
	t1.y = b1.y;
	t1.z = b1.z + height3D;

	t2.x = b2.x;
	t2.y = b2.y;
	t2.z = b2.z + height3D;

	t3.x = b3.x;
	t3.y = b3.y;
	t3.z = b3.z + height3D;

	t4.x = b4.x;
	t4.y = b4.y;
	t4.z = b4.z + height3D;

	Vec2 b1_2, b2_2, b3_2, b4_2, t1_2, t2_2, t3_2, t4_2;
	if (W2S(b1, b1_2) && W2S(b2, b2_2) && W2S(b3, b3_2) && W2S(b4, b4_2) && W2S(t1, t1_2) && W2S(t2, t2_2) && W2S(t3, t3_2) && W2S(t4, t4_2))
	{
		// columns
		DrawLine(t1_2, b1_2, thickness, color);
		DrawLine(t2_2, b2_2, thickness, color);
		DrawLine(t3_2, b3_2, thickness, color);
		DrawLine(t4_2, b4_2, thickness, color);

		// top
		DrawLine(t1_2, t2_2, thickness, color);
		DrawLine(t2_2, t3_2, thickness, color);
		DrawLine(t3_2, t4_2, thickness, color);
		DrawLine(t4_2, t1_2, thickness, color);

		// bottom
		DrawLine(b1_2, b2_2, thickness, color);
		DrawLine(b2_2, b3_2, thickness, color);
		DrawLine(b3_2, b4_2, thickness, color);
		DrawLine(b4_2, b1_2, thickness, color);
	}
}

void DrawText(const char* text, float x, float y, D3DCOLOR color)
{
	RECT rect;

	if (!hack->FontF)
		D3DXCreateFont(pDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &hack->FontF);

	// Adding a shadow effect
	// Creating zero-size rectangle centered at (x + 1, y + 1) with the following flags
	// DT_CENTER: aligns the text centered on that point
	// DT_NOCLIP: tells DirectX to ignore the rectangle's size, so the text won't get cut off
	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	hack->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0)); // D3DCOLOR_ARGB(255, 0, 0, 0) = solid black

	// Adding the actual text
	SetRect(&rect, x, y, x, y);
	hack->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}
