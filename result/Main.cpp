#include <ncurses.h>
#include <iostream>
#include "Rules.h"
#include "Item.h"
#include "Gate.h"
#include "Map.h"


int main()
{
	initSet();
	initWindow();
	DrawMap(1);
	Snake sk;
	if(isEnd==true) return 0;
	char a = getch();
	while(a != 'q') {
		if(a == 'r') {
			sk.reset();
			sk.move();
			if(isEnd==true) return 0;
		}
		a = getch();
	}
	endwin();
	return 0;
}
