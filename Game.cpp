#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <cstring>
#include <vector>
#include <chrono>

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
    clear();
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    getmaxyx(stdscr, maxY, maxX);

    char message[] = "Press <Enter> to play game";
    mvprintw(maxY/2, (maxX-strlen(message))/2, "%s", message);
    mvchgat(maxY/2, (maxX-strlen(message))/2, -1, A_BLINK, 0, NULL);
    
    while (c = getch()) {
        if (c == '\n') {
            clear();
            break;
        }
    }
    getch();
    move(0, 0);
    refresh(); // 실제 스크린에 출력
}

void InitBoard() {
    gWidth = 60;
    gHeight = 21;
    gameBoard = newwin(gHeight, gWidth, 0, 0);
    nodelay(gameBoard, TRUE);
    keypad(gameBoard, TRUE);
}

void DrawMap(int n) {
    int row = 0, col = 0;
    string mapFile = "map.txt";
    ifstream map(mapFile, ios::in);

    init_pair(1, COLOR_WHITE, COLOR_BLACK); // 기본 배경 색 설정

    while ((c = map.get()) != EOF) {
        if (c == '2') {
            mapData[row][col++] = c;
            wattron(gameBoard, COLOR_PAIR(1));
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
    wrefresh(gameBoard);
}

int main() {
    int n = 1;

    InitWindow();
    InitBoard();
    DrawMap(n);

    return 0;
}
