#include <ncurses.h>
#include <cstdlib>
#include "Rules.h"
#include "Item.h"
#include "Gate.h"
#include "Map.h"

vector<Item> item;
Gate gate;

Head::Head() {
    init();
}

void Head::init() {
    y = HEIGHT/2; x = WIDTH/2;
    show();
}

void Head::position(int i, int j) { y += i, x += j; }

void Head::show() {
    attron(COLOR_PAIR(3));
    mvprintw(y, x, "H");
    attroff(COLOR_PAIR(3));
}

Body::Body() {
    init();
}

void Body::init() {
    len = 2; std::fill_n(x, LEN, 0); std::fill_n(y, LEN, 0);
    y[0] = y[1] = HEIGHT/2; x[0] = WIDTH/2+1; x[1] = x[0]+1;
    show();
}

void Body::position(int i, int j) { // 몸은 머리에 종속적 -> 머리의 위치를 인자로 받아 첫번째 몸을 머리의 위치로 이동, 나머지 몸은 앞의 몸 위치로 이동
    del();
    int by, bx, py, px; // 이전 위치와 현재 위치
    by = y[0]; bx = x[0];
    y[0] = i; x[0] = j;

    for (int l = 1; l < len; l++) {
        py = y[l]; px = x[l];
        y[l] = by; x[l] = bx;
        by = py; bx = px;
    }
}

void Body::show() {
    attron(COLOR_PAIR(8));
    for (int i = 0; i < len; i++) {
        mvprintw(y[i], x[i], "B");
    }
    attroff(COLOR_PAIR(8));
}

void Body::IncBody() { // 몸 길이 증가
    y[len] = 2 * y[len - 1] - y[len - 2];
    x[len] = 2 * x[len - 1] - x[len - 2];
    len++;
}

void Body::DecBody() { // 몸 길이 감소
    mvprintw(y[len - 1], x[len - 1], " ");
    y[len - 1] = 0; x[len - 1] = 0; len--;

    if (len < 2) { Snake::fail(); } // snake 길이가 2 이하이면 실패
}

void Body::del() { // 지나간 몸 삭제
    mvprintw(y[len - 1], x[len - 1], " ");
}

bool Snake::GameOver = false;

Snake::Snake(){
  UpdateBoard(score, state);
  ti = time(NULL);
  tg = time(NULL);
  move();
}

void Snake::move() {
  while (!GameOver) {
      makeItem(); // 3초씩 아이템 생성

      if(state[0]>state[4]) UpdateBoard(score, state);

      if(passtime == 1) { // 게이트 통과시 알맞은 방향으로 진행
      	keyInput(gate.key[0], gate.key[1]);
        control(0.4);
        continue;
      }

      makeGate(); // 게이트가 존재하지 않으면 게이트 생성

      if(hd.getY()==bd.y[0]) { // 키보드 입력이 없으면 머리와 제일 가까운 몸 반대 방향으로 이동
        if(hd.getX()>bd.x[0]) {
          keyInput(0,1);
          control(0.4);
        }
        else {
          keyInput(0,-1);
          control(0.4);
        }
      }
      else {
        if(hd.getY()>bd.y[0]) {
          keyInput(1,0);
          control(0.4);
        }
        else {
          keyInput(-1,0);
          control(0.4);
        }
      }

      if(levelUp){
        if(level==4) { // 4단계 모두 통과
          gameEnd();
          return;
        }
        level++;
        succeed();
        reset();
      }

    }
 level = 1;
}

void Snake::keyInput(int y, int x) {
    for (int i = 0; i < bd.len; i++) { // 진행방향과 반대로 이동하거나 body에 부딪히면 fail
        if (((hd.getY() + y) == bd.y[i]) && ((hd.getX() + x) == bd.x[i])) {
            fail(); return;
        }
    }

    if ((mvinch(hd.getY() + y, hd.getX() + x) & A_CHARTEXT) == '1' || hd.getY() + y < 0 || hd.getY() + y > HEIGHT-1 || hd.getX() + x < 0 || hd.getX() + x > WIDTH-1) { // 벽에 닿으면 fail
      beep(); fail(); return;
      }

    bd.position(hd.getY(), hd.getX());
    hd.position(y, x);

    itemRule();
    isPassingGate();
    if(passingGate){ passtime++; }

    if(passtime >= bd.len+1) { // 몸까지 모두 gate를 지나면 gate 초기화
        plusGate();
        gate.clear();
        passingGate = false;
        isGate = false;
        passtime = 0;
        tg = time(NULL);
        checkMission();
    }
    if(levelUp) {
      if(level==4) { // 4단계 모두 통과시
        gameEnd();
        return;
      }
      level++;
      succeed();
      reset();
    }

    hd.show(); bd.show();
    refresh();
}

int Snake::control(float s) { // 키 입력이 들어오면 해당 방향으로 움직임 제어
    clock_t control = s * CLOCKS_PER_SEC;
    clock_t start = clock();
    while ((clock() - start) < control){
      makeItem();
      if(GameOver == true) break;
      char keyIn;
      if(keyIn = getch()){
        switch(keyIn){
          case 'w':
            keyInput(-1,0);
            start = clock();
            break;
          case 's':
            keyInput(1,0);
            start = clock();
            break;
          case 'd':
            keyInput(0,1);
            start = clock();
            break;
          case 'a':
            keyInput(0,-1);
            start = clock();
            break;
      }
      keyIn = '\0';
    }
  }
    return 0;
}

void Snake::fail() {
    beep();
    mvprintw(HEIGHT-3, WIDTH/2-2, "Fail");
    attron(A_BLINK);
    mvprintw(HEIGHT-2, WIDTH-20, "R(restart)/Q(quit)");
    attroff(A_BLINK);

    refresh();
    GameOver = true;
}

void Snake::succeed() {
    beep();
    mvprintw(HEIGHT-3, WIDTH/2-2, "Success!");
    mvprintw(HEIGHT-2, WIDTH/2-9, "Next Stage Loading");
    attron(A_BLINK);
    mvprintw(HEIGHT-2, WIDTH-15, "N(next level)");
    attroff(A_BLINK);
    refresh();
    char k = getch();
    while(k !='n'){ k= getch(); }
    nodelay(stdscr, 1);

    GameOver = false;
}

void Snake::makeItem() {
    if(time(NULL) - ti > 3) {

      if (item.size() < 3) {
          srand(time(NULL));
          if (rand() % 2) {
            Plus p(hd, bd);
            Item i(p);
            item.emplace_back(i);
          }
          else {
            Minus m(hd, bd);
            Item i(m);
            item.emplace_back(i);
          }
      }
    ti = time(NULL);
  }
}

void Snake::itemRule(){
      for(int i = item.size() - 1; i >= 0; i--) {
        if(item[i].rules() == -1){
          if(item[i].ItemCheck() == 1) {
            plusBody(); plusItem();
            checkMission();
          }
          else if(item[i].ItemCheck() == 2) {
            minusBody(); plusPoison();
            checkMission();
          }
          item.erase(item.begin() + i);
        }
      }
}

void Snake::makeGate() {
  if(time(NULL) - tg > 10) {
      if (isGate) return;
      gate = Gate(hd, bd);
      isGate = true;
      tg = time(NULL);
  }
}

void Snake::isPassingGate() {
    if(!isGate || passtime) return;

    int n = gate.headCase();
    if(n == 1) {
      isGate = false;
      gate.clear();
      tg = time(NULL);
      return;
    }
    if(n == -1) {
      passingGate = true;
      return;
    }
}

void Snake::checkMission(){ // 미션 통과시 체크 표시
  if(state[0] > state[4]) state[4] = state[0];
  UpdateBoard(score, state);
  int cnt = 0;
  if(state[4] >= missions[0]){
    mvwprintw(mission, 3, 8, "V");
    cnt++;
  }
  for(int i=1; i<4; i++){
    if(state[i] >= missions[i]) {
      mvwprintw(mission, 3 + i, 8, "V");
      cnt++;
    }
  }
  wrefresh(mission);
  if(cnt == 4) {
    levelUp = true;
  }
}

void Snake::reset(){
  for(int i = item.size() - 1; i >= 0; i--) {
    item[i].clear();
    item.erase(item.begin() + i);
  }
  gate.clear();
  isGate = false;
  passingGate = false;
  passtime = 0;

  // 스크린 초기화
  DrawMap(level);
  state = { 3, 0, 0, 0, 3 };
  UpdateBoard(score, state);
  hd.init(); bd.init();

  GameOver = false;
  levelUp = false;
  ti = time(NULL);
  tg = time(NULL);
}
