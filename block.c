#include "block.h"

// 下降块
BLOCK myblock;
// 沉积块
short segblock[10][16];

GAME_COORD boundary;

//使用blocktype的三维数组决定不同的块在不同的旋转情况下的结构
GAME_COORD blocktype[7][4][4] = {
	{
		{{0, 0}, {0, 1}, {0, 2}, {0, 3}},
		{{0, 0}, {1, 0}, {2, 0}, {3, 0}},
		{{0, 0}, {0, 1}, {0, 2}, {0, 3}},
		{{0, 0}, {1, 0}, {2, 0}, {3, 0}},
	},
	{
		{{0, 0}, {1, 0}, {0, 1}, {0, 2}},
		{{0, 0}, {0, 1}, {1, 1}, {2, 1}},
		{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
		{{0, 0}, {1, 0}, {2, 0}, {2, 1}},
	},
	{
		{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
		{{0, 0}, {0, 1}, {1, 0}, {2, 0}},
		{{0, 0}, {0, 1}, {0, 2}, {1, 2}},
		{{0, 1}, {1, 1}, {2, 1}, {2, 0}},
	},
	{
		{{0, 0}, {1, 0}, {1, 1}, {2, 1}},
		{{1, 0}, {0, 1}, {1, 1}, {0, 2}},
		{{0, 0}, {1, 0}, {1, 1}, {2, 1}},
		{{1, 0}, {0, 1}, {1, 1}, {0, 2}},
	},
	{
		{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
		{{1, 0}, {0, 1}, {1, 1}, {2, 0}},
		{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
		{{1, 0}, {0, 1}, {1, 1}, {2, 0}},
	},
	{
		{{0, 0}, {1, 0}, {2, 0}, {1, 1}},
		{{0, 0}, {0, 1}, {1, 1}, {0, 2}},
		{{0, 1}, {1, 0}, {1, 1}, {2, 1}},
		{{1, 0}, {0, 1}, {1, 1}, {1, 2}},
	},
	{
		{{0, 0}, {1, 0}, {0, 1}, {1, 1}},
		{{0, 0}, {1, 0}, {0, 1}, {1, 1}},
		{{0, 0}, {1, 0}, {0, 1}, {1, 1}},
		{{0, 0}, {1, 0}, {0, 1}, {1, 1}},
	},
};

int CoordEqual(PGAME_COORD one, PGAME_COORD two)
{
	if (one->x == two->x && one->y == two->y)
		return 1;
	return 0;
}

// 设置边界坐标
void SetBoundary(int x, int y)
{
	boundary.x = x;
	boundary.y = y;
}

// 获得边界坐标
PGAME_COORD GetBoundary()
{
	return &boundary;
}

// 生成新块
void NextBlock()
{
	myblock.loc_coord.x = 3;
	myblock.loc_coord.y = 12;
	myblock.bshape = rand()%7;
	myblock.brotate = rand()%4;
}

//获得块的坐标
GAME_COORD GetBlock(int i)
{
	GAME_COORD tempcoord;
	tempcoord.x = myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][i].x;
	tempcoord.y = myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][i].y;
	return tempcoord;
}

int GetSegAt(int col)
{
	for (int i = 12; i >= 0; i--)
	{
		if (segblock[col][i] == 1)
		{
			return i;
		}
	}
	return -1;
}

//能成功下降一格的条件是四个小块下方都还有至少一格的空间；成功左移和右移的条件类似
//成功与否会返回不同的返回值，从而影响ui.c中的程序选择
int BlockMoveDown()
{
	if (segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][0].x][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][0].y - 1] == 0 &&
		segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][1].x][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][1].y - 1] == 0 &&
		segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][2].x][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][2].y - 1] == 0 &&
		segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][3].x][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][3].y - 1] == 0&&
		(myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][0].y) != 0&&
		(myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][1].y) != 0&&
		(myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][2].y) != 0&&
		(myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][3].y) != 0)
	{
		myblock.loc_coord.y--;
		return DOWNSUCCESS;
	}
	return DOWNFAILED;
}

int BlockMoveLeft()
{
	if (segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][0].x - 1][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][0].y] == 0 &&
		segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][1].x - 1][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][1].y] == 0 &&
		segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][2].x - 1][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][2].y] == 0 &&
		segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][3].x - 1][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][3].y] == 0 &&
		(myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][0].x) != 0 &&
		(myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][1].x) != 0 &&
		(myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][2].x) != 0 &&
		(myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][3].x) != 0
		
		)
	{
		myblock.loc_coord.x--;
		return LEFTSUCCESS;
	}
	return LEFTFAILED;
}

int BlockMoveRight()
{
	if (segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][0].x + 1][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][0].y] == 0 &&
		segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][1].x + 1][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][1].y] == 0 &&
		segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][2].x + 1][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][2].y] == 0 &&
		segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][3].x + 1][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][3].y] == 0&&
		(myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][0].x) != 9 &&
		(myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][1].x) != 9 &&
		(myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][2].x) != 9 &&
		(myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][3].x) != 9)
	{
		myblock.loc_coord.x++;
		return RIGHTSUCCESS;
	}
	return RIGHTFAILED;
}

int BlockRoll()
{
	if (segblock[myblock.loc_coord.x + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][0].x][myblock.loc_coord.y + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][0].y] == 0 &&
		segblock[myblock.loc_coord.x + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][1].x][myblock.loc_coord.y + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][1].y] == 0 &&
		segblock[myblock.loc_coord.x + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][2].x][myblock.loc_coord.y + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][2].y] == 0 &&
		segblock[myblock.loc_coord.x + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][3].x][myblock.loc_coord.y + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][3].y] == 0&&
		 - 1<myblock.loc_coord.x + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][0].x&& myblock.loc_coord.x + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][0].x <10 &&( - 1 < myblock.loc_coord.y + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][0].y )&&
		 - 1 < myblock.loc_coord.x + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][1].x&& myblock.loc_coord.x + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][1].x < 10 &&( - 1 < myblock.loc_coord.y + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][1].y) &&
		 - 1 < myblock.loc_coord.x + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][2].x&& myblock.loc_coord.x + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][2].x < 10 && ( - 1 < myblock.loc_coord.y + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][2].y) &&
		 - 1 < myblock.loc_coord.x + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][3].x&& myblock.loc_coord.x + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][3].x < 10 && ( - 1 < myblock.loc_coord.y + blocktype[myblock.bshape][(myblock.brotate + 1) % 4][3].y)
		)
	{
		myblock.brotate = (myblock.brotate + 1) % 4;
		return ROLLSUCCESS;
	}
	return ROLLFAILED;
}

int SegBlockGorwup()
{
	segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][0].x][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][0].y] = 1;
	segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][1].x][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][1].y] = 1;
	segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][2].x][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][2].y] = 1;
	segblock[myblock.loc_coord.x + blocktype[myblock.bshape][myblock.brotate][3].x][myblock.loc_coord.y + blocktype[myblock.bshape][myblock.brotate][3].y] = 1;
	return IsOver();
}

//判断已落地的块中某个坐标是否存在块，在绘制落地块的过程中使用
int isseg(int i, int j) {
	if (segblock[i][j] == 1) {
		return EXIST;
	}
	return NOTEXIST;
}

//判断游戏是否结束
int IsOver()
{
	for (int i = 0; i < 10; i++)
	{
		if (GetSegAt(i) == 12)
		{
			return GAMEOVER;
		}
	}
	return GAMENOTOVER;
}

//判断某行是否已满，满了则需要消去并加一分
int FullRow(int row)
{
	for (int i = 0; i < 10; i++)
	{
		if (segblock[i][row] == 0)
		{
			return NOTFULL;
		}
	}
	return ISFULL;
}

//消去已满的行，返回相应的分数
int RemoveRow()
{
	int countscore = 0;
	for (int i = 0; i < 12; i++)
	{
		if (FullRow(i) == ISFULL)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = i; k < 11; k++)
				{
					segblock[j][k] = segblock[j][k + 1];
				}
				segblock[j][11] = 0;
			}
			countscore++;
			i--;
		}
	}
	return countscore;
}