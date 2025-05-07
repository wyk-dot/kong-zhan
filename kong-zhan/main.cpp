#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

using namespace std;

void gotoXY(int x, int y) //光标移动到(x,y)位置
{
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄
	SetConsoleCursorPosition(hOut, pos);//两参数分别是指定哪个窗体，具体位置
}
void HideCursor()//隐藏光标
{
	CONSOLE_CURSOR_INFO cursor_info = { 1,0 };//第二个值为0表示隐藏光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

int main()
{
	int i, j;
	int x = 25, y = 17;
	char input;
	gotoXY(0, 0);

	while (1)
	{
		system("cls");
		HideCursor();
		//for (int i = 0;i <= x;i++) printf("\n");
		//for (int j = 0;j <= y-2;j++) printf(" ");
		gotoXY(y - 2, x - 1);
		printf(" /*\\ ");
		//for (int j = 0;j <= y-2;j++) printf(" ");
		gotoXY(y - 2, x);
		printf("<<*>>");
		//for (int j = 0;j <= y-2;j++) printf(" ");
		gotoXY(y - 2, x + 1);
		printf(" * * ");
		input = _getch();
		if (input == 'a')
		{
			if (y - 3 >= 0) y--;
		}
		if (input == 'w')
		{
			if (x - 2 >= 0) x--;
		}
		if (input == 's')
		{
			if (x + 1 <= 33) x++;
		}
		if (input == 'd')
		{
			if (y + 2 <= 53) y++;
		}
	}
	
}