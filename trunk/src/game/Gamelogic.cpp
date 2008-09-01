#include "game/gamelogic.h"
#include "modules/game.h"
#include "modules/controler.h"
#include "modules/qTexturer.h"
#include "modules/renderer.h"
#include <math.h>
#include "base.h"
#include "common.h"
#include "utils/lmem.h"

cGamelogic::cGamelogic(cBase * parent): cModule(parent){
	setName("cGamelogic");
	player = new qSphere(qVector(64,64,64),qVector(0,0,0),32);
	player->setElasticity(0.5f);
	camera = new qCamera();
	segs = 18;
	cur_object = 0;
	timer=0;
	gravity = 0;
	increment=0.2f;
	show_cursor=0;
	show_cursor2=-1;
}

cGamelogic::~cGamelogic(){
	delete camera;
	delete player;

	for (unsigned int i=0; i < objects.size();i++){
		delete objects[i];
	}

	for (unsigned int i=0; i < colObjs.size();i++){
		delete colObjs[i];
	}
}

int cGamelogic::postInit(){

	SDL_EnableKeyRepeat(200,50);

	//RENDER->initWindows();

	objects.push_back(new qSphere(qVector(0,0,0),qVector(0,0,0),16));
	objects.push_back(new qSphere(qVector(0,0,-15),qVector(90,0,0),4));
	objects.push_back(new qSphere(qVector(64,64,-15),qVector(90,0,0),4));
	objects[0]->setFriction(1.5f);
	objects[1]->setFriction(1.5f);
	objects[2]->setFriction(1.5f);
/*	 
	colObjs.push_back(new qVector(64,64,-10));
	colObjs.push_back(new qVector(64,80*5,-10));
	colObjs.push_back(new qVector(80*5,64,-10));
  
	colObjs.push_back(new qVector(64,80*5,-10));
	colObjs.push_back(new qVector(80*5,80*5,-10));
	colObjs.push_back(new qVector(80*5,64,-10));
	
	colObjs.push_back(new qVector(64,80*5,-10));
	colObjs.push_back(new qVector(80*5,80*5,-10));
	colObjs.push_back(new qVector(64,80*5,-10*80));

	colObjs.push_back(new qVector(64,80*5,-10*80));
	colObjs.push_back(new qVector(80*5,80*5,-10));
	colObjs.push_back(new qVector(80*5,80*5,-10*80));
	*/
/*
	triangle[0].set(64,64,-10);
	triangle[1].set(80,80,-26);
	triangle[2].set(80,64,-10);
  */
	return 1;
}

int cGamelogic::getIntersection(qCircle * circle, float x2, float y2, qPoint & intersection){
	qPoint ipos;
	qVector ppos = circle->getPosition();
	return 0;
}

void cGamelogic::testCollisions(qCircle * circle){
	circle->update();
	for (unsigned int i=0;i<map.worldMap.size();i++){
		//float distance[4];
		//qVector qpos = ((qQuad*)map.worldMap[i])->getPosition();
		//float width = ((qQuad*)map.worldMap[i])->getWidth();
		//float height = ((qQuad*)map.worldMap[i])->getHeight();
		//vertical lines
		//distance
		
	}
	
}

float cGamelogic::DistanceCR(float x, float y, float x1, float y1, float x2,float y2){
	float distA = sqrtf( (x1-x)*(x1-x)+(y1-y)*(y1-y) );
	float distB = sqrtf( (x2-x)*(x2-x)+(y1-y)*(y1-y) );
	float distC = sqrtf( (x1-x)*(x1-x)+(y2-y)*(y2-y) );
	float distD = sqrtf( (x2-x)*(x2-x)+(y2-y)*(y2-y) );
	
	//this is not 100% accurate, it only tests for rectangle corner distances
	#define dist1 ((distA<distB)? distA : distB)
	#define dist2 ((dist1 < distC) ? dist1 : distC)
	return ((dist2 < distD) ? dist2 : distD);
}

int cGamelogic::isMid(float a, float b, float c){
#define MIN(_a,_b) ( ((_a)<(_b)) ? (_a) : (_b))
#define MAX(_a,_b) ( ((_a)<(_b)) ? (_b) : (_a))
	
	if ( (b>=MIN(a,c)) && (b<=MAX(a,c)) ) return 1;
	return 0;
}

int cGamelogic::LinesIntersection(qPoint A, qPoint B, qPoint C, qPoint D, qPoint * out){
	if (!out) return 0;
	float dd = ((D.y-C.y)*(B.x-A.x)-(D.x-C.x)*(B.y-A.y));
	
	if (dd==0) return 0;

	float u_a = ((D.x-C.x)*(A.y-C.y)-(D.y-C.y)*(A.x-C.x)) / dd;
	float u_b = ((D.x-C.x)*(A.y-C.y)-(D.y-C.y)*(A.x-C.x)) / dd;

	if ((u_a>0)&&(u_a<1)&&(u_b>0)&&(u_b<1)){
		out->set(A.x+u_a*(B.x-A.x), A.y+u_a*(B.y-A.y));
		return 1;
	}

	return 0;
}

int cGamelogic::FindCollision(float x1, float y1, float x2, float y2, qPoint A, qPoint B, qPoint C, qPoint D, qPoint * out){
	if (!out) return 0;

	qPoint  vector = qPoint(x2-x1, y2-y1);
	qPoint  normal = vector.normalize();
	qPoint	temp;
	int intersections = 0;

	// there's no intersection with top and bottom side if vector is horizontal
	/*if (vector.y!=0)*/{
		// top side
		if (LinesIntersection(qPoint(x1,y1),qPoint(x2,y2),qPoint(A.x,A.y),qPoint(B.x,B.y),&temp)){
			if (isMid(A.x,temp.x,B.x)){
				out[intersections].set(temp.x,temp.y);
				intersections++;
			}
		}

		// bottom side
		if (LinesIntersection(qPoint(x1,y1),qPoint(x2,y2),qPoint(C.x,C.y),qPoint(D.x,D.y),&temp)){
			if (isMid(C.x,temp.x,D.x)){
				out[intersections].set(temp.x,temp.y);
				intersections++;
			}
		}
	}

	// there's no intersection with left and right side if vector is vertical
	
	/*if (vector.x!=0)*/{
		// left side
		if (LinesIntersection(qPoint(x1,y1),qPoint(x2,y2),qPoint(A.x,A.y),qPoint(C.x,C.y),&temp)){
			if (isMid(A.y,temp.y,C.y)){
				out[intersections].set(temp.x,temp.y);
				intersections++;
			}
		}

		// right side
		if (LinesIntersection(qPoint(x1,y1),qPoint(x2,y2),qPoint(B.x,B.y),qPoint(D.x,D.y),&temp)){
			if (isMid(B.y,temp.y,D.y)){
				out[intersections].set(temp.x,temp.y);
				intersections++;
			}
		}
		
		
	}
	return intersections;
}

int cGamelogic::Collide(float x, float y, float radius, float x1, float y1, float x2,float y2){
	/*
		Collision test optimized for quares
	*/

	qPoint	A = qPoint(x1,y1),
			B = qPoint(x2,y1),
			C = qPoint(x2,y2),
			D = qPoint(x1,y2);
	/*
		distances
	*/
	float	distA=-1,
			distB=-1,
			distC=-1,
			distD=-1,
			minDist;

	/*
		position flags
		
		0x01 =	within left and right side
		0x02 =	within top and bottom side
	*/
	unsigned char pos_flag=0;

	if ((x>A.x)&&(x<B.x)) pos_flag |= 0x01;
	if ((y>A.y)&&(y<D.y)) pos_flag |= 0x02;

	/*
		we are inside
	*/
	if ((pos_flag & 0x01)&&(pos_flag & 0x02)) return 1;

	/*
		distances from sides
	*/

	if (pos_flag & 0x01){
		distA = A.y-y;
		distC = y-C.y;
	}

	if (pos_flag & 0x01){
		distD = A.x-x;
		distB = x-C.x;
	}
	
	/*
		let's find the minimal distance
	*/
	minDist = DistanceCR( x,  y, x1, y1, x2, y2);

	if ((distA>0)&&(distA<minDist)) minDist = distA;
	if ((distB>0)&&(distB<minDist)) minDist = distB;
	if ((distC>0)&&(distC<minDist)) minDist = distC;
	if ((distD>0)&&(distD<minDist)) minDist = distD;

	if (minDist<radius) return 1;

	return 0;
}

int cGamelogic::Collide(qCircle * c, qPoint topleft, qPoint bottomright){
	return Collide(c->getPosition().x,c->getPosition().y,c->getRadius(),topleft.x,topleft.y,bottomright.x,bottomright.y);
}

void cGamelogic::ProcessGame(){
	//map.GenerateMapV2(map.getWidth(),map.getHeight(),float(timer)/(360.0f*2));
	testCollisions(player);
	
	qVector triangle[3];

	for (uint i=0; i< colObjs.size(); i+=3){
		
		triangle[0] = *(colObjs[i]);
		triangle[1] = *(colObjs[i+1]);
		triangle[2] = *(colObjs[i+2]);

		qVector _iii2=collisions.intersection;
		qVector _nnn2=collisions.nnn;
		int _test = collisions.colTest;
		
		collisions.test(getObject(0),triangle);
		if (i<=0){
			_iii2=collisions.intersection;
			_nnn2=collisions.nnn;
			_test = collisions.colTest;
		}
		
		if ((i>0)&&(((collisions.intersection-objects[cur_object]->getPosition()).length()>(_iii2-objects[cur_object]->getPosition()).length()))){
			collisions.intersection = _iii2;
			collisions.nnn = _nnn2;
			collisions.colTest = _test;
		}
	}
	
	//collisions.test2ex(getObject(0),triangle);

	if (gravity==1){
		qVector gr = objects[cur_object]->getAcceleration();
		gr = gr-qVector(0,0,-0.1f);
		if (gr.length()>0.5f){
			gr = gr.normalize()*0.5f;
		}
		objects[cur_object]->setAcceleration(gr);
	}

	for (uint i=0; i < objects.size();i++){
		if (i==0){
			if (collisions.colTest==1) 
				objects[i]->update(&collisions.intersection,&collisions.intersection2,&collisions.nnn);
			else
				objects[i]->update();
		}else{
			objects[i]->update();
		}
	}

	qVector _pos = GAME->glogic->curObject()->getPosition();
	int height,part=8;
	//is player on heightmap?
	if ((map.flags & 0x01)&&((_pos.x>0)&&(_pos.y>0)&&(_pos.x < map.getWidth()*part-1)&&(_pos.y < map.getHeight()*part-1))){
		height = (int)collisions.getHeightFromHMap(_pos,this);
		//curObject()->setPosition(qVector(_pos.x,_pos.y,height - curObject()->getRadius()));
	}
/*
	camera->setPosition(curObject()->getPosition());
	camera->setAngle(curObject()->getAngle());
*/
	int rStatus = RENDER->getStatus();

	if ((rStatus & TS_RUNNING)&&(rStatus & TS_POSTINIT)){
		timer++;
		//char * text = (char*) malloc(4096);
		//char text[4096];
		/*
		if ((CONTROL->getKeyboard()->GetKey(SDLK_RSHIFT))||(CONTROL->getKeyboard()->GetKey(SDLK_LSHIFT))){
			camera->setAngleFromRelMouse(CONTROL->getMouse()->getRelPos());
		}
		*/
		//curObject()->setPosition(c);
		curObject()->setAngle(camera->getAngle());

		synchronizeData();
		RENDER->updateWindows();		
		//free(text);
	}
}

qSphere * cGamelogic::curObject(){
	if (objects.size()<1){
		return camera;
	}else{
		return objects[cur_object];
	}
}

qSphere * cGamelogic::getObject(uint id){
	if (objects.size()<(id+1)){
		return camera;
	}else{
		return objects[id];
	}
}

//controled methods

void cGamelogic::switchCamera(){
	cur_object++;
	if ((ulong)cur_object >= objects.size()) cur_object = 0;
	SDL_Delay(200);
}
void cGamelogic::gravityON(){
	gravity = 0;
	dprintf("Gravity OFF\n");
}
void cGamelogic::gravityOFF(){
	gravity = 1;
	dprintf("Gravity ON\n");
}
void cGamelogic::increaseSegments(){
	if (segs<=64) segs++;
	printf("Segments count: %d\n",segs);
	SDL_Delay(100);
}
void cGamelogic::decreaseSegments(){
	if (segs>3) segs--;
	dprintf("Segments count: %d\n",segs);
	SDL_Delay(100);
}
void cGamelogic::quit(){
	GAME->requestStop();
}
void cGamelogic::setPosition(int type){
	qVector _pos;

	if (type){
		_pos = CONTROL->getMouse()->getPos();
	}else{
		_pos = player->getPosition();
	}

	this->pos2.x = (int)_pos.x;
	this->pos2.y = (int)_pos.y;
}

void cGamelogic::writePosition(){
	qVector _pos = player->getPosition();
	dprintf("X1: %d Y1: %d\nX2: %d Y2: %d\n---------\n",(int)_pos.x,(int)_pos.y,pos2.x,pos2.y);
}

void cGamelogic::walkForward(){
	pos -= curObject()->getFront()*increment;
}
void cGamelogic::walkBackward(){
	pos += curObject()->getFront()*increment;
}
void cGamelogic::strafeLeft(){
	pos -= curObject()->getSide()*increment*0.5f;
}
void cGamelogic::strafeRight(){
	pos += curObject()->getSide()*increment*0.5f;
}
void cGamelogic::walkUp(){
	pos.z -= increment;
}
void cGamelogic::walkDown(){
	pos.z += increment;
}

void cGamelogic::resetRelativePosition(){
	pos = qVector(0,0,0);
	angle = curObject()->getAngle();
}

void cGamelogic::applyRelativePosition(){
	curObject()->setAcceleration(pos);
}

void cGamelogic::synchronizeData(){
	if (LuaData){
		//dprintf("LuaData Online");
	}
	SCRIPT->updateData();
}

void cGamelogic::processCursor(){
	if (show_cursor!=show_cursor2){
		if (show_cursor){
			SDL_ShowCursor(SDL_ENABLE);
		}else{
			SDL_ShowCursor(SDL_DISABLE);
		}
		show_cursor2=show_cursor;
	}
}