#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <cstring>
#include <vector>
#include <chrono>

#include "Snake.cpp"

using namespace std;

typedef struct CharPos {
    int xpos;
    int ypos;
    CharPos(int x, int y);
    CharPos();
}CahrPos;

WINDOW *gameBoard;
int maxX, maxY, c;
int gWidth, gHeight;
char mapData[21][60];
vector<CharPos> wall;

/* 아이템용
int iter;
CharPos growth_item;
CharPos poison_item;
*/

void DrawBoard();
// score와 mission을 나타내는 창의 공통된 내부 양식을 만든다.
void makeFrame(WINDOW *win, const char *ch);

// 점수를 기록하는 window(score, mission)
WINDOW *score = newwin(9, 30, 1, 65);
WINDOW *mission;
// 미션으로 주어지는 수(bdoy length, item, poision, gate)들이 담긴 벡터
vector<int> missions;



CharPos::CharPos(int y, int x) {
    xpos = x;
    ypos = y;
}

CharPos::CharPos() {
    xpos = 0;
    ypos = 0;
}

void InitWindow() {
    initscr(); // Curses 모드 시작
    clear(); // 화면 지우기
    start_color();
    keypad(stdscr, TRUE);
    noecho(); //  입력한 값을 화면에 출력X
    curs_set(0); // 화면에 커서 보이지 않게 설정
    getmaxyx(stdscr, maxY, maxX); // 화면 가로, 세로 크기 설정(표준화면이므로 window pointer에 stdscr)

    char message[] = "Press <Enter> to play game";
    mvprintw(maxY/2, (maxX-strlen(message))/2, "%s", message); // 이동한 뒤 결과 출력
    mvchgat(maxY/2, (maxX-strlen(message))/2, -1, A_BLINK, 0, NULL); // 해당 위치부터 blinking
    
    while (c = getch()) {
        if (c == '\n') {
            clear();
            break;
        }
    }
    //getch();
    move(0, 0);
    refresh(); // 실제 스크린에 출력
}

void InitBoard() {
    gWidth = 60;
    gHeight = 21;
    gameBoard = newwin(gHeight, gWidth, 0, 0); // 새로운 윈도우 생성(사이즈, 위치)
    nodelay(gameBoard, TRUE);
    keypad(gameBoard, TRUE);
}

void DrawMap(int n) {
    int row = 0, col = 0;
    string mapFile = "Map.txt";
    ifstream map(mapFile, ios::in);

    init_pair(1, COLOR_WHITE, COLOR_BLACK); // 기본 배경 색 설정

    while ((c = map.get()) != EOF) {
        if (c == '2') {
            mapData[row][col++] = c;
            wattron(gameBoard, COLOR_PAIR(1)); // 특정 윈도우에 속성 적용
            wprintw(gameBoard, "%c", 'X');
            wattroff(gameBoard, COLOR_PAIR(1));
        }
        else if (c == '1') {
            mapData[row][col++] = c;
            wall.push_back(CharPos(row, col));
            wattron(gameBoard, COLOR_PAIR(1));
            wprintw(gameBoard, "%c", '#');
            wattroff(gameBoard, COLOR_PAIR(1));
        }
        else if (c == '0') {
            mapData[row][col++] = c;
            wprintw(gameBoard, "%c", ' ');
        }
        else {
            row++; col = 0;
            wprintw(gameBoard, "%c", c);
        }
    }
    map.close();
    
    wrefresh(gameBoard); // 특정 윈도우 새로고침
    DrawBoard();
}

// Draw Score & Mission board
void DrawBoard(){
  //score board
  score = newwin(9, 30, 1, 65);
  wbkgd(score, COLOR_PAIR(7));
  mvwprintw(score, 3, 6, "/   (Max Body Length)");
  makeFrame(score, "Score Board");
  // mission board
  mission = newwin(9, 30, 10, 65);
  wbkgd(mission, COLOR_PAIR(6));
  for(int i=3; i<7; i++){
    mvwprintw(mission, i, 7, "( )");
  }
  makeFrame(mission, "Mission");

}

// board(Score & Mission) Frame 생성.
void makeFrame(WINDOW *win, const char *ch){
  wborder(win, '|','|','-','-','*','*','*','*');
  mvwprintw(win, 1, 1, ch);
  mvwprintw(win, 3, 1, "B:");
  mvwprintw(win, 4, 1, "+:");
  mvwprintw(win, 5, 1, "-:");
  mvwprintw(win, 6, 1, "G:");
  wrefresh(win);
}

int main() {
    int n = 1;

    InitWindow();
    InitBoard();
    DrawMap(n);
    

    Snake s;
    s.move();
    refresh();

    return 0;
}
