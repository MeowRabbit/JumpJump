/*
* Font 관련
*/

#pragma once
#ifndef _FONT_H__
#define _FONT_H__

#include "Public.h"

extern HBITMAP OBJ_Font_Bitmap[11];			// 기본 사이즈 폰트
extern HBITMAP OBJ_Mini_Font_Bitmap[10];	// 작은 사이즈 폰트
extern HBITMAP OBJ_Pause_Font_Bitmap[3];	// 일시정지 텍스트

void Init_Font(HINSTANCE hInstance, HDC hdc);									// Font 초기 셋팅
void DrowScore(HDC& MainDC, HDC& ScreanDC, unsigned int nScore, POINT poPoint);	// 점수 그리기
void DrowMoney(HDC& MainDC, HDC& ScreanDC, unsigned int nMoney, POINT poPoint);	// 돈 그리기
void Destroy_Font();															// Font 제거

#endif // !_FONT_H__