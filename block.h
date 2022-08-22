#define DOWNSUCCESS 1
#define DOWNFAILED -1
#define LEFTSUCCESS 1
#define LEFTFAILED -1
#define RIGHTSUCCESS 1
#define RIGHTFAILED -1
#define ROLLSUCCESS 1
#define ROLLFAILED -1
#define GAMEOVER -1
#define GAMENOTOVER 1
#define ISFULL 1
#define NOTFULL -1
#define EXIST 1
#define NOTEXIST 0

#include <Windows.h>

// 游戏坐标
typedef struct _GAME_COORD
{
	short x;
	short y;
} GAME_COORD, *PGAME_COORD;


//下降块的数据结构
//我在这里使用了一个固定的坐标和四个相对坐标描述整个块（由四个小块组成）的位置和形状
typedef struct _BLOCK
{
	GAME_COORD loc_coord;				//整个块当前的位置
	int bshape;							//这个块是七种块中的哪一种
	int brotate;						//当前处于哪种旋转角度
} BLOCK, *PBLOCK;

// 函数声明

// block.c中的接口函数

// 判断两个坐标是否相等。

int CoordEqual(PGAME_COORD one, PGAME_COORD two);

// 设置边界坐标
void SetBoundary(int x, int y);

// 获得边界坐标
PGAME_COORD GetBoundary();

//某个块落下后，生成新块
void NextBlock();

//获得块的第i个格子的坐标
GAME_COORD GetBlock(int i);

// 下落一格
// 下落以后，如果触底，则停止，并将对应块加入已沉积块，销毁这个块
int BlockMoveDown();

// 下落至底部
//int BlockMoveDownToBottom();

// 左移一格
int BlockMoveLeft();

// 右移一格
int BlockMoveRight();

// 旋转块
int BlockRoll();


// 落地块生长
int SegBlockGorwup();

//判断给定坐标(i,j)的落地块是否已有块
int isseg(int i, int j);
//判断生长后是否结束
int IsOver();

//判断生长后时候要消去某行
int RemoveRow();

// 按照列获得该列沉积块高度，不能超过总列数，否则返回NULL
int GetSegAt(int col);

/// game_opration.c 中的接口函数

int GetScore();
int GetLevel();
void CreateGame(HWND hwnd,
				DWORD dwInitTimerElapse,
				unsigned int one_level_scores,
				DOUBLE level_speedup_ratio,
				int boundary_x, int boundary_y);
void OnTimer(HWND hwnd);
void OnKeyDown(DWORD vk);