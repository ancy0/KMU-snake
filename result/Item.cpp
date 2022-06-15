#include <ncurses.h>
#include <iostream>
#include <cstdlib>
#include "Item.h"
#include "Map.h"

Plus::Plus(Head &head, Body &body) : hd(&head), bd(&body) { // 랜덤으로 x, y 받아서 아이템 생성 (map의 벽, snake의 위치, 이미 생성된 아이템의 자리 제외)
	do {
		srand(time(NULL));
		x = rand()%(WIDTH-2)+1;
		y = rand()%(HEIGHT-2)+1;
		pos = (mvinch(y, x) & A_CHARTEXT);
	}
	while (pos == '1' || pos == '2' || pos == '3' || pos == '4' || pos == '5' || pos == '6' || pos == '7');

	attron(COLOR_PAIR(4));
	mvprintw(y, x, "+");
	attroff(COLOR_PAIR(4));
	refresh();

	t = time(NULL);
}

int Plus::PlusRule() {
	if (time(NULL) - t > 10) { // 생성된 아이템은 10초 후 제거
		mvprintw(y, x, " ");
		refresh();
		return -1;
	}
	if (x == hd->getX() && y == hd->getY()) { // snake의 머리와 아이템이 만나면 아이템 제거
		mvprintw(y, x, " ");

		bd->IncBody(); // snake 길이 증가
		setItem(true);
		return -1;
	}
	return 0;
}

Minus::Minus(Head &head, Body &body) : hd(&head), bd(&body) { // 랜덤으로 x, y 받아서 아이템 생성 (map의 벽, snake의 위치, 이미 생성된 아이템의 자리 제외)
	do {
		srand(time(NULL));
		x = rand()%(WIDTH-2)+1;
		y = rand()%(HEIGHT-2)+1;
		pos = (mvinch(y, x) & A_CHARTEXT);
	}
	while (pos == '1' || pos == '2' || pos == '3' || pos == '4' || pos == '5' || pos == '6' || pos == '7');

	attron(COLOR_PAIR(5));
	mvprintw(y, x, "-");
	attroff(COLOR_PAIR(5));
	refresh();

	t = time(NULL);
}

int Minus::MinusRule() {
	if (time(NULL) - t > 10) { // 생성된 아이템은 10초 후 제거
		mvprintw(y, x, " ");
		refresh();
		return -1;
	}
	if (x == hd->getX() && y == hd->getY()) { // snake의 머리와 아이템이 만나면 아이템 제거
		mvprintw(y, x, " ");

		bd->DecBody(); // snake 길이 감소
		setItem(true);
		return -1;
	}
	return 0;
}

Item::Item(Plus p){ plusing = p; }
Item::Item(Minus m){ minusing = m; }

int Item::rules(){
	if (plusing.getX()) { return plusing.PlusRule(); }
	else{ return minusing.MinusRule(); }
}

void Item::clear(){
	if (plusing.getX()) { mvprintw(plusing.getY(), plusing.getX(), " "); }
	else{ mvprintw(minusing.getY(), minusing.getX(), " "); }
}

int Item::ItemCheck(){
    // 아이템을 얻지 못한 경우(0), 성장 아이템 획득(1), 독 아이템 획득(2) 출력
	// Plus와 Minus 클래스 결과값을 외부로 전달하기 위한 함수
	if(plusing.getX()) {
		if(plusing.getItem()==true) return 1;
	}
	else {
		if(plusing.getItem()==true) return 2;
	}
	return 0;
}
