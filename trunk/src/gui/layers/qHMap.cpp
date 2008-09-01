#include "gui/layers/qHMap.h"
#include "common.h"
#include "base.h"
#include "modules/game.h"
#include "modules/renderer.h"
#include "modules/script.h"
#include <math.h>

int qHMap::draw(){
	return 1;
}

void qHMap::drawHMapPointsAroundPlayer(qVector pos){
	float height,x=0,y=0,step=4.0f,area=32.0f;
	for (y=pos.y-area; y<pos.y+area; y+=step){
		for (x=pos.x-area; x<pos.x+area; x+=step){
			height = GAME->glogic->collisions.getHeightFromHMap(qVector(x,y,0),GAME->glogic);
			glBegin(GL_POINTS);
				glColor3f(1,0,0);
				glVertex3f(x,y,height-1);
			glEnd();
		}
	}
}


void qHMap::drawHMapPoints(qVector pos){
	int mapX,mapY,h1,h2,h3,h4;
	mapX = (int)floor(pos.x/8);
	mapY = (int)floor(pos.y/8);
	h1 = GAME->glogic->map.GetMap(mapX,mapY)->z;
	h2 = GAME->glogic->map.GetMap(mapX+1,mapY)->z;
	h3 = GAME->glogic->map.GetMap(mapX+1,mapY+1)->z;
	h4 = GAME->glogic->map.GetMap(mapX,mapY+1)->z;
	glBegin(GL_LINE_LOOP);
		glColor3f(0,0,1);
		glVertex3f((float)mapX*8,	(float)mapY*8,	(float)h1-0.5f);
		glColor3f(1,1,1);
		glVertex3f((float)(mapX+1)*8,	(float)mapY*8,	(float)h2-0.5f);
		glVertex3f((float)(mapX+1)*8,	(float)(mapY+1)*8,	(float)h3-0.5f);
		glVertex3f((float)mapX*8,	(float)(mapY+1)*8,	(float)h4-0.5f);
		//glVertex3f(mapX*8,mapY*8,h1-0.5f);
	glEnd();
}