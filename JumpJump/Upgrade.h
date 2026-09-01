/*
* Upgrade 관련
*/

#pragma once
#ifndef __UPGRADE_H_
#define __UPGRADE_H_

#include "Public.h"

// Upgrade 종류
enum EUpgradeType {
	eMaxHP_Upgrade = 0,		// 최대 체력
	eMaxJump_Upgrade,		// 최대 점프 횟수
	eJump_Upgrade,			// 기본 점프 횟수
	eMaxSpeed_Upgrade,		// 최대 움직임 속도
	eItemScore_Upgrade,		// 아이템 점수 보너스
	eHeightScore_Upgrade,	// 높이 점수 보너스
	eMoney_Upgrade,			// 획득 돈 강화
	eShiled_Upgrade			// 시작 쉴드 아이템
};

// Upgrade 오브젝트
typedef struct _OBJ_Upgrade
{
	int				nUpgradeType;	// Upgrade 타입
	HBITMAP			bmIcon;			// Upgrade 아이콘
	TCHAR			strTitle[255];	// Upgrade 이름
	TCHAR			strComment[255];// Upgrade 설명
	unsigned int	unLevel;		// Upgrade 레벨
	unsigned int	unMoney;		// Upgrade 금액

} OBJ_Upgrade;

void Init_Upgrade(HINSTANCE hInstance, HDC hdc, OBJ_Upgrade Upgrade[]);	// Upgrade 초기 셋팅
BOOL Upgrade_LevelUp(unsigned int& unMyMoney, OBJ_Upgrade& Upgrade);	// Upgrade 레벨 업
void Destroy_Upgrade(OBJ_Upgrade Upgrade[]);							// Upgrade 제거

#endif // !__UPGRADE_H_
