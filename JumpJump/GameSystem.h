#pragma once
#ifndef _GAME_SYSTEM_H__
#define _GAME_SYSTEM_H__

#include "Player.h"
#include "Ground.h"
#include "Item.h"
#include "Upgrade.h"
#include "Trap.h"
#include "Font.h"

// 변수
extern int			nViewPointY;	// 클라이언트 보는 시점 
extern int			nShadowY;		// 그림자 위치 상태
extern BOOL			bShadow;		// 그림자 존재 여부
extern int			nHighGroundY;	// 가장 높은 땅의 높이

extern unsigned int unPlayerHighScore;	// 플레이어의 최고 점수
extern unsigned int	unTotalScore;		// 최종 게임 점수
extern unsigned int unHeightScore;		// 게임 높이 점수
extern unsigned int	unItemScore;		// 게임 아이템 점수
extern unsigned int	unTotalMoney;		// 총 게임 돈
extern unsigned int unPlayingMoney;		// 게임 진행 중 얻은 돈

extern BOOL			bManual;            // 게임설명 보조
extern BOOL			bEscDown;			// ESC 토글 보조
extern BOOL			bEnterDown;			// Enter 토글 보조
extern BOOL			bGamePause;			// 게임 멈춤 여부
extern BOOL			bHighScore;         // 최고 점수인지 여부
extern int			nGameBackgroundScrollValue;// 게임 페이지 - 배경 스크롤 부분
extern int			nHeightScoreCount;	// 게임 높이 점수 카운트 용도
extern int			nItemCreateCount;	// 아이템 생산 속도 카운트 용도
extern int			nTrapCreateCount;	// 함정 생산 속도 카운트 용도
extern int			nGroundDownSpeed;	// 등반 속도
extern int			nNowGamePage;		// 현재 게임 페이지
extern int			nBeforeGroundPointX;// 이전 땅 좌우 위치

extern int			nUpgradeScrollValue;// 준비 페이지 - 업그레이드 스크롤 부분

// 오브젝트들
extern OBJ_Player	    Player;		            // 플레이어
extern OBJ_Ground	    Ground[MAX_GROUND];	    // 나머지 땅
extern OBJ_Item			Item[MAX_ITEM];         // 아이템
extern OBJ_Upgrade		Upgrade[UPGRADE_COUNT]; // 업그레이드
extern OBJ_Trap			Trap[MAX_TRAP];			// 업그레이드

// 시스템
void Init_GameData(HINSTANCE& hInstance, HWND& hwnd);
void Reset_GamePageData();
BOOL SaveGame();
void EndGame();

// 그리기
void DrowStartPage(HDC& MainDC, HDC& ScreanDC);
void DrowReadyPage(HDC& MainDC, HDC& ScreanDC);
void DrowGamePage(HDC& MainDC, HDC& ScreanDC);
void DrowGameOverPage(HDC& MainDC, HDC& ScreanDC);

// 마우스 클릭
BOOL ClickCheckStartPage(POINT poMousePoint);
BOOL ClickCheckReadyPage(POINT poMousePoint);
BOOL ClickCheckGamePage(POINT poMousePoint);
BOOL ClickCheckGameOverPage(POINT poMousePoint);

// 마우스 휠
BOOL WheelCheckStartPage(short shWheelValue);
BOOL WheelReadyPage(short shWheelValue);
BOOL WheelGamePage(short shWheelValue);
BOOL WheelGameOverPage(short shWheelValue);

// 루프
void LoopStartPage(HWND& hwnd);
void LoopReadyPage(HWND& hwnd);
void LoopGamePage(HWND& hwnd);
void LoopGameOverPage(HWND& hwnd);

#endif // !_GAME_SYSTEM_H__
