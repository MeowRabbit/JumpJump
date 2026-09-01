#include "Trap.h"

HBITMAP *OBJ_Trap_Bitmap[ETrapType::eTrapTypeCount];	// Trap 이미지들

HBITMAP	Trap_Snowball_Bitmap[6];	// 눈덩이 함정 이미지
HBITMAP Rain_Ice_Bitmap[3];			// 고드름 함정 이미지
HBITMAP Spear_Bitmap;				// 창 함정 이미지

// Trap 초기 셋팅
void Init_Trap(HINSTANCE hInstance, HDC hdc, OBJ_Trap Trap[], int nTrapCount)
{
	// 이미지 세부 분리 작업 및 생성
	{
		HBITMAP MainBitmap;		// 이미지 분리를 하기 위한 메인 함정 이미지

		HDC		CompatibleDC;		// 이미지 분리를 위한 중간 메모리 DC
		HBITMAP OldCompatibleBitmap;// 이미지 분리를 위한 중간 메모리 비트맵

		HDC     TempDC;     // 분리된 이미지 생성을 보조하는 메모리 DC
		HBITMAP OldBitmap;  // 분리된 이미지 생성을 보조하는 메모리 비트맵

		// 함정 이미지 불러오기
		MainBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP21));
		
		CompatibleDC = CreateCompatibleDC(hdc);
		OldCompatibleBitmap = (HBITMAP)SelectObject(CompatibleDC, MainBitmap);

		TempDC = CreateCompatibleDC(hdc);

		// 눈덩이 함정 이미지 분리
		for (int i = 0; i < 3; i++)
		{
			Rain_Ice_Bitmap[i] = CreateCompatibleBitmap(hdc, 50, 50);
			OldBitmap = (HBITMAP)SelectObject(TempDC, Rain_Ice_Bitmap[i]);
			BitBlt(TempDC, 0, 0, 50, 50, CompatibleDC, 50 * i, 50, SRCCOPY);
			SelectObject(TempDC, OldBitmap);
		}
		
		// 고드름 함정 이미지 분리
		for (int i = 0; i < 3; i++)
		{
			Trap_Snowball_Bitmap[i] = CreateCompatibleBitmap(hdc, 50, 50);
			OldBitmap = (HBITMAP)SelectObject(TempDC, Trap_Snowball_Bitmap[i]);
			BitBlt(TempDC, 0, 0, 50, 50, CompatibleDC, 50 * i, 0, SRCCOPY);
			SelectObject(TempDC, OldBitmap);
		}
		
		// 창 함정 이미지 분리
		Spear_Bitmap = CreateCompatibleBitmap(hdc, 50, 50);
		OldBitmap = (HBITMAP)SelectObject(TempDC, Spear_Bitmap);
		BitBlt(TempDC, 0, 0, 50, 50, CompatibleDC, 0, 100, SRCCOPY);
		SelectObject(TempDC, OldBitmap);


		SelectObject(CompatibleDC, OldCompatibleBitmap);

		DeleteDC(TempDC);
		DeleteObject(MainBitmap);
		DeleteDC(CompatibleDC);
		
	}
	
	// 분리된 이미지들을 통합
	OBJ_Trap_Bitmap[ETrapType::eTrap_Snowball] = Trap_Snowball_Bitmap;
	OBJ_Trap_Bitmap[ETrapType::eTrap_Rain_Ice] = Rain_Ice_Bitmap;
	OBJ_Trap_Bitmap[ETrapType::eTrap_Spear] = &Spear_Bitmap;


	// 함정 오브젝트 초기화
	for (int i = 0; i < nTrapCount; i++)
	{
		Trap[i].bLive = FALSE;
		Trap[i].eTrapType = ETrapType::eTrap_None;
		Trap[i].poPosition = { 0, 0 };
	}
}

// Trap 생성
void Create_Trap(POINT poPosition, OBJ_Trap& Trap, int eTrapType, BOOL bRight)
{
	Trap.bLive = TRUE;
	Trap.bRight = bRight;
	Trap.eTrapType = eTrapType;
	Trap.poPosition = poPosition;
	Trap.nFrame = 0;
	
}

// Trap 삭제
void Delete_Trap(OBJ_Trap& Trap)
{
	Trap.bLive = FALSE;
	Trap.eTrapType = ETrapType::eTrap_None;
	Trap.poPosition = { 0, 0 };
}

// Trap 실제 이미지 프레임 위치 가져오기
int Get_TrapImageFrame(OBJ_Trap& Trap)
{
	switch (Trap.eTrapType)
	{
	case eTrap_Snowball:
	{
		switch (Trap.nFrame)
		{
		case 11:
			Trap.nFrame = 0;
		case 0:
		case 1:
			return 0;
			break;
		case 2:
		case 3:
		case 4:
			return 1;
			break;
		case 5:
		case 6:
		case 7:
			return 2;
			break;
		case 13:
		case 14:
		case 15:
			return 3;
			break;
		case 16:
		case 17:
		case 18:
			return 4;
		case 19:
		case 20:
			return 5;
		case 21:
			Delete_Trap(Trap);
			return 5;
		default:
			break;
		}
	}
		break;
	case eTrap_Rain_Ice:
	{
		switch (Trap.nFrame)
		{
		case 8:
			Trap.nFrame = 0;
		case 0:
		case 1:
			return 0;
			break;
		case 2:
		case 3:
		case 4:
			return 1;
			break;
		case 5:
		case 6:
		case 7:
			return 2;
			break;
		default:
			break;
		}
	}
		break;
	case eTrap_Spear:
		return 0;
		break;
	default:
		break;
	}
	return 0;
}

// Trap 제거
void Destory_Trap()
{
	// 눈덩이 함정 이미지 제거
	for (int i = 0; i < 6; i++)
	{
		if (Trap_Snowball_Bitmap[i] != NULL)
		{
			DeleteObject(Trap_Snowball_Bitmap[i]);
			Trap_Snowball_Bitmap[i] = NULL;
		}
	}

	// 고드름 함정 이미지 제거
	for (int i = 0; i < 3; i++)
	{
		if (Rain_Ice_Bitmap[i] != NULL)
		{
			DeleteObject(Rain_Ice_Bitmap[i]);
			Rain_Ice_Bitmap[i] = NULL;
		}
	}

	// 창 함정 이미지 제거
	if (Spear_Bitmap != NULL)
	{
		DeleteObject(Spear_Bitmap);
		Spear_Bitmap = NULL;
	}
}
