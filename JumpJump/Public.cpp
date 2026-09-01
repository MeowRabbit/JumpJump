#include "Public.h"

int			nClient_H;		// 클라이언트 높이
int			nClient_W;		// 클라이언트 넓이
int			nCenterX;		// 클라이언트 가로 가운대
ULONGLONG	ClientTime;		// 클라이언트 시작 기준 시간
int			ClientStatus;	// 클라이언트 상태

POINT		poMousePoint;	// 마우스 좌표


void DrowOnScrean(HDC& MainDC, HDC& ScreanDC, HBITMAP& bmImage, POINT poPosition, BOOL bRight)
{
    HDC MemDC;
    HBITMAP OldBitmap;
    BITMAP bit;

    HDC TempDC;
    HBITMAP TempBitmap, TempOldBitmap;

    MemDC = CreateCompatibleDC(MainDC);
    OldBitmap = (HBITMAP)SelectObject(MemDC, bmImage);

    GetObject(bmImage, sizeof(BITMAP), &bit);
    
    TempDC = CreateCompatibleDC(MainDC);
    TempBitmap = CreateCompatibleBitmap(MainDC, bit.bmWidth, bit.bmHeight);
    TempOldBitmap = (HBITMAP)SelectObject(TempDC, TempBitmap);

    if (bRight)
    {
        StretchBlt(TempDC,
            bit.bmWidth, 0,
            -bit.bmWidth - 1, bit.bmHeight,
            MemDC, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);
    }
    else
    {
        StretchBlt(TempDC,
            0, 0,
            bit.bmWidth, bit.bmHeight,
            MemDC, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);
    }

    TransparentBlt(ScreanDC,
        poPosition.x - bit.bmWidth / 2, poPosition.y,
        bit.bmWidth, bit.bmHeight,
        TempDC,
        0, 0,
        bit.bmWidth, bit.bmHeight,
        RGB(255, 0, 255));

    SelectObject(MemDC, OldBitmap);
    SelectObject(TempDC, TempOldBitmap);

    DeleteDC(MemDC);
    DeleteDC(TempDC);
    DeleteObject(TempBitmap);
}

void DrowOnScreanAlpha(HDC& MainDC, HDC& ScreanDC, HBITMAP& bmImage, POINT poPosition, int nAlpha)
{
    HDC MemDC;
    HBITMAP OldBitmap;
    BITMAP bit;

    HDC TempDC;
    HBITMAP TempBitmap, TempOldBitmap;

    MemDC = CreateCompatibleDC(MainDC);
    OldBitmap = (HBITMAP)SelectObject(MemDC, bmImage);

    GetObject(bmImage, sizeof(BITMAP), &bit);

    TempDC = CreateCompatibleDC(MainDC);
    TempBitmap = CreateCompatibleBitmap(MainDC, bit.bmWidth, bit.bmHeight);
    TempOldBitmap = (HBITMAP)SelectObject(TempDC, TempBitmap);

    StretchBlt(TempDC,
        0, 0,
        bit.bmWidth, bit.bmHeight,
        MemDC, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);
    

    BLENDFUNCTION bf;

    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = 0;
    bf.SourceConstantAlpha = nAlpha;

    AlphaBlend(ScreanDC,
        poPosition.x - bit.bmWidth / 2, poPosition.y,
        bit.bmWidth, bit.bmHeight,
        TempDC,
        0, 0,
        bit.bmWidth, bit.bmHeight,
        bf);

    SelectObject(MemDC, OldBitmap);
    SelectObject(TempDC, TempOldBitmap);

    DeleteDC(MemDC);
    DeleteDC(TempDC);
    DeleteObject(TempBitmap);
}

BOOL HitBoxCheck(RECT rectHitBox, POINT poPoint)
{
    if (poPoint.x >= rectHitBox.left &&
        poPoint.x <= rectHitBox.right &&
        poPoint.y >= rectHitBox.top &&
        poPoint.y <= rectHitBox.bottom)
    {
        return TRUE;
    }
    return FALSE;
}

BOOL HitBoxCheck(RECT rectHitBox1, RECT rectHitBox2)
{
    if (rectHitBox2.right >= rectHitBox1.left &&
        rectHitBox2.left <= rectHitBox1.right &&
        rectHitBox2.bottom >= rectHitBox1.top &&
        rectHitBox2.top <= rectHitBox1.bottom)
    {
        return TRUE;
    }
    return FALSE;
}
