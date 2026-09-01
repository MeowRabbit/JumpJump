#include "Item.h"

// Item 이미지들
HBITMAP OBJ_Item_Bitmap[EItemType::eItemTypeCount];

// Item 초기 셋팅
void Init_Item(HINSTANCE hInstance, HDC hdc, OBJ_Item Item[], int nItemCount)
{
	// 이미지 세부 분리 작업 및 생성
	{
		HBITMAP MainBitmap;		// 이미지 분리를 하기 위한 메인 아이템 이미지

		HDC		CompatibleDC;		// 이미지 분리를 위한 중간 메모리 DC
		HBITMAP OldCompatibleBitmap;// 이미지 분리를 위한 중간 메모리 비트맵

		HDC     TempDC;     // 분리된 이미지 생성을 보조하는 메모리 DC
		HBITMAP OldBitmap;  // 분리된 이미지 생성을 보조하는 메모리 비트맵

		// 아이템 이미지 불러오기
		MainBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP6));
		
		CompatibleDC = CreateCompatibleDC(hdc);
		OldCompatibleBitmap = (HBITMAP)SelectObject(CompatibleDC, MainBitmap);
		
		TempDC = CreateCompatibleDC(hdc);

		for (int i = 0; i < EItemType::eItemTypeCount; i++)
		{
			OBJ_Item_Bitmap[i] = CreateCompatibleBitmap(hdc, 30, 30);
			OldBitmap = (HBITMAP)SelectObject(TempDC, OBJ_Item_Bitmap[i]);

			switch (i)
			{
			case ePostion:
				BitBlt(TempDC, 0, 0, 30, 30, CompatibleDC, 0, 0, SRCCOPY);
				break;
			case eFullPpstion:
				BitBlt(TempDC, 0, 0, 30, 30, CompatibleDC, 50, 0, SRCCOPY);
				break;
			case eScore1:
				BitBlt(TempDC, 0, 0, 30, 30, CompatibleDC, 0, 50, SRCCOPY);
				break;
			case eScore2:
				BitBlt(TempDC, 0, 0, 30, 30, CompatibleDC, 50, 50, SRCCOPY);
				break;
			case eScore3:
				BitBlt(TempDC, 0, 0, 30, 30, CompatibleDC, 100, 50, SRCCOPY);
				break;
			case eScore4:
				BitBlt(TempDC, 0, 0, 30, 30, CompatibleDC, 150, 50, SRCCOPY);
				break;
			case eGameSpeedUp:
				BitBlt(TempDC, 0, 0, 30, 30, CompatibleDC, 0, 100, SRCCOPY);
				break;
			case eGameSpeedDown:
				BitBlt(TempDC, 0, 0, 30, 30, CompatibleDC, 50, 100, SRCCOPY);
				break;
			case eJumpUp:
				BitBlt(TempDC, 0, 0, 30, 30, CompatibleDC, 0, 150, SRCCOPY);
				break;
			case eShield:
				BitBlt(TempDC, 0, 0, 30, 30, CompatibleDC, 50, 150, SRCCOPY);
				break;
			case eSpeedUp:
				BitBlt(TempDC, 0, 0, 30, 30, CompatibleDC, 100, 150, SRCCOPY);
				break;
			case eMoney:
				BitBlt(TempDC, 0, 0, 30, 30, CompatibleDC, 0, 200, SRCCOPY);
				break;
			case eBigMoney:
				BitBlt(TempDC, 0, 0, 30, 30, CompatibleDC, 50, 200, SRCCOPY);
				break;
			default:
				break;
			}

			SelectObject(TempDC, OldBitmap);
		}

		SelectObject(CompatibleDC, OldCompatibleBitmap);

		DeleteDC(TempDC);
		DeleteObject(MainBitmap);
		DeleteDC(CompatibleDC);
	}


	for (int i = 0; i < nItemCount; i++)
	{
		Item[i].bLive = FALSE;
	}
}

// Item 생성
void Create_Item(POINT poPosition, OBJ_Item& Item, int nItemType)
{
	Item.poPosition = poPosition;
	Item.nItemType = nItemType;
	Item.bLive = TRUE;

	switch (nItemType)
	{
	case ePostion:
	case eFullPpstion:
		Item.nScore = 10;
		break;
	case eScore1:
	case eScore2:
	case eScore3:
	case eScore4:
		Item.nScore = 100;
		break;
	case eGameSpeedUp:
	case eGameSpeedDown:
	case eJumpUp:
	case eShield:
	case eSpeedUp:
		Item.nScore = 50;
		break;
	case eMoney:
		Item.nScore = 1;
		break;
	case eBigMoney:
		Item.nScore = 10;
		break;
	default:
		break;
	}
}

// Item 삭제
void Delete_Item(OBJ_Item& Item)
{
	Item.bLive = FALSE;
	Item.nScore = 0;
	Item.poPosition = { 0, 0 };
	Item.nItemType = EItemType::eNone;
}

// Item 제거
void Destroy_Item()
{
}
