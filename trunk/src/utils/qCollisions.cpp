#include "utils/qCollisions.h"
#include "game/gamelogic.h"
#include <math.h>

qCollisions::qCollisions(){
}

qCollisions::~qCollisions(){
}

qVector qCollisions::closestPointOnLine(qVector a, qVector b, qVector p){
	qVector c = (p-a);
	qVector V = (b-a).normalize();
	float dist = (b-a).length();
	float t = (V&c);

	if (t<0) return a;
	if (t>dist) return b;

	V *= t;
	return (a+V);
}

qVector qCollisions::closestPointOnTriangle(qVector *triangle, qVector p){
	qVector Rab = closestPointOnLine(triangle[0],triangle[1],p);
	qVector Rbc = closestPointOnLine(triangle[1],triangle[2],p);
	qVector Rca = closestPointOnLine(triangle[2],triangle[0],p);

	//return (((Rab.length() < Rbc.length()) ? Rab : Rbc).length() < Rbc.length()) ? ((Rab.length() < Rbc.length()) ? Rab : Rbc) : Rbc;

	ints[0] = Rab;
	ints[1] = Rbc;
	ints[2] = Rca;

	float l[3];
	qVector temp;

	l[0] = (p-Rab).length();
	l[1] = (p-Rbc).length();
	l[2] = (p-Rca).length();

	test1 = l[0];
	test2 = l[1];
	test3 = l[2];
/*	
	if (l[0] < l[1]) temp = Rab; else
		if (l[1] < l[2]) temp = Rbc; else
			temp = Rca;
			*/
	float smallest = l[0];
	temp = Rab;

	if (smallest > l[1]){
		smallest = l[1];
		temp = Rbc;
	}

	if (smallest > l[2]){
		smallest = l[2];
		temp = Rca;
	}
			

	test4 = (p-temp).length();

	return temp;
}

int qCollisions::sameSide (qVector p1, qVector p2,qVector a, qVector b){
	qVector cp1 = (b-a)|(p1-a);
	qVector cp2 = (b-a)|(p2-a);
	if ((cp1&cp2)>=0) return 1; else return 0;
}

int qCollisions::isInsideTriangle(qVector * triangle, qVector p){
	if ((sameSide(p,triangle[0],triangle[1],triangle[2])) &&
		(sameSide(p,triangle[1],triangle[0],triangle[2])) &&
		(sameSide(p,triangle[2],triangle[0],triangle[1])))
		return 1;
	else
		return 0;
}

int qCollisions::test2ex(qSphere *sphere, qVector *triangle){
	qVector angle = sphere->getFront()*(-1);
	qVector pos  = sphere->getPosition();
	intersection2 = pos + (angle.normalize()*50);

	return 0;
}

int qCollisions::test(qSphere *sphere, qVector *triangle){
	qVector normal;
	planeNormal(triangle,&normal);
	normal = normal.normalize();
	
	//kolizia ovplyvnovana uhlom pohladu? to bola riadna hlupost
	qVector ray = sphere->getSpeed().normalize();  //sphere->getFront();
	if (ray.length()<0.001f) return 0;

	qVector pos = sphere->getPosition();
	qVector speed = sphere->getSpeed();
	float radius = sphere->getRadius();
	pos = pos + speed;

	float d=(normal&triangle[0]);
	float numer = (normal&pos)+d;
	float denom = (normal&ray);

	last_result = int(-(numer/denom));

	qVector perimeter = closestPointOnTriangle(triangle, pos);
	//intersection = perimeter;
	
	ints[5]  = perimeter;

	distance = fabs((normal&pos) - d) / normal.length();
	if ((pos-perimeter).length()>distance) distance = (pos-perimeter).length();

	if ((speed&normal)>0){
		//intersection = pos;
		//return 0;
	}
	
	/* sphere intersection point */
	qVector motion = (normal*sphere->getRadius())*(-1);
	float t = -((pos & normal) - d)/(ray & normal);
	qVector inter = pos + (ray*t);

	ints[3]  = inter;
	/* is this intersection on triangle? */
	int onTriangle = isInsideTriangle(triangle,inter);

	/* distance of sphere origin from plane */
	qVector inter_per;
	//if (distance<=radius){
	{
		inter_per = pos + ((normal*distance)*(-1));
	}
	ints[4] = inter_per;

	int onTriangle2 = isInsideTriangle(triangle,inter_per);

	if	((pos-inter_per).length()<((pos-inter).length())){
		inter = inter_per;
	}
	intersection2 = inter;
		
	if ((!onTriangle2)){
		inter = perimeter;
	}
 
	/* the closest point on triangle */
	/*
	if ((((pos-perimeter).length()<(pos-inter).length())) || (!onTriangle)){
		inter = perimeter;
	}
	*/
	/*
	if ((pos-inter).length()<(pos-intersection).length()){
		intersection = inter;
	}
	*/
	/*
	else{
		colTest=0;
		return 0;
	}
	*/

	//if ((pos-inter).length() < (pos-intersection).length()){
	{
		nnn = normal;
		intersection = inter;
	}


	if ((pos-intersection).length()>radius){
		colTest = 0; 
	}else{
		colTest = 1;

	}

	return int(-(numer/denom));
}

int qCollisions::find(qSphere *sphere, qVector *triangle, qVector *out){
	qVector normal;
	planeNormal(triangle,&normal);
	normal = normal.normalize();
	return 0;
}

void qCollisions::planeNormal(qVector *triangle, qVector * normal){
	qVector a = triangle[0] - triangle[1];
	qVector b = triangle[0] - triangle[2];
	qVector n = a|b;
	normal->set(n.x,n.y,n.z);
}

float qCollisions::getHeightFromHMap(qVector _pos, cGamelogic * glogic){
	int mapX,mapY,h1,h2,h3,h4;
	float part=8.0f;
	float height = 0.0f;
	float modX,modY;
	mapX = (int)floor(_pos.x/part);
	mapY = (int)floor(_pos.y/part);

	modX = fmod(_pos.x,part);
	modY = fmod(_pos.y,part);

	h1 = glogic->map.GetMap(mapX,mapY)->z;
	h2 = glogic->map.GetMap(mapX+1,mapY)->z;
	//h3 = glogic->map.GetMap(mapX+1,mapY+1)->z;
	h3=0;
	//h3 = glogic->GetMap(mapX+1,mapY+1);
	h4 = glogic->map.GetMap(mapX,mapY+1)->z;

	qVector normal;
	qVector triangle[3];
	triangle[0] = qVector((float)mapX*8,		(float)mapY*8,		(float)h1);
	triangle[1] = qVector((float)(mapX+1)*8,	(float)mapY*8,		(float)h2);
	triangle[2] = qVector((float)mapX*8,		(float)(mapY+1)*8,	(float)h4);
	planeNormal(triangle,&normal);
	float d = normal & triangle[0]; 
	height = (d - normal.x*_pos.x - normal.y*_pos.y) / normal.z;
	//height = (d - normal.x*modX - normal.y*modY) / normal.z;

	return height;
}
