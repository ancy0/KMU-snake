#ifndef __RULES_H_
#define __RULES_H_
#include <vector>

#define LEN 50

class Head { // snake 머리
    int y, x;
public:
    Head();
    void init(); // snake를 화면 중앙에 위치

    int getY() { return y; }
    int getX() { return x; }

    void position(int i, int j); // 방향 입력받아 위치 바꾸기
    void show();
};

class Body { // snake 몸
public:
    int len; int x[LEN] = { 0 }, y[LEN] = { 0 };
    Body();
    void init();

    void position(int i, int j);
    void show();

    void IncBody();
    void DecBody();

    void del();
};

class Snake {
    Head hd; Body bd;
    time_t ti; time_t tg;
    static bool GameOver;
  	bool isGate = false; bool passingGate = false;
    int passtime = 0;
    std::vector<int> state = {3,0,0,0,3};
     int level = 1;
     bool levelUp = false;

public:
    Snake();

    void move();
    void keyInput(int y, int x);

    int control(float s);
    static void fail();
    static void succeed();

    void makeItem();
    void itemRule();

    void makeGate();
    void isPassingGate();

    // 현재 미션 진행률에 따른 값 변화
    void plusBody() { state[0]++; }
    void minusBody() { state[0]--; }
    void plusItem(){ state[1]++; }
    void plusPoison() { state[2]++; }
    void plusGate() { state[3]++; }
    void checkMission();
    void reset();
};

#endif
