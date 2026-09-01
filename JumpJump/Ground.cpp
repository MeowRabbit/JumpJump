#include "Ground.h"

HBITMAP OBJ_Ground_Bitmap[2];

void Init_Ground(HINSTANCE hInstance, HDC hdc, OBJ_Ground Ground[], int nGroundCount)
{
	HDC CompatibleDC;
	HBITMAP MainBitmap, CompatibleBitmap;

	CompatibleDC = CreateCompatibleDC(hdc);
	MainBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
	CompatibleBitmap = (HBITMAP)SelectObject(CompatibleDC, MainBitmap);

	HDC     TempDC = CreateCompatibleDC(hdc);
	HBITMAP OldBitmap;

	OBJ_Ground_Bitmap[0] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
	OBJ_Ground_Bitmap[1] = CreateCompatibleBitmap(hdc, 70, 80);

	OldBitmap = (HBITMAP)SelectObject(TempDC, OBJ_Ground_Bitmap[1]);
	BitBlt(TempDC, 0, 0, 35, 40, CompatibleDC, 0, 100, SRCCOPY);
	BitBlt(TempDC, 35, 0, 35, 40, CompatibleDC, 0, 150, SRCCOPY);
	BitBlt(TempDC, 0, 40, 35, 40, CompatibleDC, 0, 200, SRCCOPY);
	BitBlt(TempDC, 35, 40, 35, 40, CompatibleDC, 50, 200, SRCCOPY);
	SelectObject(TempDC, OldBitmap);

	SelectObject(CompatibleDC, CompatibleBitmap);

	DeleteDC(TempDC);
	DeleteObject(MainBitmap);
	DeleteDC(CompatibleDC);

	for (int i = 0; i < nGroundCount; i++)
	{
		Ground[i].bLive = FALSE;
	}
}

void Create_Ground(POINT poPosition, OBJ_Ground &Ground, int eGroundType)
{
	BITMAP bit;
	GetObject(OBJ_Ground_Bitmap[eGroundType], sizeof(BITMAP), &bit);

	Ground.eGroundType = eGroundType;
 	Ground.poPosition = poPosition;
	Ground.nWidth = bit.bmWidth;
	Ground.bLive = TRUE;
}

void Delete_Ground(OBJ_Ground& Ground)
{
	Ground.bLive = FALSE;
	Ground.nWidth = 0;
	Ground.poPosition = { 0, 0 };
}
