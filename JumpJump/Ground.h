#pragma once
#ifndef _GROUND_H__
#define _GROUND_H__

#include "Public.h"

enum EGroundType
{
	eGround_None = -1,
	eGround_Start,
	eGround_Small,
	eGroundTypeCount
};

typedef struct _OBJ_Ground
{
	int		eGroundType;
	POINT	poPosition;
	int		nWidth;
	BOOL	bLive;
} OBJ_Ground;

extern HBITMAP OBJ_Ground_Bitmap[2];

void Init_Ground(HINSTANCE hInstance, HDC hdc, OBJ_Ground Ground[], int nGroundCount);
void Create_Ground(POINT poPosition, OBJ_Ground &Ground, int eGroundType);
void Delete_Ground(OBJ_Ground &Ground);

#endif // !_GROUND_H__