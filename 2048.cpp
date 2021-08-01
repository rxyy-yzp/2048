#include"stdio.h"
#include"stdlib.h"
#include"graphics.h"
#include"time.h"
#include"math.h"
#include"conio.h"
#include "windows.h"
#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

#define GRID_W 100 //每格子大小
#define MAX_SIZE 4 //每行、列格子数
#define INTERVAL 15 //间距大小（间距个数等于每行、列格子数+1）
#define WIN_SIZE MAX_SIZE * GRID_W + INTERVAL * (MAX_SIZE + 1) //窗口大小计算

IMAGE imgbk, imgwin1, imgwin2, imgmes, imggo;

enum Color
{
	zero = RGB(205, 193, 180),//0
	two = RGB(238, 228, 218),//2
	four = RGB(237, 224, 200),//4
	eight = RGB(242, 177, 121),//8
	sixt = RGB(245, 149, 99),//16
	thirtyt = RGB(246, 124, 95),//32
	sixtyf = RGB(246, 94, 59),//64
	ote = RGB(242, 177, 121),//128
	tfs = RGB(237, 204, 97),//256
	foe = RGB(255, 0, 128),//512
	oztf = RGB(145, 0, 72),//1024
	tzfe = RGB(242, 17, 158),//2048
	back = RGB(187, 173, 160),//背景色
};
Color arr[13] = { zero,two,four,eight,sixt,thirtyt,sixtyf,ote,tfs,foe,oztf,tzfe,back };

int map[MAX_SIZE][MAX_SIZE] = { 0 };//初始化数组

//随机产生2/4
int CreateNum()
{
	srand((unsigned)time(NULL) + clock());
	if (rand() % 6 == 0)
		return 4;
	else
		return 2;
}

//绘制界面
void DrawGame()
{
	BeginBatchDraw();
	int i, j;
	int x, y;//找每个格子左上角坐标
	int tw, th;//每个字符宽与高
	int tx, ty;//中间文字左上角坐标
	int index;
	char str[10] = "";
	setbkcolor(RGB(187, 173, 160));//设置背景色
	cleardevice();
	for (i = 0;i < MAX_SIZE;i++)
	{
		for (j = 0;j < MAX_SIZE;j++)
		{
			x = j * GRID_W + (j + 1) * INTERVAL;
			y = i * GRID_W + (i + 1) * INTERVAL;
			index = (int)(log10((double)map[i][j])/log10(2));//求出数字对应枚举数组中的下标
			COLORREF tcolor = arr[index];//从枚举数组中找出相应颜色变量赋给tcolor
			setfillcolor(tcolor);
			solidroundrect(x, y, x + GRID_W, y + GRID_W, 10, 10);
			if (map[i][j] != 0)
			{
				settextstyle(50, 0, "黑体");
				settextcolor(RGB(119, 110, 101));
				setbkmode(TRANSPARENT);
				sprintf(str, "%d", map[i][j]);
				tw = textwidth(str);
				th = textheight(str);
				tx = (GRID_W - tw) / 2;
				ty = (GRID_W - th) / 2;
				outtextxy(x + tx, y + ty, str);
			}
		}
	}
	EndBatchDraw();
}

//生成数字坐标
void InitGame()
{
	int i, j;
	int r, c;//生成数字坐标
	for (i = 0;i < MAX_SIZE;i++)
		for (j = 0;j < MAX_SIZE;j++)
			map[i][j] = 0;
	for (i = 0;i < 2;)
	{
		r = rand() % MAX_SIZE;
		c = rand() % MAX_SIZE;
		if (map[r][c] == 0)
		{
			map[r][c] = CreateNum();
			i++;
		}
	}
}

void AddNum()
{
	int r, c;//生成数字坐标
	while (1)
	{
		r = rand() % MAX_SIZE;
		c = rand() % MAX_SIZE;
		if (map[r][c] == 0)
		{
			map[r][c] = CreateNum();
			break;
		}
	}
}

//移动格子
int moveUp()
{
	int flag = 0;
	int i, j;
	for (i = 0;i < 4;i++)
	{
		for (j = 0;j < 4;j++)
		{
			int curkey = map[i][j];
			if (curkey != 0)
			{
				int k = i + 1;
				while (k < 4)
				{
					int nextcurkey = map[k][j];
					if (nextcurkey != 0)
					{
						if (curkey == nextcurkey)
						{
							flag = 1;
							map[i][j] += map[k][j];
							map[k][j] = 0;
						}
						k = 4;
						break;
					}
					k++;
				}
			}
		}
	}
	for (i = 0;i < 4 - 1;i++)
	{
		for (j = 0;j < 4;j++)
		{
			int curkey = map[i][j];
			if (curkey == 0)
			{
				int k = i + 1;
				while (k < 4)
				{
					int nextcurkey = map[k][j];
					if (nextcurkey != 0)
					{
						flag = 1;
						map[i][j] = nextcurkey;
						map[k][j] = 0;
						k = 4;
					}
					k++;
				}
			}
		}
	}
	if (flag)
		return 0;
	else
		return 4;
}

int moveDown()
{
	int flag = 0;
	int i, j;
	for (i = 4 - 1;i >= 0;i--)
	{
		for (j = 0;j < 4;j++)
		{
			int curkey = map[i][j];
			if (curkey != 0)
			{
				int k = i - 1;
				while (k >= 0)
				{
					int nextcurkey = map[k][j];
					if (nextcurkey != 0)
					{
						if (map[i][j] == map[k][j])
						{
							flag = 1;
							map[i][j] += map[k][j];
							map[k][j] = 0;
						}
						k = 0;
						break;
					}
					k--;
				}
			}
		}
	}
	for (i = 4 - 1;i >= 0;i--)
	{
		for (j = 0;j < 4;j++)
		{
			int curkey = map[i][j];
			if (curkey == 0)
			{
				int k = i - 1;
				while (k >= 0)
				{
					int nextcurkey = map[k][j];
					if (nextcurkey != 0)
					{
						flag = 1;
						map[i][j] = nextcurkey;
						map[k][j] = 0;
						k = 0;
					}
					k--;
				}
			}
		}
	}
	if (flag)
		return 0;
	else
		return 4;
}

int moveLeft()
{
	int i, j;
	int flag = 0;
	for (i = 0;i < 4;i++)
	{
		for (j = 0;j < 4;j++)
		{
			int curkey = map[i][j];
			int k = j + 1;
			if (curkey != 0)
			{
				while (k < 4)
				{
					int nextcurkey = map[i][k];
					if (nextcurkey != 0)
					{
						if (curkey == nextcurkey)
						{
							flag = 1;
							map[i][j] += map[i][k];
							map[i][k] = 0;
						}
						k = 4;
						break;
					}
					k++;
				}
			}
		}
	}
	for (i = 0;i < 4;i++)
	{
		for (j = 0;j < 4;j++)
		{
			int curkey = map[i][j];
			if (curkey == 0)
			{
				int k = j + 1;
				while (k < 4)
				{
					int nextcurkey = map[i][k];
					if (nextcurkey != 0)
					{
						flag = 1;
						map[i][j] = nextcurkey;
						map[i][k] = 0;
						k = 4;
					}
					k++;
				}
			}
		}
	}
	if (flag)
		return 0;
	else
		return 4;
}

int moveRight()
{
	int i, j;
	int flag = 0;
	for (i = 0;i < 4;i++)
	{
		for (j = 4 - 1;j >= 0;j--)
		{
			int curkey = map[i][j];
			int k = j - 1;
			if (curkey != 0)
			{
				while (k >= 0)
				{
					int nextcurkey = map[i][k];
					if (nextcurkey != 0)
					{
						if (curkey == nextcurkey)
						{
							flag = 1;
							map[i][j] += map[i][k];
							map[i][k] = 0;
						}
						k = -1;
						break;
					}
					k--;
				}
			}
		}
	}
	for (i = 0;i < 4;i++)
	{
		for (j = 4 - 1;j >= 0;j--)
		{
			int curkey = map[i][j];
			if (curkey == 0)
			{
				int k = j - 1;
				while (k >= 0)
				{
					int nextcurkey = map[i][k];
					if (nextcurkey != 0)
					{
						flag = 1;
						map[i][j] = nextcurkey;
						map[i][k] = 0;
						k = -1;
					}
					k--;
				}
			}
		}
	}
	if (flag)
		return 0;
	else
		return 4;
}
	
//按键处理
char keyDeal()
{
	char key = _getch();
	switch (key)
	{
	case 'w':
	case 'W':
	case 72:
		if(moveUp()==0)
			AddNum();
		return '0';
		break;
	case 's':
	case 'S':
	case 80:
		if(moveDown()==0)
			AddNum();
		return '0';
		break;
	case 'a':
	case 'A':
	case 75:
		if(moveLeft()==0)
			AddNum();
		return '0';
		break;
	case 'd':
	case 'D':
	case 77:
		if(moveRight()==0)
			AddNum();
		return '0';
		break;
	case 'q':
	case 'Q':
	case 27:
		return 'q';
		break;
	}
	
}

int ifvictory()
{
	int i, j;
	for (i = 0;i < MAX_SIZE;i++)
	{
		for (j = 0;j < MAX_SIZE;j++)
		{
			if (map[i][j] == 2048)
				return 1;
		}
	}
	return 0;
}

int ifgameover()
{
	int i, j;
	for(i = 0;i < MAX_SIZE;i++)
		for (j = 0;j < MAX_SIZE;j++)
		{
			if (map[i][j] == 0)
				return 0;
		}
	for(i = 1;i <= 2;i++)
		for (j = 0;j < MAX_SIZE;j++)
		{
			if (map[i][j] == map[i - 1][j])
				return 0;
			if (map[i][j] == map[i + 1][j])
				return 0;
		}
	for(j = 1;j <= 2;j++)
		for (i = 0;i < MAX_SIZE;i++)
		{
			if (map[i][j] == map[i][j - 1])
				return 0;
			if (map[i][j] == map[i][j + 1])
				return 0;
		}
	return 1;
}

int main()
{
	MOUSEMSG m;
	mciSendString("open sf.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);
	initgraph(WIN_SIZE, WIN_SIZE);	
	loadimage(&imgbk, "bk.jpg", WIN_SIZE, WIN_SIZE);
	do {
		cleardevice();
		putimage(0, 0, &imgbk);
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 105 && m.x <= 370 && m.y >= 270 && m.y <= 335)
			{
				cleardevice();
				loadimage(&imgmes, "mes.jpg", WIN_SIZE, WIN_SIZE);
				putimage(0, 0, &imgmes);
				system("pause");
			}
		}
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 105 && m.x <= 370 && m.y >= 165 && m.y <= 230)
			{
				cleardevice();
				InitGame();
				while (1)
				{
					DrawGame();
					if (ifvictory() == 1)
					{
						Sleep(500);
						cleardevice();
						loadimage(&imgwin1, "win1.jpg", WIN_SIZE, WIN_SIZE);
						loadimage(&imgwin2, "win2.jpg", WIN_SIZE, WIN_SIZE);
						putimage(0, 0, &imgwin1);
						Sleep(300);
						putimage(0, 0, &imgwin2);
						Sleep(300);
						putimage(0, 0, &imgwin1);
						Sleep(300);
						putimage(0, 0, &imgwin2);
						system("pause");
						break;
					}
					if (ifgameover() == 1)
					{
						Sleep(1000);
						cleardevice();
						loadimage(&imggo, "go.jpg", WIN_SIZE, WIN_SIZE);
						putimage(0, 0, &imggo);
						system("pause");
						break;
					}
					if (keyDeal() == 'q')
						break;
				}
			} 
		}
		
	} while (1);
	closegraph();
	return 0;
}