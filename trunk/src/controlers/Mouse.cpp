#include "controlers/Mouse.h"
#include <SDL/SDL.h>
#include "base.h"
#include "modules/script.h"

#define BUTTON(n)	(1 << (n-1))

cMouse::cMouse(){
	position.x = 0;
	position.y = 0;
	rel_pos.x = 0;
	rel_pos.y = 0;
	buttons = 0;
	_buttons = 0;
	_position.x = 0;
	_position.y = 0;
	setName("cMouse");
	dprintf("Mouse handler installed\n");
}

cMouse::~cMouse(){
	dprintf("Mouse handler uninstalled\n");
}

void cMouse::setPos(int x, int y){
	position.set(x,y);
}

void cMouse::setRelPos(int x, int y){
	rel_pos.set(x,y);
}

void cMouse::setButtons(unsigned int state){
	/*if (state!=buttons)*/ _buttons = buttons;
	buttons = state;
}

void cMouse::update(){
	int state = SDL_GetMouseState(&position.x,&position.y);
	/*if (state!=buttons)*/ _buttons = buttons;
	buttons = state;
	SDL_GetRelativeMouseState(&rel_pos.x,&rel_pos.y);
	_position.x += rel_pos.x;
	_position.y += rel_pos.y;
}

qPointi cMouse::getPos(){
	return position;
}

qPointi cMouse::getRelPos(){
	return rel_pos;
}

qPointi cMouse::getPos2(){
	return _position;
}

unsigned char cMouse::getButtons(){
	return buttons;
}

unsigned char cMouse::getPrevButtons(){
	return _buttons;
}

void cMouse::setButton( unsigned int button, unsigned char state ){
	_buttons = buttons;
	if (state)
		buttons |= BUTTON(button);
	else
		buttons &= ~BUTTON(button);
	//printf("button %08X -> %08X\n",button,buttons);
	__SCRIPT->l_setMouseButton(buttons,_buttons);
}

void cMouse::synchronizePosition(){
	__SCRIPT->l_setMousePos(position.x,position.y,rel_pos.x,rel_pos.y);
}