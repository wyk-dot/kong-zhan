#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <graphics.h>
#include <time.h>

using namespace std;

const int enemyinterval = 1500;
const int enemymoveinterval = 50;

void gotoXY(int x, int y) //光标移动到(x,y)位置
{
    COORD pos = { x,y };
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄
    SetConsoleCursorPosition(hOut, pos);//两参数分别是指定哪个窗体，具体位置
}

void clearAt(int x, int y) {
    gotoXY(y - 1, x - 1); printf("   ");
    gotoXY(y - 2, x);   printf("     ");
    gotoXY(y - 1, x + 1); printf("   ");
}
void HideCursor()
{
    CONSOLE_CURSOR_INFO cursor_info = { 1,0 };//第二个值为0表示隐藏光标
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

struct bullet
{
public:
    bullet(int x, int y) :bx(x), by(y), next(NULL){}
    bullet():bx(-1), by(-1), next(NULL){}
    int bx;
    int by;
    void setxy(int x, int y)
    {
        bx = x;by = y;
    }
    bullet* next;
    bool move()
    {
        gotoXY(by, bx); printf(" ");
        bx--;
        if (bx < 0) return 1;
        gotoXY(by, bx); 
        printf("*");
        return 0;
    }
    
};

struct enemy
{
public:
    enemy(int x, int y):ex(x), ey(y), next(NULL){}
    int ex;
    int ey;
    enemy* next;
    bool move()
    {
        if (ex == 0)
        {
            gotoXY(ey, ex);printf(" ");
            ex++;
            gotoXY(ey - 1, ex - 1);printf("\\*/");gotoXY(ey, ex);printf("|");
            return 0;
        }
        else
        {
            if (ex > 0 && ex <= 33)
            {
                gotoXY(ey - 1, ex-1);printf("   ");gotoXY(ey, ex);printf(" ");
                ex++;
                gotoXY(ey - 1, ex-1);printf("\\*/");gotoXY(ey, ex);printf("|");
                return 0;
            }
            else
            {
                if (ex == 34)
                {
                    gotoXY(ey - 1, ex-1);printf("   ");gotoXY(ey, ex);printf(" ");
                    ex++;
                    gotoXY(ey - 1, ex - 1);printf("\\*/");
                }
                else return 1;
            }
            
        }
        
        
    }
};

int main() {
    bullet* head = new bullet(-1, -1);
    enemy* head2=new enemy(-1, -1);
    int i = 0, j = 0;
    int x = 25, y = 17;
    int starttime=0, endtime=0;
    int enemySpawnTimer = 0;
    int enemymovetime = 0;
    float fps = 1000 / 60.0f;
    char input1 = 'k';
    
    HideCursor();

    int prev_x = x, prev_y = y;

    while (1) {

        starttime = clock();
        // 只清除上一次的位置
        clearAt(prev_x, prev_y);

        // 绘制新位置
        gotoXY(y - 1, x - 1); printf("/*\\");
        gotoXY(y - 2, x);     printf("<<*>>");
        gotoXY(y - 1, x + 1); printf("* *");

        // 保存当前位置
        prev_x = x;
        prev_y = y;

        enemySpawnTimer += (endtime + (fps - endtime > 0 ? fps - endtime : 0));
        if (enemySpawnTimer >= enemyinterval)
        {
            int Y = rand() % 54 + 1;
            enemy* ene = new enemy(0, Y);
            ene->next = head2->next;
            head2->next = ene;
            gotoXY(Y, 0);printf("|");
            enemySpawnTimer = 0;
        }

        enemymovetime += (endtime + (fps - endtime > 0 ? fps - endtime : 0));
        if (enemymovetime >= enemymoveinterval)
        {
            for (enemy* i = head2->next, *k = head2;i != NULL && i->ex >= 0 && i->ey >= 0;)
            {
                if (i->move())
                {
                    enemy* temp = i->next;
                    gotoXY((i->ey) - 1, (i->ex) - 1);printf("   ");
                    k->next = i->next;
                    i = i->next;
                    delete i;
                    i = temp;
                }
                else
                {
                    i = i->next;
                    k = k->next;
                }
            }
            enemymovetime = 0;
        }
        

        if (_kbhit())
        {
            input1 = _getch();
            if (input1 == 'a')
            {
                if (y - 3 >= 0) y--;
             }
            if (input1 == 'w')
            {
                if (x - 2 >= 0) x--;
            }
            if (input1 == 's')
            {
                if (x + 1 <= 33) x++;
            }
            if (input1 == 'd')
            {
                if (y + 2 <= 53) y++;
            }
            if (input1 == ' ')
            {
                bullet* bull=new bullet(x, y);
                bull->next = head->next;
                head->next = bull;
            
                gotoXY(y, x);
                printf("*");
            }
        }
        
        for (bullet*i=head->next, * k=head;i != NULL && i->bx >= 0 && i->by >= 0;)
        {
            if (i->move())
            {
                bullet* temp=i->next;
                k->next = i->next;
                delete i;
                i = temp;
            }
            else
            {
                i = i->next;
                k = k->next;
            }
        }
        endtime = clock() - starttime;
        if ((fps-endtime) > 0)
        {
            Sleep(fps - endtime);
        }
    }
    delete head;
    delete head2;
}