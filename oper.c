#include <Windows.h>
#include "block.h"

/************** 宏定义 ****************/
// 计时器ID。
#define TIMER_ID 12340
// 判断游戏是否达到升级条件
#define IS_SPEEDUP(s) (s % dwOneLevelScores) == 0



/************** 全局变量 ****************/
// 计时器到时间隔
DWORD dwTimerElapse;
// 每提高一级，计时器时钟周期缩短的比例。
DOUBLE dbLevelSpeedupRatio;
// 此变量表示多少个积分上升一个级别
DWORD dwOneLevelScores;

// 保存计分，初始为0
int score = 0;
// 保存级别，初始为0
int level = 0;


/************** 函数定义 ****************/
// 返回积分
int GetScore() { return score; }

// 返回level
int GetLevel() { return level; }

// 提高速度。
BOOL SpeedUp(HWND hwnd)
{
    // 调整计时器到时时间
    dwTimerElapse = (DWORD)(dwTimerElapse * dbLevelSpeedupRatio);
    KillTimer(hwnd, TIMER_ID);
    SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);
    return TRUE;
}

// 游戏控制的一个主要流程。
// 当计时器发生时进行的处理逻辑。
void OnTimer(HWND hwnd)
{
    // 计时器到时以后，块下降一格
    // 根据以后以后的状态，进行后续处理。
    switch (BlockMoveDown())
    {
    //如果成功下降一格，不做任何处理
    case DOWNSUCCESS:
        break;
    // 如果不能下降，说明已经落到底
    // 要加入到沉积块，判断是否游戏结束以及是否有得分并刷新一个新的下落块
    case DOWNFAILED:
        // 将该下降块加入沉积块并判断是否结束
        switch (SegBlockGorwup())
        {
            // 游戏未结束
        case GAMENOTOVER:
            score += RemoveRow();
            NextBlock();
            break;
            // 涨到最高，游戏结束
        case GAMEOVER:
            KillTimer(hwnd, TIMER_ID);
            // 然后通知玩家，Game Over了，退出进程。
            MessageBox(0, "Game Over!", "Game Over!", 0);
            ExitProcess(0);
            break;
        }
        break;
    }
    return;
}

// 当用于操作时进行的逻辑处理。
// 本游戏只使用到了键盘上下左右键控制。
void OnKeyDown(DWORD vk)
{
    switch (vk) // virtual key value
    {
    case VK_LEFT:
        BlockMoveLeft();
        break;
    case VK_RIGHT:
        BlockMoveRight();
        break;
    case VK_UP:
        BlockRoll();
        break;
    }
    return;
}

// 游戏的初始化，
// 创建游戏的内部数据结构和系统对象。
void CreateGame(HWND hwnd,               // 主窗口句柄
                DWORD dwInitTimerElapse, //
                unsigned int one_level_scores,
                DOUBLE level_speedup_ratio,
                int boundary_x, int boundary_y)
{
    // 设置随机数种子
    // 需要使用随机数生成食物的位置等。
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    srand(ft.dwLowDateTime);

    dbLevelSpeedupRatio = level_speedup_ratio;
    dwTimerElapse = dwInitTimerElapse;
    dwOneLevelScores = one_level_scores;

    // 设置游戏的边界
    SetBoundary(boundary_x, boundary_y);

    // 创建表示食物的数据结构
    NextBlock();

    // 创建一个计时器
    // 每经过 dwTimerElapse 毫秒，hwnd窗口（主窗口）就会收到一个WM_TIMER消息。
    // 计时器是驱动本游戏进行的主要时间线。
    // dwTimerElapse变量影响游戏进行的快慢变化。
    SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);
}