#include <ncurses.h>
#include <iostream>
#include <cstdlib>
#include "Item.h"
#include "Map.h"

Growth::Growth(Head &head, Body &body) : hd(&head), bd(&body) {
	// snake, 벽, 이미 생성된 아이템 자리 제외하고 랜덤으로 x, y 받아서 아이템 생성
	do {
		srand(time(NULL));
		x = rand()%(WIDTH-2)+1;
		y = rand()%(HEIGHT-2)+1;
		pos = (mvinch(y, x) & A_CHARTEXT);
	}
	while (pos == '1' || pos == '2' || pos == '3' || pos == '4' || pos == '5' || pos == '6' || pos == '7');

	attron(COLOR_PAIR(4));  // item color on
	mvprintw(y, x, "5");
	attroff(COLOR_PAIR(4)); // off
	refresh();

	t = time(NULL);
}

int Growth::GrowthRule() {
	// 생성 후 10초가 지나면 아이템 사라짐
	if (time(NULL) - t > 10) {
		mvprintw(y, x, " ");
		refresh();
		return -1;
	}
	// snake head와 만나면 아이템 사라짐
	if (x == hd->getX() && y == hd->getY()) {
		mvprintw(y, x, " ");

		// snake body 1 증가
		bd->IncBody();
		setItemState(true);
		return -1;
	}
	return 0;
}


Poison::Poison(Head &head, Body &body) : hd(&head), bd(&body) {
	// snake, 벽, 이미 생성된 아이템 자리 제외하고 랜덤으로 x, y 받아서 아이템 생성
	do {
		srand(time(NULL));
		x = rand()%(WIDTH-2)+1;
		y = rand()%(HEIGHT-2)+1;
		pos = (mvinch(y, x) & A_CHARTEXT);
	}
	while (pos == '1' || pos == '2' || pos == '3' || pos == '4' || pos == '5' || pos == '6' || pos == '7');

	attron(COLOR_PAIR(5));  // item color on
	mvprintw(y, x, "6");
	attroff(COLOR_PAIR(5)); // off
	refresh();

	t = time(NULL);
}

int Poison::PoisonRule() {
	// 생성 후 10초가 지나면 아이템 사라짐
	if (time(NULL) - t > 10) {
		mvprintw(y, x, " ");
		refresh();
		return -1;
	}
	// snake head와 만나면 아이템 사라짐
	if (x == hd->getX() && y == hd->getY()) {
		mvprintw(y, x, " ");

		// snake body 1 감소
		bd->DecBody();
		setItemState(true);
		return -1;
	}
	return 0;
}


Item::Item(Growth g){ growp = g; }

Item::Item(Poison p){ poisp = p; }

int Item::rule(){
		if (growp.getX()) { return growp.GrowthRule(); }
		else{ return poisp.PoisonRule(); }
}

void Item::clear(){
		if (growp.getX()) { mvprintw(growp.getY(), growp.getX(), " "); }
		else{ mvprintw(poisp.getY(), poisp.getX(), " "); }
}

int Item::CheckItem(){
	if(growp.getX()){
		if(growp.getItemState()==true) return 1;
	}
	else{
		if(poisp.getItemState()==true) return 2;
	}
	return 0;
}
