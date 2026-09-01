/*
* Item 관련
*/

#pragma once

#ifndef _ITEM_H__
#define _ITEM_H__

#include "Public.h"

// Item 종류
enum EItemType
{
	eNone = -1,		// 아이템이 없음
	ePostion,		// 회복 포션
	eFullPpstion,	// 풀회복 포션
	eScore1,		// 점수 아이콘 1
	eScore2,		// 점수 아이콘 2
	eScore3,		// 점수 아이콘 3
	eScore4,		// 점수 아이콘 4
	eGameSpeedUp,	// 게임 속도 증가 ( 땅 떨어지는 속도 증가 )
	eGameSpeedDown,	// 게임 속도 감소	 ( 땅 떨어지는 속도 감소 )
	eJumpUp,		// 최대 점프 횟수 증가
	eShield,		// 쉴드
	eSpeedUp,		// 최대 속도 증가
	eMoney,			// 돈
	eBigMoney,		// 큰 돈
	eItemTypeCount	// 아이템 총 가지 수
};

// Item 오브젝트
typedef struct _OBJ_Item
{
	int		nItemType;	// 아이템 타입
	POINT	poPosition;	// 아이템 위치
	int		nScore;		// 아이템 점수
	BOOL	bLive;		// 아이템이 아직 존재하는지
} OBJ_Item;

extern HBITMAP OBJ_Item_Bitmap[EItemType::eItemTypeCount];	// Item 이미지들

void Init_Item(HINSTANCE hInstance, HDC hdc, OBJ_Item Item[], int nItemCount);	// Item 초기 셋팅
void Create_Item(POINT poPosition, OBJ_Item& Item, int nItemType);				// Item 생성
void Delete_Item(OBJ_Item& Item);												// Item 삭제
void Destroy_Item();															// Item 제거

#endif // !_ITEM_H__
