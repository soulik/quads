#include "utils/md3.h"
#include "errors/errors.h"
#include "errors/exception.h"
#include <fstream>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "common.h"
#include "base.h"
#include "modules/script.h"
#include "utils/qQuaternion.h"
#include "gui/cGL.h"

bool IsInString(string strString, string strSubString){
	// Make sure both of these strings are valid, return false if any are empty
	if(strString.length() <= 0 || strSubString.length() <= 0) return false;

	// grab the starting index where the sub string is in the original string
	unsigned int index = strString.find(strSubString);

	// Make sure the index returned was valid
	if(index >= 0 && index < strString.length())
		return true;

	// The sub string does not exist in strString.
	return false;
}


cMD3::cMD3(){
	debug = NULL;
}

cMD3::~cMD3(){
	hash_map<string,cMD3Model*>::iterator model_iter;
	hash_map<string,cMD3Animation*>::iterator anim_iter;
	for( model_iter = models.begin(); model_iter != models.end(); model_iter++ ) {
		if (model_iter->second) delete model_iter->second;
	}
}

void cMD3::loadModel( const char * fname, const char * style ){
	char temp_name[256]={};
	if (!style){
		style="_default";
	}
	cMD3Model * temp_mdl;
	try{
		name = fname;
		temp_mdl = new cMD3Model();
		sprintf(temp_name,"%s\\lower.md3",fname);
		temp_mdl->loadModel(temp_name);
		models.insert(make_pair("lower",temp_mdl));
		sprintf(temp_name,"%s\\lower%s.skin",fname,style);
		temp_mdl->loadSkin(temp_name);

		temp_mdl = new cMD3Model();
		sprintf(temp_name,"%s\\upper.md3",fname);
		temp_mdl->loadModel(temp_name);
		models.insert(make_pair("upper",temp_mdl));
		sprintf(temp_name,"%s\\upper%s.skin",fname,style);
		temp_mdl->loadSkin(temp_name);

		temp_mdl = new cMD3Model();
		sprintf(temp_name,"%s\\head.md3",fname);
		temp_mdl->loadModel(temp_name);
		models.insert(make_pair("head",temp_mdl));
		sprintf(temp_name,"%s\\head%s.skin",fname,style);
		temp_mdl->loadSkin(temp_name);
		sprintf(temp_name,"%s\\animation.cfg",fname);

		loadAnimation(temp_name);

		models["lower"]->linkTo(models["upper"],"tag_torso");
		models["upper"]->linkTo(models["head"],"tag_head");

		models["upper"]->setAnimation("TORSO_STAND");
		models["lower"]->setAnimation("LEGS_WALK");
		
	}catch (cException e) {
		throw e;
	}
}

void cMD3::prepareMaterials(){

}

void cMD3::updateMaterial( const char * material_name ){

}

void cMD3::drawModel( int mode ){
	glRotatef(-180, 1, 0, 0);

	models["upper"]->update();
	models["lower"]->update();

	models["lower"]->drawLinkedModel(mode);
}

void cMD3::setDebug( qDebug * debug ){
	this->debug = debug;
}

void cMD3::loadAnimation( const char * fname ){
	try{
		if(fname)	{
			ifstream fin(fname);
			if(fin.fail())	{
				throw(cException("Can't read animation file '%s'",fname));
			}
			string strLine = "";
			int a,b,c,d,torso_offset=0;
			char buffer[256];
			cMD3Animation * anim;

			while(getline(fin, strLine)){
				if (sscanf(strLine.c_str(),"%d %d %d %d // %[^\t^ ^(^)]",&a,&b,&c,&d,buffer)>4){
					anim = new cMD3Animation;
					anim->start_frame = a;
					anim->end_frame = a+b;
					anim->looping_frames = c;
					anim->fps = d;
					if (strstr(buffer,"BOTH")){
						models["upper"]->setAnimation(buffer,anim);
						models["lower"]->setAnimation(buffer,anim);
					}else
					if (strstr(buffer,"TORSO")){
						models["upper"]->setAnimation(buffer,anim);
					}else
					if (strstr(buffer,"LEGS")){
						if (!torso_offset){
							if (!strcmp(buffer,"LEGS_WALKCR")){
								torso_offset = anim->start_frame - models["upper"]->getAnimation("TORSO_GESTURE")->start_frame;
							}else{
								torso_offset = models["lower"]->getAnimation("LEGS_WALKCR")->start_frame - models["upper"]->getAnimation("TORSO_GESTURE")->start_frame;
							}
						}
						anim->start_frame -= torso_offset;
						anim->end_frame -= torso_offset;

						models["lower"]->setAnimation(buffer,anim);
					}
				}
			}
			fin.close();
		}
	}catch(cException e){
		throw (e);
	}
	return;
}

void cMD3::setFrame( const char * obj_name,int num ){
	hash_map<string,cMD3Model*>::iterator iter = models.find(obj_name);
	if (iter != models.end()){
		(iter->second)->setFrame(num);
	}
}

void cMD3::setAnimation(const char * obj_name, const char * name ){
	hash_map<string,cMD3Model*>::iterator iter = models.find(obj_name);
	if (iter != models.end()){
		(iter->second)->setAnimation(name);
	}
}

void cMD3::update(){
	hash_map<string,cMD3Model*>::iterator model_iter;
	hash_map<string,cMD3Animation*>::iterator anim_iter;
	for( model_iter = models.begin(); model_iter != models.end(); model_iter++ ) {
		if (model_iter->second) model_iter->second->update();
	}
}
cMD3Model::cMD3Model(){
	blank_animation.start_frame = 0;
	blank_animation.end_frame = 0;
	blank_animation.looping_frames = 0;
	blank_animation.fps = 0;
	current_animation = "";
	current_frame = 0;
	start_frame = 0;
	end_frame = 0;
	next_frame = 0;
	time = 0;
	lastTime = 0;
}

cMD3Model::~cMD3Model(){
	vector<cMD3Tag*>::iterator tag_iter;
	hash_map<string,cMD3Object*>::iterator object_iter;
	hash_map<string,cMD3Animation*>::iterator anim_iter;
	vector<cMD3Bone*>::iterator bone_iter;

	for( tag_iter = tags.begin(); tag_iter != tags.end(); tag_iter++ ) {
		if (*tag_iter) delete (*tag_iter);
	}
	for( object_iter = objects.begin(); object_iter != objects.end(); object_iter++ ) {
		if (object_iter->second) delete object_iter->second;
	}
	for( bone_iter = bones.begin(); bone_iter != bones.end(); bone_iter++ ) {
		if (*bone_iter) delete (*bone_iter);
	}
	for( anim_iter = animations.begin(); anim_iter != animations.end(); anim_iter++ ) {
		if (anim_iter->second) delete anim_iter->second;
	}
}

void cMD3Model::loadShader( const char * fname ){

}

void cMD3Model::loadSkin( const char * fname ){
	try{
		if(fname)	{
			ifstream fin(fname);
			if(fin.fail())	{
				throw(cException("Can't read skin file '%s'",fname));
			}
			string strLine = "";
			char obj_name[256]={0};
			char tex_name[512]={0};
			hash_map<string,cMD3Object*>::iterator object_iter;

			while(getline(fin, strLine)){
				if (sscanf(strLine.c_str(),"%[^,],%s",obj_name,tex_name)>1){
					for( object_iter = objects.begin(); object_iter != objects.end(); object_iter++ ) {
						if (object_iter->second){
							const cMD3Mesh * mesh = object_iter->second->getMeshInfo();
							if(!strcmp(obj_name,mesh->strName)){
								int tex_id = __SCRIPT->call("loadTex",tex_name,0x01);
								object_iter->second->setTexture(tex_id);
							}						
						}
					}
				}
			}
			fin.close();
		}
	}catch(cException e){
		throw (e);
	}
	return;
}

void cMD3Model::loadModel( const char * fname ){
	try{
		if (fname){
			f = fopen(fname,"rb");
			if (f){
				fread(&header,1,sizeof(cMD3Header),f);
				if (!memcmp(header.fileID,MD3_ID,4)){
					readBones();
					readTags();
					readMesh();
				}else{
					throw(cException("'%s' is not valid MD3 model",fname));
				}
				fclose(f);
			}
		}
	}catch(cException e){
		throw(e);
	}
}

void cMD3Model::readBones(){
	cMD3Bone * bone;
	for (int i=0; i<header.numFrames;i++ ){
		bone = new cMD3Bone;
		fread(bone,1,sizeof(cMD3Bone),f);
		bones.push_back(bone);
	}
}
void cMD3Model::readTags(){
	cMD3Tag * tag;
	for (int i=0; i<header.numFrames*header.numTags;i++ ){
		tag = new cMD3Tag;
		fread(tag,1,sizeof(cMD3Tag)-sizeof(cMD3Model*),f);
		//warning!
		tag->link = NULL;
		tags.push_back(tag);
	}
}
void cMD3Model::readMesh(){
	long meshOffset = ftell(f);

	cMD3Object * object;

	for (int i=0; i<header.numMeshes;i++ ){
		if (fseek(f, meshOffset, SEEK_SET)!=0) throw(cException("Can't read mesh"));
		object = new cMD3Object();
		const cMD3Mesh * mesh = object->readObject(f,meshOffset);
		meshOffset += mesh->meshSize;
		object->process();
		objects.insert(make_pair(mesh->strName,object));
	}
}
void cMD3Object::readVertices(FILE *f){
	cMD3Triangle tri;
	cMD3Vertex * _vertex;
	float lat =0;
	float lng =0;
	unsigned short normal;

	for (int i=0; i<mesh.numVertices*mesh.numMeshFrames;i++ ){
		fread(&tri,sizeof(cMD3Triangle),1,f);
		_vertex = new cMD3Vertex;
		_vertex->vertex[0] = (float)tri.vertex[0]/64.0f;
		_vertex->vertex[1] = (float)tri.vertex[1]/64.0f;
		_vertex->vertex[2] = (float)tri.vertex[2]/64.0f;

		normal = tri.normal[0] | (tri.normal[1]<<8);
		lat = float(((normal >> 8) & 255)* (2*M_PI) / 255);
		lng = float((normal & 255)*(2*M_PI)/255);
		_vertex->normal[0] = cos ( lat ) * sin ( lng );
		_vertex->normal[1] = sin ( lat ) * sin ( lng );
		_vertex->normal[2] = cos(lng);
		vertices.push_back(_vertex);
	}
}
void cMD3Object::readFaces(FILE * f){
	cMD3Face * face;
	for (int i=0; i<mesh.numTriangles;i++ ){
		face = new cMD3Face;
		fread(face,sizeof(cMD3Face),1,f);
		faces.push_back(face);
	}
}
void cMD3Object::readSkins(FILE * f){
	cMD3Skin * skin;
	for (int i=0; i<mesh.numSkins;i++ ){
		skin = new cMD3Skin;
		fread(skin,sizeof(cMD3Skin),1,f);
		skins.insert(make_pair(skin->strName,skin));
	}
}

void cMD3Object::readTexCoords(FILE * f){
	cMD3TexCoord * texCoord;
	for (int i=0; i<mesh.numVertices;i++ ){
		texCoord = new cMD3TexCoord;
		fread(texCoord,sizeof(cMD3TexCoord),1,f);
		texCoords.push_back(texCoord);
	}
}

void cMD3Model::readShaders(){

}


void cMD3Model::drawModel(int index, int index2, float t, int mode){
	hash_map<string,cMD3Object*>::iterator object_iter;
	for( object_iter = objects.begin(); object_iter != objects.end(); object_iter++ ) {
		if (object_iter->second) (object_iter->second)->draw(index,index2,t,mode);
	}
}

void cMD3Model::linkTo( cMD3Model * model , const char * name){
	if (model && name){
		vector<cMD3Tag*>::iterator tag_iter;
		for( tag_iter = tags.begin(); tag_iter != tags.end(); tag_iter++ ) {
			if (*tag_iter){
				if (!strcmp((*tag_iter)->strName,name)){
					(*tag_iter)->link = model;
				}
			}
		}
	}
}

void cMD3Model::drawLinkedModel(int mode){
	drawModel(current_frame,next_frame,time,mode);
	qQuaternion qQuat, qNextQuat, qInterpolatedQuat;
	float *pMatrix, *pNextMatrix;
	float finalMatrix[16] = {0};

	static int recursions=0;
	recursions++;
	cMD3Model * link;

	for( int i=0; i<header.numTags; i++) {
		if (tags.at(i)->link){
			link = tags.at(i)->link;
		}else{
			continue;
		}

	// To find the current translation position for this frame of animation, we times
	// the currentFrame by the number of tags, then add i.  This is similar to how
	// the vertex key frames are interpolated.
		qVector vPosition = (tags.at(current_frame*header.numTags+i)->vPosition);

		// Grab the next key frame translation position
		qVector vNextPosition = (tags.at(current_frame*header.numTags+i)->vPosition);

		// By using the equation: p(t) = p0 + t(p1 - p0), with a time t,
		// we create a new translation position that is closer to the next key frame.
		vPosition.x = vPosition.x + time * (vNextPosition.x - vPosition.x),
		vPosition.y	= vPosition.y + time * (vNextPosition.y - vPosition.y),
		vPosition.z	= vPosition.z + time * (vNextPosition.z - vPosition.z);			

		// Now comes the more complex interpolation.  Just like the translation, we
		// want to store the current and next key frame rotation matrix, then interpolate
		// between the 2.

		// Get a pointer to the start of the 3x3 rotation matrix for the current frame
		pMatrix = &(tags.at(current_frame*header.numTags + i)->rotation[0][0]);

		// Get a pointer to the start of the 3x3 rotation matrix for the next frame
		pNextMatrix = &(tags.at(next_frame*header.numTags + i)->rotation[0][0]);

		// Now that we have 2 1D arrays that store the matrices, let's interpolate them

		// Convert the current and next key frame 3x3 matrix into a quaternion
		qQuat.CreateFromMatrix( pMatrix, 3);
		qNextQuat.CreateFromMatrix( pNextMatrix, 3 );

		// Using spherical linear interpolation, we find the interpolated quaternion
		qInterpolatedQuat = qQuat.Slerp(qQuat, qNextQuat, time);

		// Here we convert the interpolated quaternion into a 4x4 matrix
		qInterpolatedQuat.CreateMatrix( finalMatrix );

		// To cut out the need for 2 matrix calls, we can just slip the translation
		// into the same matrix that holds the rotation.  That is what index 12-14 holds.
		finalMatrix[12] = vPosition.x;
		finalMatrix[13] = vPosition.y;
		finalMatrix[14] = vPosition.z;

		glPushMatrix();
			glMultMatrixf( finalMatrix );
			//glTranslatef(tags.at(i)->vPosition.x,tags.at(i)->vPosition.y,tags.at(i)->vPosition.z);
			link->drawLinkedModel(mode);
		glPopMatrix();
	}
	recursions--;
}

void cMD3Model::setAnimation( const char * name, cMD3Animation * anim ){
	if (current_animation.length()<1){
		current_animation = name;
	}
	animations.insert(make_pair(name,anim));
}

void cMD3Model::setAnimation( const char * name){
	current_animation = name;
	hash_map<string,cMD3Animation*>::iterator iter = animations.find(name);
	if (iter != animations.end()){
		current_frame = (iter->second)->start_frame;
	}

}

cMD3Animation * cMD3Model::getAnimation( const char * name ){
	hash_map<string,cMD3Animation*>::iterator iter = animations.find(name);
	if (iter != animations.end()){
		return (iter->second);
	}else{
		return &blank_animation;
	}

}

void cMD3Model::update(){
	int start_frame = 0;
	int end_frame = 1;
	if (current_animation.length()>0){
		cMD3Animation * c_anim = getAnimation(current_animation.c_str());
		if (animations.size()>0 && c_anim){
			start_frame = c_anim->start_frame;
			end_frame = c_anim->end_frame;
			if (end_frame<1) end_frame = 1;
		}
		next_frame = (current_frame + 1) % end_frame;
		if(next_frame == 0) 
			next_frame =  start_frame;
	}
	setTime();
}

void cMD3Model::setFrame( int num ){
	current_frame = num;
}

void cMD3Model::setTime(){
	int elapsedTime   = 0;

	if(!animations.size()) return;

	int _time = GetTickCount();

	elapsedTime = _time - lastTime;
	int animationSpeed;
	if(current_animation.length()<1){
		animationSpeed = 25;
	}else{
		cMD3Animation * anim = animations[current_animation];
		if (anim){
			animationSpeed = anim->fps;
		}else{
			animationSpeed = 25;
		}
	}

	float t = float(elapsedTime) / (1000.0f / animationSpeed);

	// If our elapsed time goes over the desired time segment, start over and go 
	// to the next key frame.
	if (elapsedTime >= (1000.0f / animationSpeed) ){
		// Set our current frame to the next key frame (which could be the start of the anim)
		current_frame = next_frame;

		// Set our last time for the model to the current time
		lastTime = _time;
	}

	// Set the t for the model to be used in interpolation
	this->time = t;
	
}

cMD3Object::cMD3Object(){
	texture = 0;
}

cMD3Object::~cMD3Object(){
	vector<cMD3TexCoord*>::iterator texCoord_iter;
	vector<cMD3Face*>::iterator face_iter;
	vector<cMD3Vertex*>::iterator vertex_iter;
	hash_map<string,cMD3Skin*>::iterator skin_iter;
	for( texCoord_iter = texCoords.begin(); texCoord_iter != texCoords.end(); texCoord_iter++ ) {
		if (*texCoord_iter) delete (*texCoord_iter);
	}
	for( face_iter = faces.begin(); face_iter != faces.end(); face_iter++ ) {
		if (*face_iter) delete (*face_iter);
	}
	for( vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++ ) {
		if (*vertex_iter) delete (*vertex_iter);
	}
	for( skin_iter = skins.begin(); skin_iter != skins.end(); skin_iter++ ) {
		if (skin_iter->second) delete skin_iter->second;
	}
	glDeleteBuffersARB(4,glBuffers);
}

#define doInterpolation(a, b, t) (a)+(t)*((b)-(a))

void cMD3Object::draw(int index,int index2, float t, int mode){
	int current_index = index*mesh.numVertices;
	int next_index = index2*mesh.numVertices;
	if (texture){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,texture);
	}	
	vector<cMD3Face*>::iterator face_iter;
	float ambient[4] = {0,0,0,1};
	float diffuse[4] = {0.8f,0.8f,0.8f,1};
	float specular[4] = {1,1,1,1};
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glColor4f(1.0f,1.0f,1.0f,1.0f);

	drawBuffers(index,index2,t);
	if (texture){
		glDisable(GL_TEXTURE_2D);
	}
}

void cMD3Object::drawBuffers(int index,int index2, float t){
	glEnableClientState(GL_VERTEX_ARRAY);
	if (texture) glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, glBuffers[0]);
	
	float * buf = (float*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
	if (buf){
		int current_index = index*mesh.numVertices;
		int next_index = index2*mesh.numVertices;

		for (int i=0; i < mesh.numVertices; i++){
			cMD3Vertex * c_vertex1 = this->vertices.at(i+current_index);
			cMD3Vertex * c_vertex2 = this->vertices.at(i+next_index);

			for (int j=0; j<3; j++){
				buf[i*3+j] = doInterpolation(c_vertex1->vertex[j],c_vertex2->vertex[j],t);
			}

		}
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, glBuffers[0]);
	glVertexPointer(3, GL_FLOAT, 0,0);

	if (texture){
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, glBuffers[1]);
		glTexCoordPointer(2,GL_FLOAT, 0, 0);
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, glBuffers[2]);
	glNormalPointer(GL_FLOAT, 0, 0);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, glBuffers[3]);
	glDrawElements(GL_TRIANGLES,mesh.numTriangles*3,GL_UNSIGNED_INT,0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

	glDisableClientState(GL_NORMAL_ARRAY);
	if (texture) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}

const cMD3Mesh * cMD3Object::readObject( FILE * f ,int meshOffset){
	fread(&mesh,1,sizeof(cMD3Mesh),f);

	if (fseek(f, meshOffset+mesh.headerSize, SEEK_SET)!=0) throw(cException("Can't read skins"));
	readSkins(f);

	if (fseek(f, meshOffset + mesh.triStart, SEEK_SET)!=0) throw(cException("Can't read faces"));
	readFaces(f);

	if (fseek(f, meshOffset + mesh.uvStart, SEEK_SET)!=0) throw(cException("Can't read texture coords"));
	readTexCoords(f);

	if (fseek(f, meshOffset + mesh.vertexStart, SEEK_SET)!=0) throw(cException("Can't read vertices"));
	readVertices(f);
	return &mesh;
}

unsigned int createVBO(const void* data, int dataSize, GLenum target, GLenum usage){
	GLuint id = 0;  // 0 is reserved, glGenBuffersARB() will return non-zero id if success

	glGenBuffersARB(1, &id);                        // create a vbo
	glBindBufferARB(target, id);                    // activate vbo id to use
	glBufferDataARB(target, dataSize, data, usage); // upload data to video card

	// check data size in VBO is same as input array, if not return 0 and delete VBO
	int bufferSize = 0;
	glGetBufferParameterivARB(target, GL_BUFFER_SIZE_ARB, &bufferSize);
	if(dataSize != bufferSize)
	{
		glDeleteBuffersARB(1, &id);
		id = 0;
		fprintf(stderr,"Can't use VBO on array!\n");
	}

	return id;      // return VBO id
}

void cMD3Object::process(){
	int xyzs = mesh.numVertices;
	int uvs = mesh.numVertices;
	int norms = mesh.numVertices;
	int indices = faces.size()*mesh.numMeshFrames*3;
	buffer.prepare(xyzs, uvs, norms,indices);
	vector<cMD3Face*>::iterator face_iter;
	int A = 0, B= 0, C=0, D=0;
	int num_faces =  faces.size();

	for( int i = 0; i < mesh.numVertices; i++){
		cMD3Vertex * c_vertex = vertices.at(i);
		for (int j=0; j < 3; j++){
			buffer.xyz[A++] = c_vertex->vertex[j];
			buffer.norm[B++] = c_vertex->normal[j];
		}
		if ((i <texCoords.size()) && (i < mesh.numVertices)){
			cMD3TexCoord * c_texcoord = this->texCoords.at(i);
			for (int j=0; j < 2; j++){
				buffer.uv[C++] = c_texcoord->textureCoord[j];
			}
		}
	}
	for( face_iter = faces.begin(); face_iter != faces.end(); face_iter++ ) {
		for (int i=0; i<3; i++){
			buffer.index[D++] = (*face_iter)->vertexIndices[i];
		}
	}
	int max_elements = 0, max_vertices=0;
	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES,&max_vertices);
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES,&max_elements);
	//printf("Max vertices: %d\tMax indices: %d\n",max_vertices,max_elements);

	glBuffers[0] = createVBO(buffer.xyz,buffer.xyzs*sizeof(float),GL_ARRAY_BUFFER_ARB,GL_STREAM_DRAW_ARB);
	glBuffers[1] = createVBO(buffer.uv,buffer.uvs*sizeof(float),GL_ARRAY_BUFFER_ARB,GL_STATIC_DRAW_ARB);
	glBuffers[2] = createVBO(buffer.norm,buffer.norms*sizeof(float),GL_ARRAY_BUFFER_ARB,GL_STATIC_DRAW_ARB);
	glBuffers[3] = createVBO(buffer.index,buffer.indices*sizeof(unsigned int),GL_ELEMENT_ARRAY_BUFFER_ARB,GL_STATIC_DRAW_ARB);

}

cMD3VertexBuffer::cMD3VertexBuffer(){
	xyz = NULL;
	uv = NULL;
	norm = NULL;
	index = NULL;
	xyzs = 0;
	uvs = 0;
	norms = 0;
	indices = 0;
}

cMD3VertexBuffer::~cMD3VertexBuffer(){
	this->free();
}

void cMD3VertexBuffer::free(unsigned char what ){
	if (what&1 && xyz){
		delete[] xyz;
		xyz = NULL;
	}
	if (what&2 && uv){
		delete[] uv;
		uv = NULL;
	}
	if (what&4 && norm){
		delete[] norm;
		norm = NULL;
	}
	if (what&8 && index){
		delete[] index;
		index = NULL;
	}
}

void cMD3VertexBuffer::prepare( int num_xyz,int num_uv,int num_norm , int num_indices){
	if (xyz || uv || norm || index) this->free();
	xyzs = num_xyz*3;
	uvs = num_uv*2;
	norms = num_norm*3;
	indices = num_indices;
	if (!xyz) xyz = new float[xyzs];
	if (!uv) uv = new float[uvs];
	if (!norm) norm = new float[norms];
	if (!index) index = new unsigned int[indices];
}