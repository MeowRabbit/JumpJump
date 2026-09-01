#include "Upgrade.h"

// Upgrade 초기 셋팅
void Init_Upgrade(HINSTANCE hInstance, HDC hdc, OBJ_Upgrade Upgrade[])
{
	// Upgrade 제목
	TCHAR strTitle[UPGRADE_COUNT][255] = {
		TEXT("최대 체력 강화"),
		TEXT("최대 점프 횟수 강화"),
		TEXT("기본 점프 횟수 강화"),
		TEXT("최대 속도 강화"),
		TEXT("과일 점수 강화"),
		TEXT("높이 점수 강화"),
		TEXT("메소 획득량 증가"),
		TEXT("보호막")
	};

	// Upgrade 내용
	TCHAR strComment[UPGRADE_COUNT][255] = {
		TEXT("최대 체력 + %%d"),
		TEXT("최대 점프 + %%d"),
		TEXT("기본 점프 + %%d"),
		TEXT("최대 속도 + %%d"),
		TEXT("과일 추가 점수 X %%d"),
		TEXT("높이 추가 점수 + %%d"),
		TEXT("메소 추가 획득량 + %%d"),
		TEXT("보호막을 가지고 시작")
	};

	// 이미지 세부 분리 작업 및 생성
	{
		HBITMAP MainBitmap;     // 기본 캐릭터 이미지
		HBITMAP ShiledBitmap;   // 쉴드가 있는 캐릭터 이미지

		HDC     CompatibleDC;       // 이미지 분리를 위한 중간 메모리 DC
		HBITMAP OldCompatibleBitmap;// 이미지 분리를 위한 중간 메모리 비트맵

		HDC     TempDC;     // 분리된 이미지 생성을 보조하는 메모리 DC
		HBITMAP OldBitmap;  // 분리된 이미지 생성을 보조하는 메모리 비트맵

		MainBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP25));
		
		CompatibleDC = CreateCompatibleDC(hdc);
		OldCompatibleBitmap = (HBITMAP)SelectObject(CompatibleDC, MainBitmap);

		TempDC = CreateCompatibleDC(hdc);

		for (int i = 0; i < UPGRADE_COUNT; i++)
		{
			Upgrade[i].bmIcon = CreateCompatibleBitmap(hdc, 60, 60);
			OldBitmap = (HBITMAP)SelectObject(TempDC, Upgrade[i].bmIcon);

			BitBlt(TempDC, 0, 0, 60, 60, CompatibleDC, i * 60, 0, SRCCOPY);
			SelectObject(TempDC, OldBitmap);
		}

		SelectObject(CompatibleDC, OldCompatibleBitmap);

		DeleteDC(TempDC);
		DeleteObject(MainBitmap);
		DeleteDC(CompatibleDC);
	}

	// 변수 초기화
	for (int i = 0; i < UPGRADE_COUNT; i++)
	{
		Upgrade[i].nUpgradeType = i;

		wsprintf(Upgrade[i].strTitle, strTitle[i]);
		wsprintf(Upgrade[i].strComment, strComment[i]);
		Upgrade[i].unLevel = 0;

		switch (Upgrade[i].nUpgradeType)
		{
		case eMaxHP_Upgrade:
			Upgrade[i].unMoney = 100;
			break;
		case eMaxJump_Upgrade:
			Upgrade[i].unMoney = 100;
			break;
		case eJump_Upgrade:
			Upgrade[i].unMoney = 150;
			break;
		case eMaxSpeed_Upgrade:
			Upgrade[i].unMoney = 100;
			break;
		case eItemScore_Upgrade:
			Upgrade[i].unMoney = 100;
			break;
		case eHeightScore_Upgrade:
			Upgrade[i].unMoney = 1000;
			break;
		case eMoney_Upgrade:
			Upgrade[i].unMoney = 1000;
			break;
		case eShiled_Upgrade:
			Upgrade[i].unMoney = 50;
			break;
		default:
			break;
		}
	}
}

// Upgrade 레벨 업
BOOL Upgrade_LevelUp(unsigned int& unMyMoney, OBJ_Upgrade& Upgrade)
{
	if (unMyMoney >= Upgrade.unMoney * (Upgrade.unLevel + 1))
	{
		unMyMoney -= Upgrade.unMoney * (Upgrade.unLevel + 1);
		Upgrade.unLevel++;

		return TRUE;
	}
	
	return FALSE;
}

// Upgrade 제거
void Destroy_Upgrade(OBJ_Upgrade Upgrade[])
{
	// 만들어놓은 이미지 핸들 삭제

	for (int i = 0; i < UPGRADE_COUNT; i++)
	{
		if (Upgrade[i].bmIcon != NULL)
		{
			DeleteObject(Upgrade[i].bmIcon);
			Upgrade[i].bmIcon = NULL;
		}
	}
}
