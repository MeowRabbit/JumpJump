#include "Player.h"

// Player 초기 셋팅
void Init_Player(HINSTANCE hInstance, HDC hdc, OBJ_Player& Player)
{
    // 이미지 세부 분리 작업 및 생성
    {
        HBITMAP MainBitmap;     // 기본 캐릭터 이미지
        HBITMAP ShiledBitmap;   // 쉴드가 있는 캐릭터 이미지

        HDC     CompatibleDC;       // 이미지 분리를 위한 중간 메모리 DC
        HBITMAP OldCompatibleBitmap;// 이미지 분리를 위한 중간 메모리 비트맵

        HDC     TempDC;     // 분리된 이미지 생성을 보조하는 메모리 DC
        HBITMAP OldBitmap;  // 분리된 이미지 생성을 보조하는 메모리 비트맵

        MainBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
        ShiledBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP7));

        CompatibleDC = CreateCompatibleDC(hdc);
        OldCompatibleBitmap = (HBITMAP)SelectObject(CompatibleDC, MainBitmap);

        TempDC = CreateCompatibleDC(hdc);

        /*
        * 기본 캐릭터 이미지 분리 및 생성
        */

        // Idle
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                Player.bmIdle[0][i][j] = CreateCompatibleBitmap(hdc, 45, 45);
                OldBitmap = (HBITMAP)SelectObject(TempDC, Player.bmIdle[0][i][j]);
                BitBlt(TempDC, 0, 0, 45, 45, CompatibleDC, 0 + (j * 50), 0 + (i * 50), SRCCOPY);
                SelectObject(TempDC, OldBitmap);
            }
        }

        // Move
        for (int i = 0; i < 4; i++)
        {
            Player.bmMove[0][i] = CreateCompatibleBitmap(hdc, 45, 45);
            OldBitmap = (HBITMAP)SelectObject(TempDC, Player.bmMove[0][i]);
            BitBlt(TempDC, 0, 0, 45, 45, CompatibleDC, 0 + (i * 50), 100, SRCCOPY);
            SelectObject(TempDC, OldBitmap);
        }

        // Jump
        Player.bmJump[0] = CreateCompatibleBitmap(hdc, 45, 45);
        OldBitmap = (HBITMAP)SelectObject(TempDC, Player.bmJump[0]);
        BitBlt(TempDC, 0, 0, 45, 45, CompatibleDC, 0, 150, SRCCOPY);
        SelectObject(TempDC, OldBitmap);

        // Hit
        for (int i = 0; i < 2; i++)
        {
            Player.bmHit[i] = CreateCompatibleBitmap(hdc, 45, 45);
            OldBitmap = (HBITMAP)SelectObject(TempDC, Player.bmHit[i]);
            BitBlt(TempDC, 0, 0, 45, 45, CompatibleDC, 0 + (i * 50), 200, SRCCOPY);
            SelectObject(TempDC, OldBitmap);
        }

        SelectObject(CompatibleDC, OldCompatibleBitmap);

        /*
        * 쉴드 캐릭터 이미지 분리 및 생성
        */

        OldCompatibleBitmap = (HBITMAP)SelectObject(CompatibleDC, ShiledBitmap);

        // Idle
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                Player.bmIdle[1][i][j] = CreateCompatibleBitmap(hdc, 45, 45);
                OldBitmap = (HBITMAP)SelectObject(TempDC, Player.bmIdle[1][i][j]);
                BitBlt(TempDC, 0, 0, 45, 45, CompatibleDC, 0 + (j * 50), 0 + (i * 50), SRCCOPY);
                SelectObject(TempDC, OldBitmap);
            }
        }

        // Move
        for (int i = 0; i < 4; i++)
        {
            Player.bmMove[1][i] = CreateCompatibleBitmap(hdc, 45, 45);
            OldBitmap = (HBITMAP)SelectObject(TempDC, Player.bmMove[1][i]);
            BitBlt(TempDC, 0, 0, 45, 45, CompatibleDC, 0 + (i * 50), 100, SRCCOPY);
            SelectObject(TempDC, OldBitmap);
        }

        // Jump
        Player.bmJump[1] = CreateCompatibleBitmap(hdc, 45, 45);
        OldBitmap = (HBITMAP)SelectObject(TempDC, Player.bmJump[1]);
        BitBlt(TempDC, 0, 0, 45, 45, CompatibleDC, 0, 150, SRCCOPY);
        SelectObject(TempDC, OldBitmap);

        SelectObject(CompatibleDC, OldCompatibleBitmap);
        
        /*
        * 그림자 이미지 생성
        */

        // Shadow
        Player.bmShadow = CreateCompatibleBitmap(hdc, 30, 5);
        OldBitmap = (HBITMAP)SelectObject(TempDC, Player.bmShadow);
        SelectObject(TempDC, OldBitmap);


        DeleteDC(TempDC);
        DeleteObject(MainBitmap);
        DeleteDC(CompatibleDC);
    }

    /*
    * 기본 변수 초기화
    */ 

    Player.bmNow = Player.bmIdle[0][0][0];

	Player.dwTime = GetTickCount64();
	Player.nStatus = EPlayerStatus::Idle;
	Player.nFrame = 0;
    Player.bRight = FALSE;
    Player.bJump = FALSE;

	Player.poPosition = { 0,0 };
    Player.dPositionX = 0.0;
    Player.dPositionY = 0.0;
    Player.poShadow = { 0, 0 };

    Player.nMAXHp = 3;
    Player.nCurrentHP = 3;
    Player.dSpeed = 1.0;
    Player.dMaxSpeed = 10.0;
    Player.dJump = 12.0;
    Player.nExtraJumpMAXCount = 0;
    Player.nExtraJumpCount = 0;
    Player.dNowSpeed = 0.0;
    Player.dNowGravity = 0.0;
    Player.bShield = FALSE;
    Player.bJumpAble = TRUE;

}

// Player 애니메이션 제어					
void PlayAnimation_Player(OBJ_Player& Player, ULONGLONG Time)
{
    while (Time > Player.dwTime + 1000/16)
    {
        Player.nFrame++;
        Player.dwTime += 1000/16;
    }
    
	switch (Player.nStatus)
	{
	case Idle:
        switch (Player.nFrame)
        {
        case 19:
            Player.nFrame = 0;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 7:
        case 8:
        case 10:
        case 13:
        case 14:
            Player.bmNow = Player.bmIdle[Player.bShield][0][0];
            break;
        case 5:
        case 9:
        case 11:
            Player.bmNow = Player.bmIdle[Player.bShield][0][1];
            break;
        case 6:
        case 12:
            Player.bmNow = Player.bmIdle[Player.bShield][0][2];
            break;
        case 15:
            Player.bmNow = Player.bmIdle[Player.bShield][1][0];
            break;
        case 16:
        case 18:
            Player.bmNow = Player.bmIdle[Player.bShield][1][1];
            break;
        case 17:
            Player.bmNow = Player.bmIdle[Player.bShield][1][2];
            break;
        default:
            Player.nFrame = 0;
            break;
        }
		break;
	case Move:
        switch (Player.nFrame)
        {
        case 0:
            Player.bmNow = Player.bmMove[Player.bShield][0];
            break;
        case 1:
            Player.bmNow = Player.bmMove[Player.bShield][1];
            break;
        case 2:
            Player.bmNow = Player.bmMove[Player.bShield][2];
            break;
        case 3:
            Player.nFrame = 0;
            Player.bmNow = Player.bmMove[Player.bShield][3];
            break;
        default:
            Player.nFrame = 0;
            break;
        }
		break;
	case Jump:
        Player.bmNow = Player.bmJump[Player.bShield];
		break;
	case Hit:
        switch (Player.nFrame)
        {
        case 0:
        case 1:
            Player.bmNow = Player.bmHit[0];
            break;
        case 2:
        case 3:
        case 4:
            Player.bmNow = Player.bmHit[1];
            break;
        default:
            Player.nFrame = 0;
            break;
        }
		break;
	default:
		break;
	}
}

// Player 움직임 제어
void Move_Player(HWND hwnd, OBJ_Player& Player)
{
    // 움직임 제어
    {
        // 좌우 버튼을 누름
        if ((GetKeyState(VK_LEFT) & 0x80 || GetKeyState(VK_RIGHT) & 0x80) && !(GetKeyState(VK_LEFT) & 0x80 && GetKeyState(VK_RIGHT) & 0x80))
        {
            // 움직임을 처음 시작하는 경우
            if (!Player.bJump && Player.nStatus != EPlayerStatus::Move)
            {
                Player.nFrame = 0;
                Player.nStatus = EPlayerStatus::Move;
            }

            if (Player.nStatus != EPlayerStatus::Move)
            {
                Player.nFrame = 0;
            }

            // 왼쪽으로 움직임
            if (GetKeyState(VK_LEFT) & 0x80)
            {
                // 왼쪽 바라보기
                Player.bRight = FALSE;
            
                // 왼쪽 방향으로 움직임 가속
                if (Player.nStatus != EPlayerStatus::Jump)
                {
                    Player.dNowSpeed -= Player.dSpeed * 1.5;
                }
                else
                {
                    Player.dNowSpeed -= Player.dSpeed;
                }

                // 최대 속도는 넘지 못한다
                if (Player.dNowSpeed < -Player.dMaxSpeed) Player.dNowSpeed = -Player.dMaxSpeed;

            }

            // 오른쪽으로 움직임
            if (GetKeyState(VK_RIGHT) & 0x80)
            {
                // 오른쪽 바라보기
                Player.bRight = TRUE;
            
                // 오른쪽 방향으로 움직임 가속
                if (Player.nStatus != EPlayerStatus::Jump)
                {
                    Player.dNowSpeed += Player.dSpeed * 1.5;
                }
                else
                {
                    Player.dNowSpeed += Player.dSpeed;
                }

                // 최대 속도는 넘지 못한다
                if (Player.dNowSpeed > Player.dMaxSpeed) Player.dNowSpeed = Player.dMaxSpeed;
            }

        }
        // 좌우 버튼을 누르지 않는 경우
        else
        {
            // 움직임 가속도를 줄임
            if (Player.nStatus != EPlayerStatus::Jump)
            {
                if (Player.dNowSpeed != 0) Player.dNowSpeed = Player.dNowSpeed * 2 / 3;
            }
            else
            {
                if (Player.dNowSpeed != 0) Player.dNowSpeed = Player.dNowSpeed * 9 / 10;
            }

            // 속도가 가속도의 절반 이하일경우 0으로 멈추게 한다.
            if (Player.dNowSpeed < Player.dSpeed / 2.0 && Player.dNowSpeed > -Player.dSpeed / 2.0) Player.dNowSpeed = 0;


            // 속도가 0이고 기본상태가 아닐경우 기본상태로 만든다.
            if (!Player.bJump && Player.nStatus != EPlayerStatus::Idle && Player.dNowSpeed == 0)
            {
                Player.nStatus = EPlayerStatus::Idle;
                Player.nFrame = 0;
            }
        }

        // 점프
        if (GetKeyState(VK_SPACE) & 0x80)
        {
            // 점프가 가능할 경우
            if (Player.bJumpAble)
            {
                // 현 상태가 점프상태인지 확인
                if (Player.nStatus == EPlayerStatus::Jump)
                {
                    // 점프 상태인 경우 추가 점프 횟수를 확인하고 추가 점프
                    if (Player.nExtraJumpCount > 0)
                    {
                        Player.nExtraJumpCount--;
                        Player.dNowGravity = -Player.dJump;
                        Player.bJumpAble = FALSE;
                    }
                }
                else
                {
                    // 점프 상태가 아닌경우
                    // 점프 상태로 변경 후 점프
                    Player.nFrame = 0;
                    Player.bJump = TRUE;
                    Player.nStatus = EPlayerStatus::Jump;
                    Player.dNowGravity = -Player.dJump;
                    Player.bJumpAble = FALSE;
                }
            }
        }
        else
        {
            // 키를 계속 입력하여 점프하는 경우 방지
            // 키를 다시 눌렀을때 점프 할 수 있게 도와준다.
            Player.bJumpAble = TRUE;
        }
    }

    // Player의 움직임 변화
    {
        // 좌우 움직임 변화
        Player.dPositionX += Player.dNowSpeed;

        if (CLIENT_W < Player.dPositionX + CLIENT_W / 2.0 + 20)
            Player.dPositionX = CLIENT_W / 2.0 - 20;
        if (-CLIENT_W > Player.dPositionX - CLIENT_W / 2.0 - 20)
            Player.dPositionX = -CLIENT_W / 2.0 + 20;

        Player.poPosition.x = (long)Player.dPositionX;
    
        // 상하 움직임 변화
        if (Player.nStatus == EPlayerStatus::Jump)
        {
            Player.dPositionY -= Player.dNowGravity;
            Player.poPosition.y = (long)Player.dPositionY;
            Player.dNowGravity += GRAVITY;
        }
    }
}

// Player 제거
void Destory_Player(OBJ_Player& Player)
{
    // 만들어놓은 이미지 핸들 삭제

    for (int i = 0; i < 2; i++)
    {
        // Idle 삭제
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (Player.bmIdle[i][j][k] != NULL)
                {
                    DeleteObject(Player.bmIdle[i][j][k]);
                    Player.bmIdle[i][j][k] = NULL;
                }
            }
        }

        // Move 삭제
        for (int j = 0; j < 4; j++)
        {
            if (Player.bmMove[i][j] != NULL)
            {
                DeleteObject(Player.bmMove[i][j]);
                Player.bmMove[i][j] = NULL;
            }
            
        }

        // Jump 삭제
        if (Player.bmJump[i] != NULL)
        {
            DeleteObject(Player.bmJump[i]);
            Player.bmJump[i] = NULL;
        }

        // Hit 삭제
        if (Player.bmHit[i] != NULL)
        {
            DeleteObject(Player.bmHit[i]);
            Player.bmHit[i] = NULL;
        }
    }
    
    // Shadow 삭제
    if (Player.bmShadow != NULL)
    {
        DeleteObject(Player.bmShadow);
        Player.bmShadow = NULL;
    }
}
