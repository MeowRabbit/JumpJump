/*
* Player 관련
*/

#pragma once
#ifndef _PLAYER_H__
#define _PLAYER_H__
#include "Public.h"

// Player 상태
enum EPlayerStatus
{
	Idle = 0,	// 기본 상태
	Move,		// 움직이는 상태
	Jump,		// 점프 상태
	Hit			// 맞은 상태
};

// Player 오브젝트
typedef struct _OBJ_Player
{
	// 이미지들
	HBITMAP bmNow;			// 플레이어의 현재 이미지
	HBITMAP	bmIdle[2][2][3];// 플레이어 Idle
	HBITMAP bmMove[2][4];	// 플래이어 Move
	HBITMAP bmJump[2];		// 플레이어 Jump
	HBITMAP bmHit[2];		// 플레이어 Hit
	HBITMAP bmShadow;		// 플레이어 그림자

	// 이미지 보조
	ULONGLONG	dwTime;		// 플래에어 내부 시간
	int			nStatus;	// 플레이어의 상태
	int			nFrame;		// 플레이어의 내부 프레임
	BOOL		bRight;		// 플레이어가 오른쪽을 보고 있는지
	BOOL		bJump;		// 플레이어가 점프 중 인지

	// 위치 관련
	POINT	poPosition;		// 플레이어의 위치
	double	dPositionX;		// 플레이어 좌우 보조
	double	dPositionY;		// 플레이어 높이 보조
	POINT	poShadow;		// 플레이어 그림자 위치

	// 스테이터스
	int		nMAXHp;				// 플레이어 최대 체력
	int		nCurrentHP;			// 플레이어 현재 체력
	double	dSpeed;				// 플레이어 이동속도
	double	dMaxSpeed;			// 플레이어 최대 이동속도
	double	dJump;				// 플레이어 점프력
	int		nExtraJumpMAXCount;	// 플레이어 최대 추가 점프 횟수
	int		nExtraJumpCount;	// 플레이어 추가 점프 횟수
	double	dNowSpeed;			// 플레이어 현재 속도
	double	dNowGravity;		// 플레이어 중력 속도
	BOOL	bShield;			// 플레이어 쉴드 유무
	BOOL	bJumpAble;			// 플레이어가 점프가 가능한지


} OBJ_Player;

void Init_Player(HINSTANCE hInstance, HDC hdc, OBJ_Player& Player);	// Player 초기 셋팅
void PlayAnimation_Player(OBJ_Player& Player, ULONGLONG Time);		// Player 애니메이션 제어
void Move_Player(HWND hwnd, OBJ_Player& Player);					// Player 움직임 제어
void Destory_Player(OBJ_Player& Player);							// Player 제거
#endif // !_PLAYER_H__
