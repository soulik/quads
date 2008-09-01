#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <malloc.h>
#include <stdio.h>
#include "errors/exception.h"
#include "qDebug.h"
#include "common.h"
#include "game/gamelogic.h"
#define MAX_KEYS_NUM	512

//class cGamelogic;

class cKeyboard: public qDebug{
private:
	int * keys;
	ulong max_num;
	friend void cGamelogic::synchronizeData();
public:
	cKeyboard(int max_num);
	virtual ~cKeyboard();
	void SetKey(uint key, int state);
	int GetKey(uint key);
	unsigned int GetMod();
};

#endif /*KEYBOARD_H_*/
