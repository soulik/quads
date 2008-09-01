#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

#include <SDL/sdl.h>
#include "objects/qPoint.h"
#include "objects/qSphere.h"
#include "objects/qQuad.h"
#include "objects/qCamera.h"
#include "objects/map/qMap.h"
#include "utils/qCollisions.h"
#include <vector>
#include "module.h"

using namespace std;

class cGamelogic : public cModule{
private:
	qVector pos;
	qVector angle;
	float increment;
	int show_cursor2;
public:
	cGamelogic(cBase * parent);
	virtual ~cGamelogic();

	qSphere * player;
	qCamera * camera;
	qMap map;

	qPointi pos2;
	qCollisions collisions;
	int segs;
	unsigned int timer;

	vector<qSphere *> objects;
	vector<qVector *> colObjs;

	int cur_object;
	int gravity;
	int show_cursor;

	int postInit();

	float DistanceCR(float x, float y, float x1, float y1, float x2, float y2);
	int Collide(float x, float y, float radius, float x1, float y1, float x2, float y2);
	int Collide(qCircle * c, qPoint topleft, qPoint bottomright);
	void testCollisions(qCircle * circle);
	int getIntersection(qCircle * circle, float x2, float y2, qPoint & intersection);
	int FindCollision(float x1, float y1, float x2, float y2, qPoint A, qPoint B, qPoint C, qPoint D, qPoint * out);
	int LinesIntersection(qPoint A, qPoint B, qPoint C, qPoint D, qPoint * out);

	void ProcessGame();

	int isMid(float a, float b, float c);

	qSphere * curObject();
	qSphere * getObject(uint id);

	//controlled methods

	void switchCamera();
	void gravityON();
	void gravityOFF();
	void increaseSegments();
	void decreaseSegments();
	void quit();
	void setPosition(int type);
	void writePosition();

	void walkForward();
	void walkBackward();
	void strafeLeft();
	void strafeRight();
	void walkUp();
	void walkDown();

	inline void setSpeed(float num){
		increment = num;
	}
	inline float getSpeed(){
		return increment;
	}

	void resetRelativePosition();
	void applyRelativePosition();

	void synchronizeData();
	void processCursor();
};

#endif /*GAMELOGIC_H_*/
