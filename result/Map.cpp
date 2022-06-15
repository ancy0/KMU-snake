
#include <ncurses.h>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include "Map.h"
using namespace std;

// Gate가 생길 수 있는 wall 좌표들을 모아놓은 벡터
vector<array<int, 2>> wall;

bool isEnd = false;

// 점수를 기록하는 window(score, mission)
WINDOW *score = newwin(9, 30, 1, 65);
WINDOW *mission;
// 미션으로 주어지는 수(bdoy length, item, poision, gate)들이 담긴 벡터
vector<int> missions;


void initWindow(){
  mvprintw(10, 30, "Press <Enter> to play game");
  refresh(); // 실제 스크린에 출력
  int key = getch();
  while(key=='\0'){}
  // 시작을 위해 키를 입력 받을때까지 기다리게 하고,
  // 이후에는 원활한 움직임 제어를 위해 getch()의 delay를 없앤다.
  nodelay(stdscr, 1);

}


void gameEnd(){
  delwin(score);
  delwin(mission);
  clear();
  mvprintw(10, 20, "End");
  refresh();
  isEnd = true;
  clock_t delay = 3 * CLOCKS_PER_SEC;
  clock_t start = clock();
  while ((clock() - start) < delay){}
  endwin();
}

// set the game base settings
void initSet(){
  initscr(); // Curses 모드 시작
  curs_set(0); // 화면에 커서 보이지 않게 설정
  noecho();     //  입력한 값을 화면에 출력X
  start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK); // 기본 배경 색 설정
	init_pair(2, COLOR_RED, COLOR_GREEN); // 게이트
	init_pair(3, COLOR_WHITE, COLOR_MAGENTA);  //머리
	init_pair(4, COLOR_RED, COLOR_CYAN); // +아이템
  init_pair(5, COLOR_CYAN, COLOR_RED); // -아이템
  init_pair(7, COLOR_BLACK, COLOR_WHITE); // 점수보드, 미션보드
  init_pair(8, COLOR_MAGENTA, COLOR_BLACK); //몸

	bkgd(COLOR_PAIR(1));
}

void DrawMap(int stage){
  clear();
  wall.clear();
  attron(COLOR_PAIR(1));
  switch(stage){ //맵 속 벽
    case 1:
      for(int i=6; i<14; i++){
        mvprintw(i, 10, "#");
        mvprintw(i, 49, "#");
      }
      missions = {6, 2, 1, 1};
      break;
    case 2:
      for(int i=6; i<18; i+=4){
        for(int j=6; j<16; j++){
          mvprintw(i, j, "#");
        }
        for(int j=44; j<54; j++){
          mvprintw(i, j, "#");
        }
      }
      for(int i=7; i<10; i++){
        mvprintw(i, 15, "#");
        mvprintw(i, 53, "#");
      }
      for(int i=11; i<14; i++){
        mvprintw(i, 6, "#");
        mvprintw(i, 44, "#");
      }
      missions = {8, 3, 2, 2};
      break;

    case 3:
      for(int i=6; i<18; i+=4){
        for(int j=6; j<16; j++){
          mvprintw(i, j, "#");
        }
        for(int j=44; j<54; j++){
          mvprintw(i, j, "#");
        }
      }
      for(int i=7; i<14; i++){
        mvprintw(i, 15, "#");
        mvprintw(i, 53, "#");
      }
      missions = {9, 4, 3, 3};
      break;

    case 4:
      for(int i=6; i<16; i++){
          mvprintw(11, i, "#");
      }
      for(int i=44; i<54; i++){
          mvprintw(11, i, "#");
      }
      for(int i=6; i<11; i++){
        mvprintw(i, 6, "#");
        mvprintw(i, 13, "#");
        mvprintw(i, 44, "#");
        mvprintw(i, 51, "#");
      }
      mvprintw(11, 13, "+");
      mvprintw(11, 51, "+");
      for(int i=12; i<14; i++){
        mvprintw(i, 13, "#");
        mvprintw(i, 51, "#");
      }
      missions = {10, 5, 5, 5};
      break;
  }
    attroff(COLOR_PAIR(1));
    // 게임보드 
    attron(COLOR_PAIR(1));  
    for(int i=0; i<HEIGHT; i++){
      mvprintw(i, 0, "#");
      mvprintw(i, WIDTH-1, "#");
    }
    for(int i=0; i<WIDTH; i++){
      if((i==0)||(i==(WIDTH-1))){
        mvprintw(0, i, "+");
        mvprintw(HEIGHT-1, i, "+");
        continue;
      }
      mvprintw(0, i, "#");
      mvprintw(HEIGHT-1, i, "#");
    }

  	attroff(COLOR_PAIR(1)); 
    // Gate생성 가능 wall 좌표
    for(int i = 0; i < HEIGHT; i++) {
      for(int j = 0; j < WIDTH; j++)
        if((mvinch(i, j) & A_CHARTEXT) == '#') {
          array<int, 2> a = {i, j};
          wall.emplace_back(a);
        }
    }
    refresh();
    DrawBoard();
    UpdateBoard(mission, missions);
}

// 보드 그리기
void DrawBoard(){
  //점수보드
  score = newwin(9, 30, 1, 65);
  wbkgd(score, COLOR_PAIR(7));
  mvwprintw(score, 3, 6, "/   (Max Length)");
  makeFrame(score, "Score Board");
  // 미션보드
  mission = newwin(9, 30, 10, 65);
  wbkgd(mission, COLOR_PAIR(7));
  for(int i=3; i<7; i++){
    mvwprintw(mission, i, 7, "( )");
  }
  makeFrame(mission, "Mission");

}

// 미션, 점수 보드 생성.
void makeFrame(WINDOW *win, const char *ch){
  wborder(win, '|','|','-','-','*','*','*','*');
  mvwprintw(win, 1, 1, ch);
  mvwprintw(win, 3, 1, "B:");
  mvwprintw(win, 4, 1, "+:");
  mvwprintw(win, 5, 1, "-:");
  mvwprintw(win, 6, 1, "G:");
  wrefresh(win);
}

//  (보드 상태 업데이트 용)
void UpdateBoard(WINDOW *board, const vector<int> &v){
    mvwprintw(board, 3, 5, " ");
    const char* body = to_string(v[0]).c_str();
    mvwprintw(board, 3, 4, body);
    const char* plus = to_string(v[1]).c_str();
    mvwprintw(board, 4, 4, plus);
    const char* minus = to_string(v[2]).c_str();
    mvwprintw(board, 5, 4, minus);
    const char* gate = to_string(v[3]).c_str();
    mvwprintw(board, 6, 4, gate);
    // state 벡터, 진행 중 최대 길이도 출력해야함
    if(v.size()>4){
      const char* Maxbody = to_string(v[4]).c_str();
      mvwprintw(board, 3, 8, Maxbody);
    }

    wrefresh(board);

}
