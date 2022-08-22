// 引用的头文件
#include <windows.h> // Windows窗口程序编程，需要引用头文件 Windows.h
#include "block.h"

#define CELL_PIXEL 20
#define COLOR_BLOCK RGB(193, 205, 205)
#define COLOR_BOUNDARY RGB(139, 134, 130)
#define COLOR_TEXT RGB(173, 216, 230)
#define INIT_TIMER_ELAPSE 500 // 初始的时钟周期，确定游戏初始速度
#define ONE_LEVELS_SCORES 2  // 每升级一次需要的计分
#define SPEEDUP_RATIO 0.5     // 升级以后时间周期（确定游戏速度）提高的比例。
#define MAX_X 10              // 游戏界面大小
#define MAX_Y 12              // 游戏界面大小

HINSTANCE hinst;
RECT rectBoundary;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL SpeedUp(HWND hwnd);

int WINAPI WinMain(
    HINSTANCE hinstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    WNDCLASS wc;
    HWND hwnd;

    MSG msg;
    int fGotMessage;

    hinst = hinstance;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = "MainMenu";
    wc.lpszClassName = "MainWClass";

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, "FAILED", "error!", MB_ICONERROR | MB_OK);
        return -1;
    }

    hwnd = CreateWindow(
        "MainWClass",
        "Tetris!!",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (HWND)NULL,
        (HMENU)NULL,
        hinstance,     
        (LPVOID)NULL);

    if (!hwnd)
    {
        MessageBox(NULL, "Failed", "error!", MB_ICONERROR | MB_OK);
        return -1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0 && fGotMessage != -1)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

void GamePaint(HWND hwnd)
{
    HPEN hpen;
    HDC hdc, hdcmem;
    HBITMAP hbmMem;

    HPEN hPenBoundary;
    HPEN hOldPen;

    HBRUSH hBrushBlock;
    HBRUSH hOldBrush;

    HFONT hFont, hOldFont;

    RECT rect;

    PGAME_COORD pBlock;

    GetClientRect(hwnd, &rect);

    hdc = GetDC(hwnd);

    hdcmem = CreateCompatibleDC(hdc);
    hbmMem = CreateCompatibleBitmap(hdc,rect.right - rect.left, rect.bottom - rect.top);

    SelectObject(hdcmem, hbmMem);

    hBrushBlock = CreateSolidBrush(COLOR_BLOCK);
    hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
    hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);

    FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    hOldBrush = (HBRUSH)SelectObject(hdcmem,hBrushBlock);
    hOldPen = (HPEN)SelectObject(hdcmem, hpen);

    for (int i = 0; i < 4; i++)
	{
		if (GetBlock(i).y<12){
        Rectangle(hdcmem,
            GetBlock(i).x * CELL_PIXEL + rectBoundary.left,
            (11 - GetBlock(i).y) * CELL_PIXEL + rectBoundary.top,
            (GetBlock(i).x + 1) * CELL_PIXEL + rectBoundary.left,
            (12 - GetBlock(i).y) * CELL_PIXEL + rectBoundary.top);
    }

    }


    SelectObject(hdcmem, hBrushBlock);

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 12; j++) {
            if (isseg(i,j)==EXIST)
            {
                Rectangle(hdcmem,
                    i * CELL_PIXEL + rectBoundary.left,
                    (11 - j) * CELL_PIXEL + rectBoundary.top,
                    (i+ 1) * CELL_PIXEL + rectBoundary.left,
                    (12 - j) * CELL_PIXEL + rectBoundary.top);
            }
        }
        
    }
    SelectObject(hdcmem, hPenBoundary);

    // 将PEN移动到需要绘制的方框的左上角
    MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
    // 画了一个方框。演示LineTo函数
    LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
    LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
    LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
    LineTo(hdcmem, rectBoundary.left, rectBoundary.top);

    // 创建了一个字体对象
    hFont = CreateFont(26, 0, 0, 0, FW_DONTCARE, 0, 1, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                       CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));

    // 将这个FONT对象放入DC中
    if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
    {
        CHAR szSourceInfo[1024];
        wsprintf(szSourceInfo, "score %d level %d", GetScore(), GetLevel());
        // 设置输出颜色
        SetTextColor(hdcmem, COLOR_TEXT);
        // 输出字符串。
        TextOut(hdcmem, rectBoundary.left + 10, rectBoundary.bottom +10,
                szSourceInfo, lstrlen(szSourceInfo));
        // 输出完成，将原来的字体对象放回DC中
        SelectObject(hdcmem, hOldFont);
    }

    // 在内存DC中画完，一次输出的窗口DC上。
    BitBlt(hdc,
           0, 0, rect.right - rect.left, rect.bottom - rect.top,
           hdcmem, 0, 0, SRCCOPY);

    DeleteObject(hbmMem);
    DeleteObject(hdcmem);
    DeleteObject(hBrushBlock);
    DeleteObject(hpen);
    DeleteObject(hPenBoundary);
    DeleteObject(hFont);
    ReleaseDC(hwnd, hdc);
}


void ReSizeGameWnd(HWND hwnd)
{
    POINT ptLeftTop;     // 左上角
    POINT ptRightBottom; // 右下角
    RECT rectWindow;
    PGAME_COORD pCoordBoundary = GetBoundary();

    // 设置游戏边界
    rectBoundary.left = 10;
    rectBoundary.top = 10;
    rectBoundary.right = 10 + CELL_PIXEL * (pCoordBoundary->x);
    rectBoundary.bottom = 10 + CELL_PIXEL * (pCoordBoundary->y );

    // 计算上下左右角的位置
    ptLeftTop.x = rectBoundary.left;
    ptLeftTop.y = rectBoundary.top;
    ptRightBottom.x = rectBoundary.right;
    ptRightBottom.y = rectBoundary.bottom;
    ClientToScreen(hwnd, &ptLeftTop);
    ClientToScreen(hwnd, &ptRightBottom);

    GetWindowRect(hwnd, &rectWindow);
    // 计算好了，设置窗口大小。
    MoveWindow(hwnd,
               rectWindow.left,
               rectWindow.top,
               ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // 保存边界和左右两边边框相等。
               rectBoundary.bottom + 120,                                         //给积分信息留出显示空间。
               TRUE);
}

LRESULT CALLBACK MainWndProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        CreateGame(hwnd,
                   INIT_TIMER_ELAPSE,
                   ONE_LEVELS_SCORES,
                   SPEEDUP_RATIO,
                   MAX_X, MAX_Y);
        ReSizeGameWnd(hwnd);
        break;
    case WM_PAINT:
        GamePaint(hwnd);
        break;
    case WM_KEYDOWN:
        OnKeyDown(wParam);
        GamePaint(hwnd);
        break;
    case WM_LBUTTONDOWN:
        OnTimer(hwnd);
        GamePaint(hwnd);
        break;
    case WM_TIMER:
        OnTimer(hwnd);
        GamePaint(hwnd);
        break;
    case WM_DESTROY:
        ExitProcess(0);
        break;

    default:
        break;
    }
    return DefWindowProc(hwnd,
                         msg,
                         wParam,
                         lParam);
}