#include "gui/layers/qScene.h"
#include "gui/layers/qHMap.h"
#include "common.h"
#include "base.h"
#include "modules/game.h"
#include "modules/renderer.h"
#include <math.h>

qScene::qScene(){
	heightMap = new qHMap();
}

qScene::~qScene(){
	delete heightMap;
}

void qScene::setParent(cBase * parent){
	this->parent = parent;
	if (heightMap){
		heightMap->setParent(parent);
	}
}

int qScene::draw(){
	try{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		qVector ppos = GAME->glogic->player->getPosition();
		SCRIPT->call("scene","draw");

	}DEFAULT_EXCEPTION(stderr)

	return 1;
}
