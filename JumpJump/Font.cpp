#include "Font.h"

HBITMAP OBJ_Font_Bitmap[11];		// 기본 사이즈 폰트
HBITMAP OBJ_Mini_Font_Bitmap[10];	// 작은 사이즈 폰트
HBITMAP OBJ_Pause_Font_Bitmap[3];	// 일시정지 텍스트

// Font 초기 셋팅
void Init_Font(HINSTANCE hInstance, HDC hdc)
{
	// 이미지 세부 분리 작업 및 생성
	{
		HBITMAP MainBitmap;     // 폰트 모음 이미지

		HDC     CompatibleDC;       // 이미지 분리를 위한 중간 메모리 DC
		HBITMAP OldCompatibleBitmap;// 이미지 분리를 위한 중간 메모리 비트맵

		HDC     TempDC;     // 분리된 이미지 생성을 보조하는 메모리 DC
		HBITMAP OldBitmap;  // 분리된 이미지 생성을 보조하는 메모리 비트맵

		MainBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP23));

		CompatibleDC = CreateCompatibleDC(hdc);
		OldCompatibleBitmap = (HBITMAP)SelectObject(CompatibleDC, MainBitmap);

		TempDC = CreateCompatibleDC(hdc);

		/*
		* 기본 사이즈 폰트 이미지 분리 및 생성
		*/

		// 숫자
		for (int i = 0; i < 10; i++)
		{
			OBJ_Font_Bitmap[i] = CreateCompatibleBitmap(hdc, 20, 21);
			OldBitmap = (HBITMAP)SelectObject(TempDC, OBJ_Font_Bitmap[i]);
			BitBlt(TempDC, 0, 0, 20, 21, CompatibleDC, 20 * i, 0, SRCCOPY);
			SelectObject(TempDC, OldBitmap);
		}

		// "점"
		OBJ_Font_Bitmap[10] = CreateCompatibleBitmap(hdc, 25, 25);
		OldBitmap = (HBITMAP)SelectObject(TempDC, OBJ_Font_Bitmap[10]);
		BitBlt(TempDC, 0, 0, 25, 25, CompatibleDC, 200, 0, SRCCOPY);
		SelectObject(TempDC, OldBitmap);

		/*
		* 작은 사이즈 폰트 이미지 분리 및 생성
		*/

		for (int i = 0; i < 10; i++)
		{
			OBJ_Mini_Font_Bitmap[i] = CreateCompatibleBitmap(hdc, 15, 16);
			OldBitmap = (HBITMAP)SelectObject(TempDC, OBJ_Mini_Font_Bitmap[i]);
			BitBlt(TempDC, 0, 0, 15, 16, CompatibleDC, 15 * i, 25, SRCCOPY);
			SelectObject(TempDC, OldBitmap);
		}

		/*
		* 일시정지 폰트 이미지 분리 및 생성
		*/

		for (int i = 0; i < 3; i++)
		{
			OBJ_Pause_Font_Bitmap[i] = CreateCompatibleBitmap(hdc, 226, 50);
			OldBitmap = (HBITMAP)SelectObject(TempDC, OBJ_Pause_Font_Bitmap[i]);
			BitBlt(TempDC, 0, 0, 226, 50, CompatibleDC, 0, 50 + 50 * i, SRCCOPY);
			SelectObject(TempDC, OldBitmap);

		}

		SelectObject(CompatibleDC, OldCompatibleBitmap);

		DeleteDC(TempDC);
		DeleteObject(MainBitmap);
		DeleteDC(CompatibleDC);
	}
	
}

// 점수 그리기
void DrowScore(HDC& MainDC, HDC& ScreanDC, unsigned int nScore, POINT poPoint)
{
	if (nScore > 0)
	{
		// 점수가 0점 이상일 경우

		int Score[10];		// 점수는 9자리로 제한, 마지막 자리는 "점"
		int nTemp1;			// 계산시 사용할 나눈 값
		int nTemp2;			// 계산시 사용할 나머지
		int nNumCount = -1;	// 점수 자릿수

		// 숫자들 뽑아내기
		for (int i = 0; i < 10; i++)
		{
			nTemp1 = nScore / 10;
			nTemp2 = nScore % 10;

			if (nTemp1 == 0 && nTemp2 == 0)
			{
				Score[i] = 10;
			}
			else
			{
				Score[i] = nTemp2;
				nScore /= 10;
				nNumCount++;
			}
		}

		// 총 글자의 길이의 절반 ( 가운대 정렬을 위한 값 보조 )
		int nCorrectionX = (nNumCount * 20 + 25) / 2;

		// 숫자 그리기
		for (int i = nNumCount; i >= 0; i--)
		{
			DrowOnScrean(MainDC, ScreanDC, OBJ_Font_Bitmap[Score[i]], { poPoint.x - nCorrectionX + 20 * (nNumCount - i), poPoint.y }, FALSE);
		}
		// "점" 그리기
		DrowOnScrean(MainDC, ScreanDC, OBJ_Font_Bitmap[Score[nNumCount + 1]], { poPoint.x - nCorrectionX + 20 * (nNumCount + 1) + 2, poPoint.y - 1 }, FALSE);
	}
	else
	{
		// 0점일 경우

		// 총 글자의 길이의 절반 ( 가운대 정렬을 위한 값 보조 )
		int nCorrectionX = 35 / 2;

		// "0점" 그리기
		DrowOnScrean(MainDC, ScreanDC, OBJ_Font_Bitmap[0], { poPoint.x - nCorrectionX, poPoint.y }, FALSE);
		DrowOnScrean(MainDC, ScreanDC, OBJ_Font_Bitmap[10], { poPoint.x - nCorrectionX + 20 + 2, poPoint.y - 1 }, FALSE);
	}
}

// 돈 그리기
void DrowMoney(HDC& MainDC, HDC& ScreanDC, unsigned int nMoney, POINT poPoint)
{
	if (nMoney > 0)
	{
		// 돈이 0원 이상일 경우

		int Money[10];		// 자리수는 10자리로 제한
		int nTemp1;			// 계산시 사용할 나눈 값
		int nTemp2;			// 계산시 사용할 나머지
		int nNumCount = -1;	// 돈 자릿수

		// 숫자들 뽑아내기
		for (int i = 0; i < 10; i++)
		{
			nTemp1 = nMoney / 10;
			nTemp2 = nMoney % 10;

			if (nTemp1 == 0 && nTemp2 == 0)
			{
				Money[i] = 10;
			}
			else
			{
				Money[i] = nTemp2;
				nMoney /= 10;
				nNumCount++;
			}
		}

		// 숫자 그리기
		for (int i = nNumCount; i >= 0; i--)
		{
			DrowOnScrean(MainDC, ScreanDC, OBJ_Mini_Font_Bitmap[Money[i]], { poPoint.x + 13 * (nNumCount - i), poPoint.y }, FALSE);
		}
	}
	else
	{
		// "0" 그리기
		DrowOnScrean(MainDC, ScreanDC, OBJ_Mini_Font_Bitmap[0], { poPoint.x, poPoint.y }, FALSE);
	}
}

// Font 제거
void Destroy_Font()
{
	// 만들어놓은 이미지 핸들 삭제
	
	// Font 비트맵 제거
	for (int i = 0; i < 11; i++)
	{
		if (OBJ_Font_Bitmap[i] != NULL)
		{
			DeleteObject(OBJ_Font_Bitmap[i]);
			OBJ_Font_Bitmap[i] = NULL;
		}
	}

	// Mini Font 비트맵 제거
	for (int i = 0; i < 10; i++)
	{
		if (OBJ_Mini_Font_Bitmap[i] != NULL)
		{
			DeleteObject(OBJ_Mini_Font_Bitmap[i]);
			OBJ_Mini_Font_Bitmap[i] = NULL;
		}
	}

	// Pause Font 비트맵 제거
	for (int i = 0; i < 3; i++)
	{
		if (OBJ_Pause_Font_Bitmap[i] != NULL)
		{
			DeleteObject(OBJ_Pause_Font_Bitmap[i]);
			OBJ_Pause_Font_Bitmap[i] = NULL;
		}
	}
}
