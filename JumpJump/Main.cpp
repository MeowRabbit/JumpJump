#include <windows.h>
#include "GameSystem.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void LoopClient(HWND);

HINSTANCE g_hInstance;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("Walk_Training");
    HWND         hwnd;
    MSG          msg;
    WNDCLASS     wndclass;
    g_hInstance = hInstance;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    RegisterClass(&wndclass);

    hwnd = CreateWindow(szAppName,
        TEXT("산책훈련"),
        WS_SYSMENU | WS_CAPTION,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CLIENT_W,
        CLIENT_H,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    
    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // 초당 60프레임 제어
            // 1000 : 1초
            // 1000 / 60 : 1초에 60번
            if (GetTickCount64() >= ClientTime + 1000 / 60)
            {
                ClientTime += 1000 / 60;
                LoopClient(hwnd);
            }
        }
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC			hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
    case WM_CREATE:
        // 게임 기본 셋팅
        Init_GameData(g_hInstance, hwnd);
        return 0;

    case WM_SIZE:
        // 사이즈 변경 시 실제 사이즈 가져오기
        nClient_H = HIWORD(lParam);
        nClient_W = LOWORD(lParam);
        nCenterX = nClient_W / 2;

        InvalidateRect(hwnd, NULL, FALSE);

        return 0;
    case WM_MOUSEMOVE:
        // 화면 위의 마우스 위치 가져오기
        // LOWORD : X축 위치
        // HIWORD : Y축 위치
        poMousePoint = { (short)LOWORD(lParam), (short)HIWORD(lParam) };
        return 0;
        
    case WM_MOUSEWHEEL:
        if (nClient_H && nClient_W)
        {
            // nNowGamePage : 현제 게임 페이지 위치
            switch (nNowGamePage)
            {
            case eStartPage:
            {
                if(WheelCheckStartPage(GET_WHEEL_DELTA_WPARAM(wParam)))
                    InvalidateRect(hwnd, NULL, FALSE);
            }
            break;
            case eReadyPage:
            {
                if (WheelReadyPage(GET_WHEEL_DELTA_WPARAM(wParam)))
                    InvalidateRect(hwnd, NULL, FALSE);
            }
            break;
            case eGamePage:
            {
                if (WheelGamePage(GET_WHEEL_DELTA_WPARAM(wParam)))
                    InvalidateRect(hwnd, NULL, FALSE);
            }
                break;
            case eGameOverPage:
            {
                if (WheelGameOverPage(GET_WHEEL_DELTA_WPARAM(wParam)))
                    InvalidateRect(hwnd, NULL, FALSE);
            }
            break;
            default:
                break;
            }
        }

        break;

    case WM_LBUTTONDOWN:
        if (nClient_H && nClient_W)
        {
            // 마우스 클릭 위치
            // LOWWORD : X축
            // HIWORD : Y축
            POINT MouseClickPoint = { LOWORD(lParam) , HIWORD(lParam) };

            // nNowGamePage : 현제 게임 페이지 위치
            switch (nNowGamePage)
            {
            case eStartPage:
                if(ClickCheckStartPage(MouseClickPoint))
                    InvalidateRect(hwnd, NULL, FALSE);
                break;
            case eReadyPage:
                if(ClickCheckReadyPage(MouseClickPoint))
                    InvalidateRect(hwnd, NULL, FALSE);
                break;
            case eGamePage:
                if(ClickCheckGamePage(MouseClickPoint))
                    InvalidateRect(hwnd, NULL, FALSE);
                break;
            case eGameOverPage:
                if(ClickCheckGameOverPage(MouseClickPoint))
                    InvalidateRect(hwnd, NULL, FALSE);
                break;
            default:
                break;
            }
        }
        
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        {
            if (nClient_H && nClient_W)
            {
                HDC BG_MemDC;
                HBITMAP BG_Bitmap, BG_OldBitmap;

                // 메모리 DC 생성
                BG_MemDC = CreateCompatibleDC(hdc);

                // 화면 크기에 맞는 가상의 메모리 비트맵 생성
                BG_Bitmap = CreateCompatibleBitmap(hdc, nClient_W, nClient_H);

                // 메모리 DC에 비트맵 선택
                BG_OldBitmap = (HBITMAP)SelectObject(BG_MemDC, BG_Bitmap);

                // nNowGamePage : 현제 게임 페이지 위치
                switch (nNowGamePage)
                {
                case eStartPage:
                {
                    DrowStartPage(hdc, BG_MemDC);
                }
                    break;
                case eReadyPage:
                {
                    DrowReadyPage(hdc, BG_MemDC);
                }
                    break;
                case eGamePage:
                {
                    DrowGamePage(hdc, BG_MemDC);
                }
                    break;
                case eGameOverPage:
                {
                    DrowGameOverPage(hdc, BG_MemDC);
                }
                    break;
                default:
                    break;
                }
                
                // 메모리 DC에서 만든 메모리 비트맵을 화면으로 전송
                BitBlt(hdc, 0, 0, nClient_W, nClient_H, BG_MemDC, 0, 0, SRCCOPY);

                // 메모리 비트맵 반환
                SelectObject(BG_MemDC, BG_OldBitmap);

                // 만들어진 메모리 DC 및 비트맵 삭제
                DeleteDC(BG_MemDC);
                DeleteObject(BG_Bitmap);
                
            }
        }
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(WM_QUIT);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

// 실시간 반복용 함수
void LoopClient(HWND hwnd)
{
    if (nClient_H && nClient_W)
    {
        // nNowGamePage : 현제 게임 페이지 위치
        switch (nNowGamePage)
        {
        case eStartPage:
            LoopStartPage(hwnd);
            break;
        case eReadyPage:
            LoopReadyPage(hwnd);
            break;
        case eGamePage:
            LoopGamePage(hwnd);
            break;
        case eGameOverPage:
            LoopGameOverPage(hwnd);
            break;
        default:
            break;
        }
    }
}