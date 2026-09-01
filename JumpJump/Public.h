#pragma once
#ifndef _PUBLIC_H__
#define _PUBLIC_H__

#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "resource.h"

// 열거형 데이터
enum EGamePage
{
	eStartPage = 0,
	eReadyPage,
	eGamePage,
	eGameOverPage,
};

// 공용 정의
#define CLIENT_H		700	// 클라이언트 초기 높이
#define CLIENT_W		500	// 클라이언트 초기 넓이
#define MAX_GROUND		10	// 최대 땅 개수
#define MAX_ITEM		10	// 최대 아이템 개수
#define MAX_TRAP		10	// 최대 함정 개수
#define UPGRADE_COUNT	8	// 업그레이드 갯수
#define GRAVITY			0.63// 중력 가속도
#define MAX_GRAVITY		10	// 최대 중력 가속도

#define START_Y 500		// 시작 Y 좌표

#define GROUND_DOWN_MAX_SPEED 4	// 등반 최대 속도

// 공용 변수
extern int			nClient_H;		// 클라이언트 높이
extern int			nClient_W;		// 클라이언트 넓이
extern int			nCenterX;		// 클라이언트 가로 가운대
extern ULONGLONG	ClientTime;		// 클라이언트 시작 기준 시간
extern int			ClientStatus;	// 클라이언트 상태

extern POINT		poMousePoint;	// 마우스 좌표



// 공용 함수
void DrowOnScrean(HDC& MainDC, HDC& ScreanDC, HBITMAP& bmImage, POINT poPosition, BOOL bRight);
void DrowOnScreanAlpha(HDC& MainDC, HDC& ScreanDC, HBITMAP& bmImage, POINT poPosition, int nAlpha);
BOOL HitBoxCheck(RECT rectHitBox, POINT poPoint);
BOOL HitBoxCheck(RECT rectHitBox1, RECT rectHitBox2);


#endif // !_PUBLIC_H__

