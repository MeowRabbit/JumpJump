/*
* Trap 관련
*/
#pragma once
#ifndef _TRAP_H__
#define _TRAP_H__

#include "Public.h"

// Trap 종류
enum ETrapType
{
	eTrap_None = -1,	// 함정이 없음
	eTrap_Snowball,		// 눈덩이 함정
	eTrap_Rain_Ice,		// 고드름 함정
	eTrap_Spear,		// 창 함정 ( 미구현 )
	eTrapTypeCount		// 함정의 총 가짓수
};

// Trap 오브젝트
typedef struct _OBJ_Trap
{
	int		eTrapType;	// 함정 타입
	int		nFrame;		// 함정 내부 프레임
	POINT	poPosition;	// 함정 위치
	BOOL	bRight;		// 함정의 오른쪽 유무
	BOOL	bLive;		// 함정이 아직 존재하는지
} OBJ_Trap;

extern HBITMAP *OBJ_Trap_Bitmap[ETrapType::eTrapTypeCount];	// Trap 이미지들

void	Init_Trap(HINSTANCE hInstance, HDC hdc, OBJ_Trap Trap[], int nTrapCount);	// Trap 초기 셋팅
void	Create_Trap(POINT poPosition, OBJ_Trap& Trap, int eTrapType, BOOL bRight);	// Trap 생성
void	Delete_Trap(OBJ_Trap& Trap);												// Trap 삭제
int		Get_TrapImageFrame(OBJ_Trap& Trap);											// Trap 실제 이미지 프레임 위치 가져오기
void	Destory_Trap();																// Trap 제거

#endif // !_TRAP_H__