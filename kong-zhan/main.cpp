#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <graphics.h>
#include <time.h>
#include <cmath>

using namespace std;

const int enemyinterval = 3000;
const int enemymoveinterval = 75;
const int bulletinterval = 200;
const int bulletmoveinterval = 30;
const int score = 25;
const int blood = 5;

void gotoXY(int x, int y) //光标移动到(x,y)位置
{
    COORD pos = { x,y };
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄
    SetConsoleCursorPosition(hOut, pos);//两参数分别是指定哪个窗体，具体位置
}

void clearAt(int x, int y) {
    gotoXY(y - 1, x - 1); printf("   ");
    gotoXY(y - 2, x);printf("     ");
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
    //我方子弹移动方式
    bool move1()
    {
        gotoXY(by, bx); printf(" ");
        bx--;
        if (bx < 0) return 1;
        gotoXY(by, bx); 
        printf("*");
        return 0;
    }
    //敌方子弹移动方式
    bool move2()
    {
        gotoXY(by, bx); printf(" ");
        bx++;
        if (bx > 34) return 1;
        gotoXY(by, bx);
        printf("*");
        return 0;
    }
    
};

struct enemy
{
public:
    enemy(int x, int y):ex(x), ey(y),bullettime(30), next(NULL){}
    int ex;
    int ey;
    int bullettime;
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

//测试我方子弹是否打中敌方飞机
bool hittest1(int bx, int by, int ex, int ey)
{
    if ((ex == bx && ey == by) || (ex - 1 == bx && (ey - 1 == by || ey == by || ey + 1 == by)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//测试我方飞机是否和敌方飞机相撞

bool hittest2(int ex, int ey, int x, int y)
{
    if ((ex == x && abs(ey - y) <= 2) || ((ex == x - 1 || ex == x + 1) && abs(ey - y) <= 1)) return true;
    if ((ex - 1 == x && abs(ey - y) <= 2) || ((ex - 1 == x - 1 || ex - 1 == x + 1) && abs(ey - y) <= 1)) return true;
    if ((ex - 1 == x && abs(ey - 1 - y) <= 2) || ((ex - 1 == x - 1 || ex - 1 == x + 1) && abs(ey - 1 - y) <= 1)) return true;
    if ((ex - 1 == x && abs(ey + 1 - y) <= 2) || ((ex - 1 == x - 1 || ex - 1 == x + 1) && abs(ey + 1 - y) <= 1)) return true;
    return false;
}

//测试敌方子弹是否打中我方飞机
bool hittest3(int bx, int by, int x, int y)
{
    if ((bx == x && abs(by - y) <= 2) || ((bx == x - 1 || bx == x + 1) && abs(by - y) <= 1)) return true;
    else return false;
}


int main() {
    bullet* head = new bullet(-1, -1);
    enemy* head2=new enemy(-1, -1);
    bullet* head3 = new bullet (-1, -1);
    int i = 0, j = 0;
    int x = 25, y = 17;
    int starttime=0, endtime=0;
    int enemySpawnTimer = 0;//生成敌机时间间隔
    int enemymovetime = 0;//移动敌机时间间隔
    int enemybulletmove = 0;//敌机子弹移动时间间隔
    int num = 0;
    int myblood = blood;
    float fps = 1000 / 80.0f;
    char input1 = 'k';
    
    HideCursor();
    input1 = _getch();
    if (input1 == ' ') {

        int prev_x = x, prev_y = y;

        while (1) {

            starttime = clock();
            gotoXY(0, 33);cout << "得分: " << num;
            gotoXY(0, 34);cout << "血条: " << myblood;
            if (myblood == 0)
            {
                system("cls");cout << "游戏失败";
                break;
            }
            if (num == score)
            {
                system("cls");cout << "游戏成功";
                break;
            }
            // 只清除上一次的位置
            clearAt(prev_x, prev_y);

            // 绘制新位置
            gotoXY(y - 1, x - 1); printf("/*\\");
            gotoXY(y - 2, x);     printf("<<*>>");
            gotoXY(y - 1, x + 1); printf("* *");

            // 保存当前位置
            prev_x = x;
            prev_y = y;

            //在随机位置创建一个敌机
            enemySpawnTimer += (endtime + (fps - endtime > 0 ? fps - endtime : 0));
            if (enemySpawnTimer >= enemyinterval)
            {
                int Y = rand() % 53 + 1;
                enemy* ene = new enemy(0, Y);
                ene->next = head2->next;
                head2->next = ene;
                gotoXY(Y, 0);printf("|");
                enemySpawnTimer = 0;
            }
            
            //移动敌机同时判断是否和我方相撞
            enemymovetime += (endtime + (fps - endtime > 0 ? fps - endtime : 0));
            if (enemymovetime >= enemymoveinterval)
            {
                bool hit = false;
                for (enemy* i = head2->next, *k = head2;i != NULL && i->ex >= 0 && i->ey >= 0;)
                {
                    i->bullettime += (endtime + (fps - endtime > 0 ? fps - endtime : 0));
                    if (i->bullettime >= bulletinterval)
                    {
                        bullet* bull = new bullet(i->ex, i->ey);
                        bull->next = head3->next;
                        head3->next = bull;

                        gotoXY(y, x);
                        printf("*");
                        i->bullettime = 0;
                    }

                    //判断是否两方相撞
                    if (hittest2(i->ex, i->ey, prev_x, prev_y))
                    {
                        hit = true;
                        break;
                    }
                    else
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

                }
                if (hit == true)
                {
                    system("cls");break;
                }
                enemymovetime = 0;
            }

            //移动敌方飞机发射的子弹
            enemybulletmove += (endtime + (fps - endtime > 0 ? fps - endtime : 0));
            if (enemybulletmove >= bulletmoveinterval)
            {
                for (bullet* i = head3->next, *k = head3;i != NULL && i->bx >= 0 && i->by >= 0;)
                {
                    if (hittest3(i->bx, i->by, prev_x, prev_y))
                    {
                        bullet* temp = i->next;
                        k->next = i->next;
                        delete i;
                        i = temp;

                        myblood--;
                    }
                    else
                    {
                        if (i->move2())
                        {
                            bullet* temp = i->next;
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

                }
                enemybulletmove = 0;
            }
            

            //读取键盘事件，操控飞机移动
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
                    bullet* bull = new bullet(x, y);
                    bull->next = head->next;
                    head->next = bull;

                    gotoXY(y, x);
                    printf("*");
                }
            }

            //移动我方发射的子弹；
            for (bullet* i = head->next, *k = head;i != NULL && i->bx >= 0 && i->by >= 0;)
            {
                bool hit = false;
                for (enemy* j = head2->next, *h = head2;j != NULL && j->ex >= 0 && j->ey >= 0;)
                {
                    if (hittest1(i->bx, i->by, j->ex, j->ey))
                    {
                        gotoXY((j->ey) - 1, (j->ex) - 1);printf("   ");gotoXY(j->ey, j->ex);printf(" ");
                        enemy* temp1 = j->next;
                        h->next = j->next;
                        delete j;
                        j = temp1;
                        hit = true;
                        break;
                    }
                    else
                    {
                        j = j->next;
                        h = h->next;
                    }
                }
                if (hit)
                {
                    bullet* temp = i->next;
                    k->next = i->next;
                    delete i;
                    i = temp;

                    num++;
                }
                else
                {
                    if (i->move1())
                    {
                        bullet* temp = i->next;
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

            }
            endtime = clock() - starttime;
            //控制帧率
            if ((fps - endtime) > 0)
            {
                Sleep(fps - endtime);
            }
        }
    }
    delete head;
    delete head2;
}