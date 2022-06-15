#ifndef __ITEM_H_
#define __ITEM_H_
#include <ctime>
#include "Rules.h"

class Plus {
	int x = 0, y = 0;
	Head *hd = nullptr; Body *bd = nullptr;
	char pos;
	time_t t;

	bool ItemState = false; // 해당 아이템을 획득했는지 여부 확인용 변수

public:
	Plus() {}
	Plus(Head &head, Body &body);

	int getX() { return x; }
	int getY() { return y; }

	int PlusRule();

	bool getItem() {
        return ItemState;
    };
	void setItem(bool check) {
        ItemState = check;
    };
};

class Minus {
	int x = 0, y = 0;
	Head *hd = nullptr; Body *bd = nullptr;
	char pos;
	time_t t;

	bool ItemState = false; // 해당 아이템을 획득했는지 여부 확인용 변수
	bool growth = false; // 아이템이 성장 아이템인지(1), 독 아이템인지(0)를 판별용 변수

public:
	Minus() {}
	Minus(Head &head, Body &body);

	int getX() { return x; }
	int getY() { return y; }

	int MinusRule();

	bool getItem() {
        return ItemState;
    };
	void setItem(bool check) {
        ItemState = check;
    };
};

class Item {
	Plus plusing;
	Minus minusing;

public:
	Item(Plus p);
	Item(Minus m);

	int rules();
    void clear();

    int ItemCheck();
};

#endif
