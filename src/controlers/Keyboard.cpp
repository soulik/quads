#include "controlers/keyboard.h"
#include <SDL/SDL.h>
#include "base.h"
#include "modules/script.h"

cKeyboard::cKeyboard(int max_num){
	this->max_num = max_num;
	keys = (int*) calloc(max_num,sizeof(int));
	setName("cKeyboard");
	dprintf("Keyboard handler installed\n");
}

cKeyboard::~cKeyboard(){
	free(keys);
	keys = NULL;
	dprintf("Keyboard handler uninstalled\n");
}

void cKeyboard::SetKey(uint key, int state){
	try{
		if (key>=max_num)
			throw cException("key code exceded max_num\n");
		keys[key] = state;
		__SCRIPT->l_setKey(key,state);
	}DEFAULT_EXCEPTION(STREAM_STDDBG)
}

int cKeyboard::GetKey(uint key){
	try{
		if (key>=max_num)
			throw cException("key code exceded max_num\n");
		return keys[key];
	}DEFAULT_EXCEPTION(STREAM_STDDBG)
	return -1;
}

unsigned int cKeyboard::GetMod(){
	return SDL_GetModState();
}
