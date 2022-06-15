
#ifndef __MAP_H_
#define __MAP_H_
#include <array>
#include <vector>

using namespace std;

#define HEIGHT 20
#define WIDTH 60

// 최종단계까지 진행 시, 외부 전달 인자.
extern bool isEnd;
// 점수 기록 window(scoreboard, mission)
extern WINDOW *score;
extern WINDOW *mission;
// 미션 담은 벡터
extern vector<int> missions;


// 첫화면, 엔터키(상관없음) 입력 시 시작
void initWindow();
// 최종 창, 자동 종료
void theEnd();
// 기본 세팅
void initSet();
// 맵 업데이트
void DrawMap(int stage=1);
// 점수 미션 보드 생성
void DrawBoard();
// 점수 미션 보드 공통 양식
void makeFrame(WINDOW *win, const char *ch);
// 보드 업데이트
void UpdateBoard(WINDOW *board, const vector<int> &v);
#endif
