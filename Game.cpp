#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include <vector>


typedef struct CharPos
{
  int xpos;
  int ypos;
  CharPos(int x, int y);
  CharPos();
}CharPos;

WINDOW *gameBoard;
int maxY, maxX, c;
int gWidth, gHeight;
char mapData[21][60];
std::vector<CharPos> wall;

CharPos::CharPos(int y, int x)
{
  xpos = x;
  ypos = y;
}

CharPos::CharPos()
{
  xpos = 0;
  ypos = 0;
}

void InitWindow()
{

    initscr();
    clear();
    start_color();
    keypad(stdscr,TRUE);
    noecho();
    curs_set(0);
    getmaxyx(stdscr,maxY, maxX);

     char mesg[] = "Press <Enter> to play game";
    mvprintw(maxY/2,(maxX-strlen(mesg))/2,"%s",mesg);
    mvchgat(maxY/2,(maxX-strlen(mesg))/2,-1,A_BLINK,0,NULL);
    while(c=getch())
    {
        if(c=='\n')
        {
        clear();
        break;
        }
    }
    getch();
    move(0,0);
    refresh();
}

void InitBoard()
{
    gWidth = 60;
    gHeight = 21;
    gameBoard = newwin(gHeight,gWidth,0,0);
    nodelay(gameBoard,TRUE);
    keypad(gameBoard,TRUE);
}

void DrawMap(int n)
{
    int row=0, col=0;
    std::string mapfile = "./map" + std::to_string(n) + ".txt";
    std::ifstream map(mapfile,std::ios::in);

    init_pair(1,COLOR_WHITE,COLOR_BLACK);
    init_pair(4,COLOR_CYAN,COLOR_MAGENTA);
    init_pair(5,COLOR_GREEN,COLOR_GREEN);
    init_pair(6,COLOR_RED,COLOR_RED);
    while((c=map.get())!=EOF) {
        if(c=='2'){
        mapData[row][col++] = c;
        wattron(gameBoard,COLOR_PAIR(1));
        wprintw(gameBoard,"%c",'X');
        wattroff(gameBoard,COLOR_PAIR(1));
        }
        else if(c=='1'){
        mapData[row][col++] = c;
        wall.push_back(CharPos(row,col));
        wattron(gameBoard,COLOR_PAIR(1));
        wprintw(gameBoard,"%c",'#');
        wattroff(gameBoard,COLOR_PAIR(1));
        }
        else if(c=='0') {
        mapData[row][col++] = c;
        wprintw(gameBoard,"%c",' ');
        }
        else {
        row++; col=0;
        wprintw(gameBoard,"%c",c);
        }
    }
    /*
    mapData[10][10]='3';
    mapData[5][50]='4';
    wattron(gameBoard,COLOR_PAIR(5));
    mvwprintw(gameBoard,10,10,"G");
    wattroff(gameBoard,COLOR_PAIR(5));
    wattron(gameBoard,COLOR_PAIR(6));
    mvwprintw(gameBoard,5,50,"P");
    wattroff(gameBoard,COLOR_PAIR(6));

    mapData[5][0]='5';
    mapData[15][58]='6';
    wattron(gameBoard,COLOR_PAIR(4));
    mvwprintw(gameBoard,5,0,"T");
    mvwprintw(gameBoard,15,58,"T");
    wattroff(gameBoard,COLOR_PAIR(4));
    */
    map.close();
    wrefresh(gameBoard);
    
}

int main()
{
    int n=1, flag;
  
    InitWindow();
    InitBoard();
    DrawMap(n);
}