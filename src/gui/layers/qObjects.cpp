#include "gui/layers/qObjects.h"
#include "common.h"
#include "base.h"
#include "modules/game.h"
#include "modules/renderer.h"
#include <math.h>

qObjects::qObjects(){
	dList[0] = glGenLists(1);
	glNewList(dList[0],GL_COMPILE);
}

int qObjects::draw(){
	for (unsigned int i=0; i< GAME->glogic->objects.size(); i++){
		if (GAME->glogic->cur_object!=i){
			qSphere * object = GAME->glogic->objects[i];
			if (i==0){
				glPushMatrix();
				qVector pos = object->getPosition();
				glTranslatef(pos.x,pos.y,pos.z);
				glCallList(dList[0]);
				glPopMatrix();
			}else{
				RENDER->drawPoint(object->getPosition(),qRGBA(1,1,0,1),8);
			}
		}
	}
	return 1;
}
