#include "GameSystem.h"
#include <stdio.h>

// 변수
int			nViewPointY;	// 클라이언트 보는 시점 
int			nShadowY;		// 그림자 위치 상태
BOOL		bShadow;		// 그림자 존재 여부
int			nHighGroundY;	// 가장 높은 땅의 높이

unsigned int    unPlayerHighScore;	// 플레이어의 최고 점수
unsigned int    unTotalScore;		// 최종 게임 점수
unsigned int    unHeightScore;		// 게임 높이 점수
unsigned int	unItemScore;		// 게임 아이템 점수
unsigned int	unTotalMoney;		// 총 게임 돈
unsigned int    unPlayingMoney;		// 게임 진행 중 얻은 돈

BOOL        bManual;            // 게임설명 보조
BOOL		bEscDown;			// ESC 토글 보조
BOOL        bEnterDown;         // Enter 토글 보조
BOOL		bGamePause;		    // 게임 멈춤 여부
BOOL        bHighScore;         // 최고 점수인지 여부
int			nGameBackgroundScrollValue;// 게임 페이지 - 배경 스크롤 부분
int			nHeightScoreCount;	// 게임 높이 점수 카운트 용도
int			nItemCreateCount;	// 아이템 생산 속도 카운트 용도
int         nTrapCreateCount;   // 함정 생산 속도 카운트 용도
int			nGroundDownSpeed;	// 등반 속도
int			nNowGamePage;		// 현재 게임 페이지
int			nBeforeGroundPointX;// 이전 땅 좌우 위치

int			nUpgradeScrollValue;// 준비 페이지 - 업그레이드 스크롤 부분

// 오브젝트 들
OBJ_Player	    Player;
OBJ_Ground	    Ground[MAX_GROUND];
OBJ_Item        Item[MAX_ITEM];
OBJ_Upgrade     Upgrade[UPGRADE_COUNT];
OBJ_Trap        Trap[MAX_TRAP];


// UI 관련 비트맵 이미지들
HBITMAP Bitmap_UI_StartPage_Main_Background;
HBITMAP Bitmap_UI_StartPage_Title;
HBITMAP Bitmap_UI_StartPage_StartButton;
HBITMAP Bitmap_UI_StartPage_Panel;
HBITMAP Bitmap_UI_StartPage_Manual;

HBITMAP Bitmap_UI_ReadyPage_Main_Background;
HBITMAP Bitmap_UI_ReadyPage_Background_Front;
HBITMAP Bitmap_UI_ReadyPage_Background_Back;
HBITMAP Bitmap_UI_ReadyPage_Upgrade_Block;
HBITMAP Bitmap_UI_ReadyPage_StartButton;

HBITMAP Bitmap_UI_GamePage_Main_Background;
HBITMAP Bitmap_UI_GamePage_PauseButton;
HBITMAP Bitmap_UI_GamePage_Money;
HBITMAP Bitmap_UI_GamePage_Input;
HBITMAP Bitmap_UI_GamePage_HP_Background;
HBITMAP Bitmap_UI_GamePage_HP;
HBITMAP Bitmap_UI_GamePage_Pause_Screen;
HBITMAP Bitmap_UI_GamePage_Pause_Text;

HBITMAP Bitmap_UI_GameOverPage_Main_Background;
HBITMAP Bitmap_UI_GameOverPage_HighScore;


void Init_GameData(HINSTANCE& hInstance, HWND& hwnd)
{
    HDC hdc = GetDC(hwnd);

    // 랜덤 난수 초기화
    srand(GetTickCount64());
    // 게임 시간 초기화
    ClientTime = GetTickCount64();

    // 변수 초기화
    unTotalScore = 0;
    unHeightScore = 0;
    unItemScore = 0;
    nHeightScoreCount = 0;

    nViewPointY = 0;
    nHighGroundY = 0;
    nNowGamePage = EGamePage::eStartPage;

    // 오브젝트 초기화
    Init_Player(hInstance, hdc, Player);
    Init_Ground(hInstance, hdc, Ground, MAX_GROUND);
    Init_Item(hInstance, hdc, Item, MAX_ITEM);
    Init_Upgrade(hInstance, hdc, Upgrade);
    Init_Trap(hInstance, hdc, Trap, MAX_TRAP);
    Init_Font(hInstance, hdc);

    // UI 이미지 로드
    Bitmap_UI_StartPage_Main_Background = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP14));
    Bitmap_UI_StartPage_Title = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP17));
    Bitmap_UI_StartPage_StartButton = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP15));
    Bitmap_UI_StartPage_Panel = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP16));
    Bitmap_UI_StartPage_Manual = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP28));

    Bitmap_UI_ReadyPage_Main_Background = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP13));
    Bitmap_UI_ReadyPage_Background_Front = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP9));
    Bitmap_UI_ReadyPage_Background_Back = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP10));
    Bitmap_UI_ReadyPage_Upgrade_Block = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP11));
    Bitmap_UI_ReadyPage_StartButton = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP12));

    Bitmap_UI_GamePage_Main_Background = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP18));
    Bitmap_UI_GamePage_PauseButton = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP5));
    Bitmap_UI_GamePage_Input = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
    Bitmap_UI_GamePage_Money = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP8));
    Bitmap_UI_GamePage_HP_Background = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP19));
    Bitmap_UI_GamePage_HP = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP20));
    Bitmap_UI_GamePage_Pause_Screen = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP26));
    Bitmap_UI_GamePage_Pause_Text = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP27));

    Bitmap_UI_GameOverPage_Main_Background = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP22));
    Bitmap_UI_GameOverPage_HighScore = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP24));
    DeleteDC(hdc);

    // 변수 초기화
    bManual = FALSE;

    // 세이브 파일 불러오기
    FILE* fp;

    if (0 == fopen_s(&fp, "Data.bin", "rb"))
    {
        int nData[2 + UPGRADE_COUNT];
        fread(nData, sizeof(int), 2 + UPGRADE_COUNT, fp);

        unPlayerHighScore = nData[0];
        unTotalMoney = nData[1];

        for (int i = 0; i < UPGRADE_COUNT; i++)
        {
            Upgrade[i].unLevel = nData[i + 2];
        }
        fclose(fp);
    }
    else
    {
        if (0 == fopen_s(&fp, "Data.bin", "wb"))
        {
            unPlayerHighScore = 0;
            unTotalMoney = 10000;

            fwrite(&unPlayerHighScore, sizeof(int), 1, fp);
            fwrite(&unTotalMoney, sizeof(int), 1, fp);
            for (int i = 0; i < UPGRADE_COUNT; i++)
            {
                fwrite(&Upgrade[i].unLevel, sizeof(int), 1, fp);
            }

            fclose(fp);
        }
    }
}

void Reset_GamePageData()
{
    // 시스템 관련 초기화
    nUpgradeScrollValue = 0;
    nGameBackgroundScrollValue = 0;
    unTotalScore = 0;
    unHeightScore = 0;
    unItemScore = 0;
    nHeightScoreCount = 0;
    nItemCreateCount = 0;
    nTrapCreateCount = 0;
    nViewPointY = 0;
    unPlayingMoney = 0;
    bEscDown = FALSE;
    bGamePause = FALSE;
    bHighScore = FALSE;

    // 플레이어 초기화
    Player.bmNow = Player.bmIdle[Player.bShield][0][0];

    Player.nStatus = EPlayerStatus::Idle;
    Player.nFrame = 0;
    Player.dwTime = GetTickCount64();
    Player.dPositionY = 0.0;
    Player.dPositionX = 0.0;
    Player.poPosition = { 0,0 };
    Player.bRight = FALSE;
    Player.bJump = FALSE;
    Player.bJumpAble = TRUE;

    Player.nMAXHp = 3 + Upgrade[EUpgradeType::eMaxHP_Upgrade].unLevel;
    Player.nCurrentHP = 3 + Upgrade[EUpgradeType::eMaxHP_Upgrade].unLevel;
    Player.bShield = Upgrade[EUpgradeType::eShiled_Upgrade].unLevel;
    Player.dSpeed = 0.75;
    Player.dMaxSpeed = 6.5;
    Player.nExtraJumpMAXCount = Upgrade[EUpgradeType::eJump_Upgrade].unLevel;
    Player.nExtraJumpCount = Upgrade[EUpgradeType::eJump_Upgrade].unLevel;

    Player.dNowSpeed = 0;
    Player.dNowGravity = 0.0;

    Player.poShadow = { 0,0 };

    // 땅 초기화
    nHighGroundY = 0;
    nGroundDownSpeed = 1;
    nBeforeGroundPointX = 0;

    if (Ground[0].bLive)
    {
        Delete_Ground(Ground[0]);
    }
    Create_Ground({ nBeforeGroundPointX, 0 }, Ground[0], EGroundType::eGround_Start);

    for (int i = 1; i < MAX_GROUND; i++)
    {
        nHighGroundY += 100;

        if (Ground[i].bLive)
        {
            Delete_Ground(Ground[i]);
        }

        nBeforeGroundPointX += rand() % 300 - 150;
        if (nBeforeGroundPointX + nCenterX < 0)
            nBeforeGroundPointX += nCenterX;
        if (nBeforeGroundPointX + nCenterX > nClient_W)
            nBeforeGroundPointX -= nCenterX;
        Create_Ground({ nBeforeGroundPointX, nHighGroundY }, Ground[i], EGroundType::eGround_Small);
    }

    // 아이템 관련 초기화
    for (int i = 0; i < MAX_ITEM; i++)
    {
        if (Item[i].bLive)
        {
            Delete_Item(Item[i]);
        }
    }
}

BOOL SaveGame()
{
    FILE* fp;

    if (0 == fopen_s(&fp, "Data.bin", "wb"))
    {
        fwrite(&unPlayerHighScore, sizeof(unsigned int), 1, fp);
        fwrite(&unTotalMoney, sizeof(unsigned int), 1, fp);

        for (int i = 0; i < UPGRADE_COUNT; i++)
        {
            fwrite(&Upgrade[i].unLevel, sizeof(unsigned int), 1, fp);
        }

        fclose(fp);
        return TRUE;
    }
    return FALSE;
}

void EndGame()
{
    SaveGame();
    
    // HBITMAP 삭제
    // Player 삭제

}

void DrowStartPage(HDC& MainDC, HDC& ScreanDC)
{
    TCHAR strBuff[255];
    BITMAP bit;
    GetObject(Bitmap_UI_StartPage_Main_Background, sizeof(BITMAP), &bit);
    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_StartPage_Main_Background, { nCenterX, nClient_H - bit.bmHeight }, FALSE);

    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_StartPage_Title, { nCenterX, 50 }, FALSE);
    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_StartPage_StartButton, { nCenterX, nClient_H - 160 }, FALSE);
    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_StartPage_Panel, { nCenterX, nClient_H - 100 }, FALSE);
    
    if (bManual)
    {
        DrowOnScreanAlpha(MainDC, ScreanDC, Bitmap_UI_GamePage_Pause_Screen, { nCenterX, 0 }, 150);
        DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_StartPage_Manual, { nCenterX, nClient_H / 2 - 250 }, FALSE);
    }
}

void DrowReadyPage(HDC& MainDC, HDC& ScreanDC)
{
    TCHAR strBuff[255];

    // 강화공간
    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_ReadyPage_Background_Back, { nCenterX, 210 }, FALSE);

    int nUpgradeBlockInterval = 115;

    POINT   IconPoint = { 100, 235 };
    SIZE    IconSize = { 60, 60 };

    POINT   NamePoint = { 165, 235 };
    SIZE    NameSize = { 215, 25 };

    POINT   CommentPoint = { 165, 260 };
    SIZE    CommentSize = { 215, 25 };

    POINT   MoneyPoint = { 165, 285 };
    SIZE    MoneySize = { 155, 25 };

    POINT   BuyPoint = { 320, 285 };
    SIZE    BuySize = { 60, 25 };


    for (int i = 0; i < UPGRADE_COUNT; i++)
    {
        // 강화 종류 틀
        DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_ReadyPage_Upgrade_Block, { nCenterX, 215 + (i * nUpgradeBlockInterval) + nUpgradeScrollValue }, FALSE);

        // 아이콘 공간
        DrowOnScrean(MainDC, ScreanDC, Upgrade[i].bmIcon, { IconPoint.x + IconSize.cx / 2, IconPoint.y + (i * nUpgradeBlockInterval) + nUpgradeScrollValue }, FALSE);

        SetTextAlign(ScreanDC, TA_CENTER);
        SetBkMode(ScreanDC, TRANSPARENT);
        wsprintf(strBuff, TEXT("Lv.%d"), Upgrade[i].unLevel);
        TextOut(ScreanDC, 
            IconPoint.x + IconSize.cx / 2, 
            IconPoint.y + IconSize.cy + 2 + (i * nUpgradeBlockInterval) + nUpgradeScrollValue, 
            (strBuff), lstrlen(strBuff));

        // 이름 공간
        Rectangle(ScreanDC,
            NamePoint.x,
            NamePoint.y + (i * nUpgradeBlockInterval) + nUpgradeScrollValue,
            NamePoint.x + NameSize.cx,
            NamePoint.y + NameSize.cy + (i * nUpgradeBlockInterval) + nUpgradeScrollValue);

        SetTextAlign(ScreanDC, TA_CENTER);
        SetBkMode(ScreanDC, TRANSPARENT);
        wsprintf(strBuff, Upgrade[i].strTitle);
        TextOut(ScreanDC,
            NamePoint.x + NameSize.cx / 2,
            NamePoint.y + 4 + (i * nUpgradeBlockInterval) + nUpgradeScrollValue,
            (strBuff), lstrlen(strBuff));

        // 효과 공간
        Rectangle(ScreanDC,
            CommentPoint.x,
            CommentPoint.y + (i * nUpgradeBlockInterval) + nUpgradeScrollValue,
            CommentPoint.x + CommentSize.cx,
            CommentPoint.y + CommentSize.cy + (i * nUpgradeBlockInterval) + nUpgradeScrollValue);

        SetTextAlign(ScreanDC, TA_CENTER);
        SetBkMode(ScreanDC, TRANSPARENT);
        switch (Upgrade[i].nUpgradeType)
        {
        case eMaxHP_Upgrade:
        case eMaxJump_Upgrade:
        case eJump_Upgrade:
        case eMaxSpeed_Upgrade:
        case eHeightScore_Upgrade:
        case eMoney_Upgrade:
            wsprintf(strBuff, Upgrade[i].strComment, Upgrade[i].unLevel);
            break;
        case eItemScore_Upgrade:
            wsprintf(strBuff, Upgrade[i].strComment, Upgrade[i].unLevel + 1);
            break;
        case eShiled_Upgrade:
            wsprintf(strBuff, Upgrade[i].strComment);
            break;
        default:
            break;
        }
        TextOut(ScreanDC,
            CommentPoint.x + CommentSize.cx / 2,
            CommentPoint.y + 4 + (i * nUpgradeBlockInterval) + nUpgradeScrollValue,
            (strBuff), lstrlen(strBuff));

        // 강화 비용 공간
        Rectangle(ScreanDC,
            MoneyPoint.x,
            MoneyPoint.y + (i * nUpgradeBlockInterval) + nUpgradeScrollValue,
            MoneyPoint.x + MoneySize.cx,
            MoneyPoint.y + MoneySize.cy + (i * nUpgradeBlockInterval) + nUpgradeScrollValue);

        switch (Upgrade[i].nUpgradeType)
        {
        case eMaxHP_Upgrade:
        case eMaxJump_Upgrade:
        case eMaxSpeed_Upgrade:
        case eHeightScore_Upgrade:
        case eMoney_Upgrade:
        case eItemScore_Upgrade:
            wsprintf(strBuff, TEXT("비용 : %5d 메소"), Upgrade[i].unMoney * (Upgrade[i].unLevel + 1));
            break;
        case eJump_Upgrade:
            if (Upgrade[eJump_Upgrade].unLevel >= Upgrade[eMaxJump_Upgrade].unLevel)
                wsprintf(strBuff, TEXT("구매 불가"));
            else
                wsprintf(strBuff, TEXT("비용 : %5d 메소"), Upgrade[i].unMoney * (Upgrade[i].unLevel + 1));
            break;
        case eShiled_Upgrade:
            if (Upgrade[eShiled_Upgrade].unLevel == 0)
                wsprintf(strBuff, TEXT("비용 : %5d 메소"), Upgrade[i].unMoney * (Upgrade[i].unLevel + 1));
            else
                wsprintf(strBuff, TEXT("구매 완료"));
            break;
            wsprintf(strBuff, Upgrade[i].strComment);
            break;
        default:
            break;
        }

        TextOut(ScreanDC,
            MoneyPoint.x + MoneySize.cx / 2,
            MoneyPoint.y + 4 + (i * nUpgradeBlockInterval) + nUpgradeScrollValue,
            (strBuff), lstrlen(strBuff));
        
        // 강화 버튼 공간
        Rectangle(ScreanDC,
            BuyPoint.x,
            BuyPoint.y + (i * nUpgradeBlockInterval) + nUpgradeScrollValue,
            BuyPoint.x + BuySize.cx,
            BuyPoint.y + BuySize.cy + (i * nUpgradeBlockInterval) + nUpgradeScrollValue);
        wsprintf(strBuff, TEXT("구 매"));
        TextOut(ScreanDC,
            BuyPoint.x + BuySize.cx / 2,
            BuyPoint.y + 4 + (i * nUpgradeBlockInterval) + nUpgradeScrollValue,
            (strBuff), lstrlen(strBuff));

    }

    BITMAP bit;
    GetObject(Bitmap_UI_ReadyPage_Main_Background, sizeof(BITMAP), &bit);
    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_ReadyPage_Main_Background, { nCenterX, nClient_H - bit.bmHeight }, FALSE);
    
    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_ReadyPage_Background_Front, { nCenterX, 10 }, FALSE);

    // 최고 점수 공간
    DrowScore(MainDC, ScreanDC, unPlayerHighScore, { nCenterX, 70 });

    // 현제 소지금
    DrowMoney(MainDC, ScreanDC, unTotalMoney, { 140, 122 });

    // 시작 버튼
    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_ReadyPage_StartButton, { nCenterX, nClient_H - 50 }, FALSE);

}

long lFrame = 0;

void DrowGamePage(HDC& MainDC, HDC& ScreanDC)
{

    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_GamePage_Main_Background, { nCenterX, nClient_H - 1800 + nGameBackgroundScrollValue }, FALSE);

    // 땅 그리기
    for (int i = 0; i < MAX_GROUND; i++)
    {
        if (Ground[i].bLive)
        {
            //DrowOnScrean(MainDC, ScreanDC, OBJ_Trap_Bitmap[ETrapType::eTrap_Spear][0], { nCenterX + Ground[i].poPosition.x - 20, START_Y + nViewPointY - Ground[i].poPosition.y - 30}, FALSE);
            //DrowOnScrean(MainDC, ScreanDC, OBJ_Trap_Bitmap[ETrapType::eTrap_Spear][0], { nCenterX + Ground[i].poPosition.x, START_Y + nViewPointY - Ground[i].poPosition.y - 30}, FALSE);
            //DrowOnScrean(MainDC, ScreanDC, OBJ_Trap_Bitmap[ETrapType::eTrap_Spear][0], { nCenterX + Ground[i].poPosition.x + 20, START_Y + nViewPointY - Ground[i].poPosition.y - 30}, FALSE);
            DrowOnScrean(MainDC, ScreanDC, OBJ_Ground_Bitmap[Ground[i].eGroundType], { nCenterX + Ground[i].poPosition.x, START_Y + nViewPointY - Ground[i].poPosition.y }, FALSE);
            MoveToEx(ScreanDC, nCenterX + Ground[i].poPosition.x - 6, START_Y + nViewPointY - Ground[i].poPosition.y - 30, NULL);
        }
    }

    // 그림자 그리기
    if (bShadow)
    {
        int nShadowAlpha = 250 - (Player.poPosition.y - nShadowY) * 2;
        if (nShadowAlpha < 10)
            nShadowAlpha = 10;

        DrowOnScreanAlpha(MainDC, ScreanDC, Player.bmShadow, { nCenterX + Player.poPosition.x, START_Y + nViewPointY - nShadowY + 5 }, nShadowAlpha);
    }

    // 캐릭터 그리기
    DrowOnScrean(MainDC, ScreanDC, Player.bmNow, { nCenterX + Player.poPosition.x, START_Y + nViewPointY - 38 - Player.poPosition.y }, Player.bRight);

    // 아이템 그리기
    for (int i = 0; i < MAX_ITEM; i++)
    {
        if (Item[i].bLive)
        {
            DrowOnScrean(MainDC, ScreanDC, OBJ_Item_Bitmap[Item[i].nItemType], { nCenterX + Item[i].poPosition.x,  START_Y + nViewPointY - 15 - Item[i].poPosition.y }, FALSE);
        }
    }

    // 함정 그리기
    for (int i = 0; i < MAX_TRAP; i++)
    {
        if (Trap[i].bLive)
        {
            DrowOnScrean(MainDC, ScreanDC, OBJ_Trap_Bitmap[Trap[i].eTrapType][Get_TrapImageFrame(Trap[i])], { nCenterX + Trap[i].poPosition.x, START_Y + nViewPointY - 25 - Trap[i].poPosition.y }, Trap[i].bRight);
        }
    }

    lFrame++;
    // 바닥 함정 그리기
    for (int i = 0; i < 24; i++)
    {
        DrowOnScrean(MainDC, ScreanDC, OBJ_Trap_Bitmap[ETrapType::eTrap_Spear][0], { 12 + i * 20, nClient_H - 20 - (long)(sin(M_PI + ((i + (lFrame / 4.0))* M_PI_4)) * 4) }, FALSE);
    }

    // 일시정지 버튼
    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_GamePage_PauseButton, { nClient_W - 37, 17 }, FALSE);

    // 획득 메소
    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_GamePage_Money, { nCenterX, 16 }, FALSE);
    DrowMoney(MainDC, ScreanDC, unPlayingMoney, { nCenterX - 33, 24 });

    // 점수
    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_GamePage_Input, { nCenterX, 55 }, FALSE);
    DrowScore(MainDC, ScreanDC, unTotalScore, { nCenterX, 63 });

    TCHAR strBuff[255];

    // 체력 바
    POINT   HPBarPoint = { 0, 20 };
    
    int     nHPPer = Player.nCurrentHP * 100 / Player.nMAXHp;
    int     nHPSzie = 150 * nHPPer / 100;

    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_GamePage_HP_Background, { HPBarPoint.x + 75, HPBarPoint.y }, FALSE);

    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_GamePage_HP, { HPBarPoint.x - 75 + nHPSzie, HPBarPoint.y }, FALSE);

    SetTextAlign(ScreanDC, TA_CENTER);
    SetBkMode(ScreanDC, TRANSPARENT);
    wsprintf(strBuff, TEXT("HP"));
    TextOut(ScreanDC, 70, 22, (strBuff), lstrlen(strBuff));
    wsprintf(strBuff, TEXT("%2d / %2d"), Player.nCurrentHP, Player.nMAXHp);
    TextOut(ScreanDC, 70, 33, (strBuff), lstrlen(strBuff));

    // 일시정지 상태
    if (bGamePause)
    {
        DrowOnScreanAlpha(MainDC, ScreanDC, Bitmap_UI_GamePage_Pause_Screen, { nCenterX, 0 }, 150);
        DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_GamePage_Pause_Text, { nCenterX, 140 }, FALSE);
        
        for (int i = 0; i < 3; i++)
        {
            DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_ReadyPage_Upgrade_Block, { nCenterX, 260 + 120 * i }, FALSE);
            DrowOnScrean(MainDC, ScreanDC, OBJ_Pause_Font_Bitmap[i], { nCenterX, 290 + 120 * i }, FALSE);
        }
    }

}

void DrowGameOverPage(HDC& MainDC, HDC& ScreanDC)
{
    DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_GameOverPage_Main_Background, { nCenterX, 0 }, FALSE);
    DrowScore(MainDC, ScreanDC, unTotalScore, { nCenterX, 280 });

    if (bHighScore)
    {
        DrowOnScrean(MainDC, ScreanDC, Bitmap_UI_GameOverPage_HighScore, { nCenterX, 345 }, FALSE);
    }
}

BOOL ClickCheckStartPage(POINT poMousePoint)
{
    if (bManual)
    {
        if (HitBoxCheck({ nCenterX - 60, nClient_H / 2 + 196, nCenterX + 60, nClient_H / 2 + 238 }, poMousePoint))
        {
            bManual = FALSE;

            return TRUE;
        }
    }
    else
    {
        if (HitBoxCheck({ nCenterX - 115, nClient_H - 160 , nCenterX + 115, nClient_H - 120 }, poMousePoint))
        {
            nUpgradeScrollValue = 0;
            nNowGamePage = EGamePage::eReadyPage;

            return TRUE;
        }

        if (HitBoxCheck({ nCenterX - 100, nClient_H - 100, nCenterX + 100, nClient_H - 66 }, poMousePoint))
        {
            bManual = TRUE;

            return TRUE;
        }
    }
    
    return FALSE;
}

BOOL ClickCheckReadyPage(POINT poMousePoint)
{
    if (HitBoxCheck({ nCenterX - 180, 210, nCenterX + 180, 570 }, poMousePoint))
    {
        int nUpgradeBlockInterval = 115;

        POINT   BuyPoint = { 320, 285 };
        SIZE    BuySize = { 60, 25 };

        for (int i = 0; i < UPGRADE_COUNT; i++)
        {

            if (HitBoxCheck({ BuyPoint.x,
                BuyPoint.y + (i * nUpgradeBlockInterval) + nUpgradeScrollValue,
                BuyPoint.x + BuySize.cx,
                BuyPoint.y + BuySize.cy + (i * nUpgradeBlockInterval) + nUpgradeScrollValue }, poMousePoint))
            {
                switch (Upgrade[i].nUpgradeType)
                {
                case eMaxHP_Upgrade:
                case eMaxJump_Upgrade:
                case eMaxSpeed_Upgrade:
                case eItemScore_Upgrade:
                case eHeightScore_Upgrade:
                case eMoney_Upgrade:
                    if (Upgrade_LevelUp(unTotalMoney, Upgrade[i]))
                        return TRUE;
                    break;
                case eJump_Upgrade:
                    if (Upgrade[eJump_Upgrade].unLevel < Upgrade[eMaxJump_Upgrade].unLevel)
                    {
                        if (Upgrade_LevelUp(unTotalMoney, Upgrade[i]))
                            return TRUE;
                    }
                    break;
                case eShiled_Upgrade:
                    if (Upgrade[eShiled_Upgrade].unLevel == 0)
                    {
                        if (Upgrade_LevelUp(unTotalMoney, Upgrade[i]))
                            return TRUE;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }

    if (HitBoxCheck({ nCenterX - 145/2, nClient_H - 50, nCenterX + 145 / 2, nClient_H - 6 }, poMousePoint))
    {
        // 초기화
        Reset_GamePageData();
        nNowGamePage = EGamePage::eGamePage;
        return TRUE;
    }

    return FALSE;
}

BOOL ClickCheckGamePage(POINT poMousePoint)
{
    // 일시정지 상태
    if (bGamePause)
    {
        if (HitBoxCheck({ nCenterX - 115, 260, nCenterX + 115, 370 }, poMousePoint))
        {
            bGamePause = FALSE;
            return TRUE;
        }

        if (HitBoxCheck({ nCenterX - 115, 380, nCenterX + 115, 490 }, poMousePoint))
        {
            Reset_GamePageData();
            return TRUE;
        }

        if (HitBoxCheck({ nCenterX - 115, 500, nCenterX + 115, 610 }, poMousePoint))
        {
            Reset_GamePageData();
            nNowGamePage = EGamePage::eReadyPage;
            return TRUE;
        }
    }
    else
    {
        if (HitBoxCheck({ nClient_W - 57, 17, nClient_W - 17, 57 }, poMousePoint))
        {
            bGamePause = TRUE;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL ClickCheckGameOverPage(POINT poMousePoint)
{
    if (HitBoxCheck({ 136, 462, 349, 528 }, poMousePoint))
    {
        nNowGamePage = EGamePage::eReadyPage;
        return TRUE;
    }
    return FALSE;
}

BOOL WheelCheckStartPage(short shWheelValue)
{
    return 0;
}

BOOL WheelReadyPage(short shWheelValue)
{
    if (HitBoxCheck({ 20, 170, nClient_W - 20, nClient_H - 70 }, poMousePoint))
    {
        if (shWheelValue > 0)
        {
            nUpgradeScrollValue += 10;
            if (nUpgradeScrollValue > 0)
                nUpgradeScrollValue = 0;
        }

        if (shWheelValue < 0)
        {
            nUpgradeScrollValue -= 10;
            if (nUpgradeScrollValue < -(115 * UPGRADE_COUNT) + nClient_H - 305)
                nUpgradeScrollValue = -(115 * UPGRADE_COUNT) + nClient_H - 305;
        }

        return TRUE;
    }
    return FALSE;
}

BOOL WheelGamePage(short shWheelValue)
{
    return 0;
}

BOOL WheelGameOverPage(short shWheelValue)
{
    return 0;
}

void LoopStartPage(HWND& hwnd)
{

}

void LoopReadyPage(HWND& hwnd)
{
    if (GetKeyState(VK_RETURN) & 0x80)
    {
        if (bEnterDown == FALSE)
        {
            bEnterDown = TRUE;
            Reset_GamePageData();
            nNowGamePage = EGamePage::eGamePage;
            InvalidateRect(hwnd, NULL, FALSE);
        }
    }
    else
    {
        bEnterDown = FALSE;
    }
}

void LoopGamePage(HWND& hwnd)
{
    if (GetKeyState(VK_ESCAPE) & 0x80)
    {
        if (bEscDown == FALSE)
        {
            if (bGamePause)
            {
                bGamePause = FALSE;
            }
            else
            {
                bGamePause = TRUE;
            }

            InvalidateRect(hwnd, NULL, FALSE);
            bEscDown = TRUE;
        }
    }
    else
    {
        bEscDown = FALSE;
    }

    if (bGamePause == FALSE)
    {
        Move_Player(hwnd, Player);
        PlayAnimation_Player(Player, ClientTime);

        nCenterX = nClient_W / 2;

        RECT PlayerHitBox = {
            nCenterX + Player.poPosition.x - 15 ,
            START_Y + nViewPointY - 25 - Player.poPosition.y,
            nCenterX + Player.poPosition.x + 15,
            START_Y + nViewPointY + 5 - Player.poPosition.y
        };

        // 보는 시점 조절
        nViewPointY += nGroundDownSpeed;
        if (Player.poPosition.y > nViewPointY + 200)
        {
            nHeightScoreCount += Player.poPosition.y - nViewPointY - 200;
            nTrapCreateCount += Player.poPosition.y - nViewPointY - 200;
            nViewPointY = Player.poPosition.y - 200;
        }

        // 점수 관련
        nHeightScoreCount += nGroundDownSpeed;
        if (nHeightScoreCount >= 50)
        {
            nGameBackgroundScrollValue++;
            unHeightScore += nGroundDownSpeed * (Upgrade[EUpgradeType::eHeightScore_Upgrade].unLevel + 1);
            nHeightScoreCount -= 50;
        }

        unTotalScore = unHeightScore + unItemScore;
        if (unTotalScore > 999999999)
            unTotalScore = 999999999;

        // 배경 조정
        if (nClient_H - 1800 + nGameBackgroundScrollValue > 0)
            nGameBackgroundScrollValue = 1800 - nClient_H;

        // 그림자 위치 조정
        bShadow = FALSE;
        nShadowY = -200;

        for (int i = 0; i < MAX_GROUND; i++)
        {
            if (Ground[i].bLive)
            {
                if (nCenterX - Ground[i].nWidth / 2 + Ground[i].poPosition.x + 5 <= nCenterX + 15 + Player.poPosition.x &&
                    nCenterX + Ground[i].nWidth / 2 + Ground[i].poPosition.x - 5 >= nCenterX - 15 + Player.poPosition.x &&
                    START_Y + 10 - Ground[i].poPosition.y >= START_Y + 10 - Player.poPosition.y)
                {
                    bShadow = TRUE;
                    if (nShadowY < Ground[i].poPosition.y)
                        nShadowY = Ground[i].poPosition.y;
                }
            }
        }

        // 공중에 떠있는지 판정 관련
        if (!bShadow || nShadowY != Player.poPosition.y)
        {
            Player.bJump = TRUE;
            Player.nStatus = EPlayerStatus::Jump;
        }

        // 시점에서 안보이는 땅 삭제 및 재생성
        for (int i = 0; i < MAX_GROUND; i++)
        {
            if (Ground[i].bLive)
            {
                if (START_Y + nViewPointY - Ground[i].poPosition.y - 50 > nClient_H)
                {
                    nHighGroundY += 100;
                    Delete_Ground(Ground[i]);
                    nBeforeGroundPointX += rand() % 300 - 150;
                    if (nBeforeGroundPointX + nCenterX < 0)
                        nBeforeGroundPointX += nCenterX;
                    if (nBeforeGroundPointX + nCenterX > nClient_W)
                        nBeforeGroundPointX -= nCenterX;
                    Create_Ground({ nBeforeGroundPointX, nHighGroundY }, Ground[i], EGroundType::eGround_Small);
                }
            }
        }

        // 아이템 관련
        nItemCreateCount++;
        for (int i = 0; i < MAX_ITEM; i++)
        {
            if (Item[i].bLive)
            {
                // 획득 시
                if (nCenterX + Player.poPosition.x + 17 >= nCenterX + Item[i].poPosition.x - 15 &&
                    nCenterX + Player.poPosition.x - 17 <= nCenterX + Item[i].poPosition.x + 15 &&
                    START_Y + nViewPointY - Player.poPosition.y + 15 >= START_Y + nViewPointY - Item[i].poPosition.y - 15 &&
                    START_Y + nViewPointY - Player.poPosition.y - 15 <= START_Y + nViewPointY - Item[i].poPosition.y + 15)
                {
                    switch (Item[i].nItemType)
                    {
                    case ePostion:
                    {
                        unItemScore += Item[i].nScore;
                        Player.nCurrentHP++;
                        if (Player.nCurrentHP > Player.nMAXHp)
                            Player.nCurrentHP = Player.nMAXHp;
                    }
                    break;
                    case eFullPpstion:
                    {
                        unItemScore += Item[i].nScore;
                        Player.nCurrentHP = Player.nMAXHp;
                    }
                    break;
                    case eScore1:
                    case eScore2:
                    case eScore3:
                    case eScore4:
                    {
                        unItemScore += Item[i].nScore * (Upgrade[EUpgradeType::eItemScore_Upgrade].unLevel + 1);
                    }
                    break;
                    case eGameSpeedUp:
                    {
                        unItemScore += Item[i].nScore;
                        nGroundDownSpeed++;
                        if (nGroundDownSpeed > GROUND_DOWN_MAX_SPEED)
                            nGroundDownSpeed = GROUND_DOWN_MAX_SPEED;
                    }
                    break;
                    case eGameSpeedDown:
                    {
                        unItemScore += Item[i].nScore;
                        nGroundDownSpeed--;
                        if (nGroundDownSpeed < 1)
                            nGroundDownSpeed = 1;
                    }
                    break;
                    case eJumpUp:
                    {
                        unItemScore += Item[i].nScore;
                        if (Player.nExtraJumpMAXCount < Upgrade[EUpgradeType::eMaxJump_Upgrade].unLevel)
                        {
                            Player.nExtraJumpMAXCount++;
                            Player.nExtraJumpCount++;
                        }
                    }
                    break;
                    case eShield:
                    {
                        unItemScore += Item[i].nScore;
                        Player.bShield = TRUE;
                    }
                    break;
                    case eSpeedUp:
                    {
                        unItemScore += Item[i].nScore;
                        if (Player.dMaxSpeed < 10 + (Upgrade[EUpgradeType::eMaxSpeed_Upgrade].unLevel * 2))
                            Player.dMaxSpeed += 2;
                    }
                    break;
                    case eMoney:
                    case eBigMoney:
                    {
                        unPlayingMoney += Item[i].nScore * (Upgrade[EUpgradeType::eMoney_Upgrade].unLevel + 1);
                    }
                    break;
                    default:
                        break;
                    }

                    Delete_Item(Item[i]);
                }

                // 바닥 아래로 떨어졌을 시
                if (START_Y + nViewPointY - Item[i].poPosition.y - 50 > nClient_H)
                {
                    Delete_Item(Item[i]);
                }

                Item[i].poPosition.y--;
            }
            else
            {
                if (nItemCreateCount >= 120)
                {
                    nItemCreateCount -= 120;

                    int nItemCode = rand() % 1000;

                    // 0.5%
                    if (nItemCode < 5)
                    {
                        nItemCode = EItemType::eFullPpstion;
                    }
                    // 0.5%
                    else if (nItemCode < 10)
                    {
                        nItemCode = EItemType::eShield;
                    }
                    // 5%
                    else if (nItemCode < 60)
                    {
                        nItemCode = EItemType::ePostion;
                    }
                    // 5%
                    else if (nItemCode < 110)
                    {
                        nItemCode = EItemType::eGameSpeedUp;
                    }
                    // 5%
                    else if (nItemCode < 160)
                    {
                        nItemCode = EItemType::eGameSpeedDown;
                    }
                    // 10%
                    else if (nItemCode < 260)
                    {
                        nItemCode = EItemType::eJumpUp;
                    }
                    // 10%
                    else if (nItemCode < 360)
                    {
                        nItemCode = EItemType::eSpeedUp;
                    }
                    // 30%
                    else if (nItemCode < 660)
                    {
                        nItemCode = EItemType::eMoney;
                    }
                    // 34%
                    else
                    {
                        nItemCode = EItemType::eScore1 + rand() % 4;
                    }

                    Create_Item({ rand() % nClient_W - nCenterX, START_Y + nViewPointY + 100 }, Item[i], nItemCode);
                }
            }
        }

        // 함정 관련
        nTrapCreateCount += nGroundDownSpeed;
        for (int i = 0; i < MAX_TRAP; i++)
        {
            RECT HitBox;

            if (Trap[i].bLive)
            {
                Trap[i].nFrame++;

                switch (Trap[i].eTrapType)
                {
                case eTrap_Snowball:
                {
                    // 움직임
                    if (Trap[i].bRight)
                    {
                        Trap[i].poPosition.x++;
                        HitBox = {
                            nCenterX + Trap[i].poPosition.x + 10,
                            START_Y + nViewPointY - 8 - Trap[i].poPosition.y,
                            nCenterX + Trap[i].poPosition.x + 25,
                            START_Y + nViewPointY + 8 - Trap[i].poPosition.y };
                    }
                    else
                    {
                        Trap[i].poPosition.x--;
                        HitBox = {
                            nCenterX + Trap[i].poPosition.x - 10,
                            START_Y + nViewPointY - 8 - Trap[i].poPosition.y,
                            nCenterX + Trap[i].poPosition.x - 25,
                            START_Y + nViewPointY + 8 - Trap[i].poPosition.y };
                    }

                }
                break;
                case eTrap_Rain_Ice:
                {
                    // 움직임
                    Trap[i].poPosition.y--;
                    HitBox = {
                            nCenterX + Trap[i].poPosition.x - 5,
                            START_Y + nViewPointY - 0 - Trap[i].poPosition.y,
                            nCenterX + Trap[i].poPosition.x + 5,
                            START_Y + nViewPointY + 25 - Trap[i].poPosition.y };
                }
                break;
                case eTrap_Spear:
                {
                    // 움직임
                    HitBox = {
                            nCenterX + Trap[i].poPosition.x - 5,
                            START_Y + nViewPointY - 0 - Trap[i].poPosition.y,
                            nCenterX + Trap[i].poPosition.x + 5,
                            START_Y + nViewPointY + 25 - Trap[i].poPosition.y };
                }
                break;
                default:
                    break;
                }

                // 판정
                if (HitBoxCheck(HitBox, PlayerHitBox))
                {
                    if (Player.bShield)
                        Player.bShield = FALSE;
                    else
                        Player.nCurrentHP--;
                    Delete_Trap(Trap[i]);
                }

                // 화면 밖으로 벗어남
                if ((Trap[i].bRight && Trap[i].poPosition.x > nCenterX + 100) || (!Trap[i].bRight && Trap[i].poPosition.x < -nCenterX - 100) ||
                    START_Y + nViewPointY - Trap[i].poPosition.y - 50 > nClient_H)
                {
                    Delete_Trap(Trap[i]);
                }
            }
            else
            {
                if (nTrapCreateCount >= 300)
                {
                    nTrapCreateCount -= 300;

                    int nTrapType = rand() % ETrapType::eTrapTypeCount;
                    POINT poTrapPosition;
                    BOOL bRight = rand() % 2;

                    switch (nTrapType)
                    {
                    case eTrap_Snowball:
                    {
                        if (bRight)
                        {
                            poTrapPosition = { -nCenterX - 100, (rand() % nClient_H) + nViewPointY };
                        }
                        else
                        {
                            poTrapPosition = { +nCenterX + 100, (rand() % nClient_H) + nViewPointY };
                        }

                    }
                    break;
                    case eTrap_Rain_Ice:
                    {
                        poTrapPosition = { rand() % nClient_W - nCenterX, START_Y + nViewPointY + 100 };
                    }
                    break;
                    case eTrap_Spear:
                    {
                        nTrapType = ETrapType::eTrap_Rain_Ice;
                        poTrapPosition = { rand() % nClient_W - nCenterX, START_Y + nViewPointY + 100 };
                    }
                    break;
                    default:
                        break;
                    }

                    Create_Trap(poTrapPosition, Trap[i], nTrapType, bRight);
                }
            }
        }

        InvalidateRect(hwnd, NULL, FALSE);

        // 점프 후 착지 관련
        if (Player.dPositionY > nShadowY &&
            Player.dPositionY - Player.dNowGravity <= nShadowY)
        {
            Player.nStatus = EPlayerStatus::Idle;
            Player.bJump = FALSE;
            Player.dNowGravity = 0;
            Player.dPositionY = nShadowY;
            Player.poPosition.y = nShadowY;
            Player.nExtraJumpCount = Player.nExtraJumpMAXCount;
        }

        // 게임 종료 시점
        if (START_Y + nViewPointY - Player.poPosition.y - 38 > nClient_H || Player.nCurrentHP <= 0)
        {
            if (unPlayerHighScore < unTotalScore)
            {
                unPlayerHighScore = unTotalScore;
                bHighScore = TRUE;
            }
            Upgrade[EUpgradeType::eShiled_Upgrade].unLevel = 0;
            unTotalMoney += unPlayingMoney;
            nNowGamePage = EGamePage::eGameOverPage;
            nItemCreateCount = 0;
            SaveGame();
        }
    }

}

void LoopGameOverPage(HWND& hwnd)
{
    if (GetKeyState(VK_RETURN) & 0x80)
    {
        if (bEnterDown == FALSE)
        {
            bEnterDown = TRUE;
            nNowGamePage = EGamePage::eReadyPage;
            InvalidateRect(hwnd, NULL, FALSE);
        }
    }
    else
    {
        bEnterDown = FALSE;
    }
}
