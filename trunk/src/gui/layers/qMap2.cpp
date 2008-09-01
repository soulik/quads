#include "gui/layers/qMap2.h"
#include "common.h"
#include "base.h"
#include "modules/game.h"
#include "modules/renderer.h"
#include <math.h>

int qMap2::draw(){
	for (ulong y=0;y<GAME->glogic->map.getHeight();y++){
		for (ulong x=0;x<GAME->glogic->map.getWidth();x++){
			#define PX (GAME->glogic->player->getPosition().x)
			#define PY (GAME->glogic->player->getPosition().y)
			int dx = int(PX-x*16-8);
			int dy = int(PY-y*16-8);
			int dd = (int)sqrt((float)(dx*dx + dy*dy));
			/*
			int px=((PX+1)/16);
			int py=((PY+1)/16);
			*/
			if (dd<39)	// (16 + SQRT(16*16+16+16))
				RENDER->drawBox2(x*16,y*16,16,16);
		}
	}
	return 1;
}

