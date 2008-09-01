#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "utils/3ds.h"
#include "errors/errors.h"
#include "errors/exception.h"
#include "objects/qVector.h"
#include "common.h"
#include "base.h"
#include "modules/script.h"

c3ds::c3ds(){
	debug = NULL;
}

c3ds::~c3ds(){
	map<string,xMaterial*>::iterator iter;
	for( iter = material.begin(); iter != material.end(); iter++ ) {
		if (iter->second) delete iter->second;
	}
	material.clear();
	for (unsigned int i=0; i<objects.size(); i++){
		delete objects[i];
	}
}

void c3ds::setDebug(qDebug * debug){
	this->debug = debug;
}

xObject::xObject(){
	vertices_count = 0;
	faces_count = 0;
	uvmap_count = 0;
	vertex = NULL;
	face = NULL;
	uvmap = NULL;
	hidden = 0;
}

xObject::~xObject(){
	delete[] vertex;
	delete[] face;
	delete[] uvmap;
	for (unsigned int i=0; i<face_mat.size(); i++){
		delete face_mat[i];
	}
}

int c3ds::readChunk(Chunk * chunk){
	int fr1,fr2;
	fr1 = fread(&(chunk->id),2,1,f);
	fr2 = fread(&(chunk->length),4,1,f);
	if ((fr1<1)||(fr2<1)){
		return 0;
	}
	return 1;
}

void c3ds::loadModel(const char * fname, const char * param){
	try{
		if (!fname) throw(cException("NULL fname"));
		f = fopen(fname,"rb");
		if (!f) throw(cException("Can't open file '%s'",fname));
		while (!feof(f)){
			readChunks();
		}
		fclose(f);
		for (unsigned int i=0; i<objects.size(); i++){
			objects[i]->assignFaceMaterials(this);
			objects[i]->computeNormals();
		}
		prepareMaterials();
	}catch(cException e){
		throw (e);
	}
}

void c3ds::readMain(Chunk * chunk){
	Chunk temp;
	long start = ftell(f);

	while (1){
		if (!readChunk(&temp)) return;
		long pos = ftell(f);
		switch (temp.id){
			case cM3D_VERSION:
				fread(&version,sizeof(unsigned long),1,f);
				break;
			case c3D_EDITOR:
				read3DEditor(&temp);
				break;
			default:
				skipChunk(&temp);
		}
		if ((ftell(f)-start+6)>=chunk->length) return;
	}
}

void c3ds::read3DEditor(Chunk * chunk){
	Chunk temp;
	long start = ftell(f);

	while (1){
		if (!readChunk(&temp)) return;
		long pos = ftell(f);
		switch (temp.id){
			case cMATERIAL:
				readMaterial(&temp);
				break;
			case cOBJECT_BLOCK:
				readObject(&temp);
				break;
			case cMESH_VERSION:
				fread(&mesh_version,sizeof(unsigned long),1,f);
				break;
			default:
				skipChunk(&temp);
		}
		if ((ftell(f)-start+6)>=chunk->length) return;
	}
}

void c3ds::readTriangularMesh(Chunk * chunk, xObject * co){
	long start = ftell(f);
	int i,j;
	while (1){
		Chunk temp;
		if (!readChunk(&temp)) return;
		long pos = ftell(f);
		switch (temp.id){
			case cVERTICES_LIST:
				if (co){
					fread(&co->vertices_count, sizeof(unsigned short),1, f);
					co->vertex = new xVertex[co->vertices_count];
					for (i=0; i < co->vertices_count; i++){
						fread(&co->vertex[i].x,sizeof(float),1,f);
						fread(&co->vertex[i].y,sizeof(float),1,f);
						fread(&co->vertex[i].z,sizeof(float),1,f);
					}
				}
				break;
			case cFACES_DESCRIPTION:
				readFacesDescription(&temp,co);
				break;
			case cFACES_COORDS:
				if (co){
					fread(&co->uvmap_count, sizeof(unsigned short),1, f);
					co->uvmap = new xCoord[co->uvmap_count];
					for (i=0; i < co->uvmap_count; i++){
						fread(&co->uvmap[i].u,sizeof(float),1,f);
						fread(&co->uvmap[i].v,sizeof(float),1,f);
					}
				}
				break;
			case cLOCAL_COORDS_SYTEM:
				for (j = 0; j < 4; j++){
					for (i = 0; i < 3; i++){
						fread(&(co->translation_matrix[j][i]),sizeof(float),1,f);
					}
				}
				co->translation_matrix[0][3] = 0;
				co->translation_matrix[1][3] = 0;
				co->translation_matrix[2][3] = 0;
				co->translation_matrix[3][3] = 1;
				/*
				debug->dprintf("Translation matrix for: %s\n",co->name);
				for (i = 0; i < 4; i++){
					debug->dprintf("| %6.2f %6.2f %6.2f %6.2f |\n",
						co->translation_matrix[i][0],
						co->translation_matrix[i][1],
						co->translation_matrix[i][2],
						co->translation_matrix[i][3]
					);
				}
				*/
				break;
			default:
				skipChunk(&temp);
		}
		if ((ftell(f)-start+6)>=chunk->length){
			return;
		}
	}
}

void c3ds::readObject(Chunk * chunk){
	long start = ftell(f);
	long counter = 0;
	xObject * co = new xObject();
	objects.push_back(co);
	readString(co->name,255);
	while (1){

		Chunk temp;
		if (!readChunk(&temp)) return;
		long pos = ftell(f);
		switch (temp.id){
			case cOBJECT_HIDDEN:
				co->hidden = 1;
				break;
			case cTRIANGULAR_MESH:
				readTriangularMesh(&temp,co);
				break;
			default:
				skipChunk(&temp);
		}
		if ((ftell(f)-start+6)>=chunk->length){
			return;
		}
	}
}

void c3ds::readFacesDescription(Chunk * chunk, xObject * co){
	long start = ftell(f);
	int counter,i;
	unsigned short num_fm,face_num;
	if (co){
		fread(&co->faces_count, sizeof(unsigned short),1, f);
		co->face = new xFace[co->faces_count];
		for (i=0; i < co->faces_count; i++){
			fread(&co->face[i].a,sizeof(unsigned short),1,f);
			fread(&co->face[i].b,sizeof(unsigned short),1,f);
			fread(&co->face[i].c,sizeof(unsigned short),1,f);
			fread(&co->face[i].flags,sizeof(unsigned short),1,f);
			co->face[i].material = NULL;
		}
		xFaceMat * cfm;
		while (1){
			Chunk temp;
			if (!readChunk(&temp)) return;
			long pos = ftell(f);
			switch (temp.id){
				case cFACES_MATERIAL:
					cfm = new xFaceMat;
					counter = 0;
					readString(cfm->name,255);
					fread(&num_fm,sizeof(unsigned short),1,f);
					for (i=0; i < num_fm; i++){
						fread(&face_num,sizeof(unsigned short),1,f);
						cfm->face.push_back(face_num);
					}
					co->face_mat.push_back(cfm);
					break;
				default:
					skipChunk(&temp);
			}
			if ((ftell(f)-start+6)>=chunk->length) return;
		}
	}
}

void c3ds::skipChunk(Chunk * chunk){
	fseek(f, chunk->length-6, SEEK_CUR);
	//debug->dprintf("Skipped chunk: 0x%04X with length: %dB\n",chunk->id,chunk->length);
}

void c3ds::readString(char * text, int max_len){
	int counter = 0;
	char tchar;
	do{
		fread(&tchar,1,1,f);
		text[counter] = tchar;
		counter++;
	}while(tchar != '\0' && counter<max_len);
}

void c3ds::readMap(Chunk * chunk, xMap * cmap){
	long start = ftell(f);
	cmap->tex_num = -1;
	while (1){
		Chunk temp;
		if (!readChunk(&temp)) return;
		long pos = ftell(f);
		switch (temp.id){
			case cMAPPING_FILENAME:
				readString(cmap->fname,255);
				break;
			case cU_SCALE:
				fread(&cmap->scale.u,sizeof(float),1,f);
				break;
			case cV_SCALE:
				fread(&cmap->scale.v,sizeof(float),1,f);
				break;
			case cU_OFFSET:
				fread(&cmap->offset.u,sizeof(float),1,f);
				break;
			case cV_OFFSET:
				fread(&cmap->offset.v,sizeof(float),1,f);
				break;
			default:
				skipChunk(&temp);
		}
		if ((ftell(f)-start+6)>=chunk->length) return;
	}
}

void c3ds::readMaterial(Chunk * chunk){
	long start = ftell(f);

	xMaterial * cm = new xMaterial;
	memset(cm->tex1_map.fname,0,255);
	memset(cm->tex2_map.fname,0,255);
	memset(cm->opacity_map.fname,0,255);
	memset(cm->reflection_map.fname,0,255);
	memset(cm->self_illum_map.fname,0,255);
	memset(cm->shininess_map.fname,0,255);
	memset(cm->specular_map.fname,0,255);
	
	while (1){
		Chunk temp;
		if (!readChunk(&temp)) return;
		long pos = ftell(f);
		switch (temp.id){
			case cMATERIAL_NAME:
				if (cm){
					readString(cm->name,255);
					material.insert(make_pair(cm->name,cm));
				}
				break;
			case cAMBIENT_COLOR:
				readColor(&temp,cm,'a');
			break;
			case cDIFFUSE_COLOR:
				readColor(&temp,cm,'d');
			break;
			case cSPECULAR_COLOR:
				readColor(&temp,cm,'s');
			break;
	
			case cTEX_MAP1:
				readMap(&temp,&cm->tex1_map);
				break;
			case cTEX_MAP2:
				readMap(&temp,&cm->tex2_map);
				break;
			default:
				skipChunk(&temp);
		}
		if ((ftell(f)-start+6)>=chunk->length) return;
	}
}

void c3ds::readColor(Chunk * chunk, xMaterial * cm, char mode){
	long start = ftell(f);

	while (1){
		Chunk temp;
		if (!readChunk(&temp)) return;
		long pos = ftell(f);
		switch (temp.id){
			case cRGBf:
				if (cm){
					switch (mode){
					case 'a':
						readRGBf(&cm->ambient_color.c);
						break;
					case 'd':
						readRGBf(&cm->diffuse_color.c);
						break;
					case 's':
						readRGBf(&cm->specular_color.c);
						break;
					}
				}
				break;
			case cRGBb:
				if (cm){
					switch (mode){
					case 'a':
						readRGBb(&cm->ambient_color.c);	
						break;
					case 'd':
						readRGBb(&cm->diffuse_color.c);
						break;
					case 's':
						readRGBb(&cm->specular_color.c);
						break;
					}
				}
				break;
			case cRGBGf:
				if (cm){
					switch (mode){
					case 'a':
						readRGBf(&cm->ambient_color.g);	
						break;
					case 'd':
						readRGBf(&cm->diffuse_color.g);
						break;
					case 's':
						readRGBf(&cm->specular_color.g);
						break;
					}
				}
				break;
			case cRGBGb:
				if (cm){
					switch (mode){
					case 'a':
						readRGBb(&cm->ambient_color.g);	
						break;
					case 'd':
						readRGBb(&cm->diffuse_color.g);
						break;
					case 's':
						readRGBb(&cm->specular_color.g);
						break;
					}
				}
				break;
			default:
				skipChunk(&temp);
		}
		if ((ftell(f)-start+6)>=chunk->length) return;
	}
}

void c3ds::readRGBb(xColor * color){
	fread(&color->b,sizeof(char),3,f);
	color->b[3] = 255;
	color->f[0] = float(color->b[0])/255.0f;
	color->f[1] = float(color->b[1])/255.0f;
	color->f[2] = float(color->b[2])/255.0f;
	color->f[3] = float(color->b[3])/255.0f;
}

void c3ds::readRGBf(xColor * color){
	fread(&color->f,sizeof(float),3,f);
	color->f[3] = 255;
	color->b[0] = char(color->b[0])*255;
	color->b[1] = char(color->b[1])*255;
	color->b[2] = char(color->b[2])*255;
	color->b[3] = char(color->b[3])*255;
}



void c3ds::readChunks(){
	Chunk temp;

	//read chunk
	if (!readChunk(&temp)) return;

	switch (temp.id){
		case cMAIN: //main chunk
			readMain(&temp);
			break;
		default:
			fseek(f, temp.length-6, SEEK_CUR);
	}
}

void xObject::assignFaceMaterials(c3ds * model){
	const char * mat_name;
	xMaterial * mat;
	unsigned int i,j;

	if (uvmap_count<1){
		hasTexture = 0;
	}else{
		hasTexture = 1;
	}
	for (i=0; i < face_mat.size(); i++){
		mat_name = face_mat[i]->name;
		if (mat_name){
			mat = model->material[mat_name];
			if (mat){
				for (j=0; j < face_mat[i]->face.size(); j++){
					face[face_mat[i]->face[j]].material = mat;
				}
			}
		}
	}
}

void xObject::computeNormals(){
	unsigned int i,j;
	xFace * _f;
	qVector v[3];
	for (i=0; i < faces_count; i++){
		_f = &(face[i]);
		v[0].set(vertex[_f->a].x,vertex[_f->a].y,vertex[_f->a].z);
		v[1].set(vertex[_f->b].x,vertex[_f->b].y,vertex[_f->b].z);
		v[2].set(vertex[_f->c].x,vertex[_f->c].y,vertex[_f->c].z);
		v[0] -= v[2];
		v[1] -= v[2];
		v[2] = v[0] | v[1];
		v[2] = v[2].normalize()*(-1);
		_f->x = v[2].x;
		_f->y = v[2].y;
		_f->z = v[2].z;
	}
	int count;
	for (i=0; i < vertices_count; i++){
		v[0].set(0,0,0);
		count = 0;
		for (j=0; j < faces_count; j++){
			_f = &(face[j]);
			if (_f->a==i || _f->b==i || _f->c==i){
				v[1].set(_f->x,_f->y,_f->z);
				v[0] += v[1];
				count++;
			}
		}
		v[0] = v[0] / (float)count;
		vertex[i].nx = v[0].x;
		vertex[i].ny = v[0].y;
		vertex[i].nz = v[0].z;
	}
}
void c3ds::prepareMaterials(){
	map<string,xMaterial*>::iterator iter;
	for( iter = material.begin(); iter != material.end(); iter++ ) {
		if (iter->second->tex1_map.fname && strlen(iter->second->tex1_map.fname)>0){
			try{
				iter->second->tex1_map.tex_num = __SCRIPT->call("loadTex",iter->second->tex1_map.fname,0x01);
			}catch (cException e){
				debug->dprintf("Can't load texture '%s' - %s\n",iter->second->tex1_map.fname,e.what());
				iter->second->tex1_map.tex_num = -1;
			}
		}
	}
}

void c3ds::drawObject(xObject * object, int mode){
	xFace * face;
	unsigned int b;

#define VX(VX) object->vertex[face->VX]

	//glLoadMatrixf((GLfloat*)object->translation_matrix);
	
	for (b=0; b < object->faces_count; b++){
		face = &(object->face[b]);
		if (face->material && !mode){
			glMaterialfv(GL_FRONT,GL_AMBIENT,face->material->ambient_color.c.f);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,face->material->diffuse_color.c.f);
			glMaterialfv(GL_FRONT,GL_SPECULAR,face->material->specular_color.c.f);
			if (face->material->tex1_map.tex_num>=0)
				glBindTexture(GL_TEXTURE_2D,face->material->tex1_map.tex_num);
		}
		glBegin(GL_TRIANGLES);
			if (face->material && !mode){
				glColor4f(face->material->diffuse_color.c.f[0],face->material->diffuse_color.c.f[1],face->material->diffuse_color.c.f[2],1);
			}else{
				glColor4f(1,1,1,1);
			}
			xCoord scale = {1.0f,-1.0f};
			/*
			if (face->material){
				scale = face->material->tex1_map.scale;
				if (scale.u==0) scale.u = 1;
				if (scale.v==0) scale.v = 1;
			}
			*/
			#define TX(VX) if (object->hasTexture) glTexCoord2f(object->uvmap[face->VX].u*scale.u, object->uvmap[face->VX].v*scale.v)

			if (!mode){
				TX(a);
				glNormal3f(VX(a).nx,VX(a).ny,VX(a).nz);
			}
			glVertex3f(-VX(a).x,VX(a).y,-VX(a).z);
			if (!mode){
				TX(b);
				glNormal3f(VX(b).nx,VX(b).ny,VX(b).nz);
			}
			glVertex3f(-VX(b).x,VX(b).y,-VX(b).z);
			if (!mode){
				TX(c);
				glNormal3f(VX(c).nx,VX(c).ny,VX(c).nz);
			}
			glVertex3f(-VX(c).x,VX(c).y,-VX(c).z);
		glEnd();
	}
}

void c3ds::drawModel(int mode){
	const c3ds * mdl = this;//getModel(name);
	unsigned int a;
	if (mdl){
		//updateMaterial(mdl, "Kryt");
		glFrontFace(GL_CCW);
		for (a=0; a < mdl->objects.size(); a++){
			drawObject(mdl->objects[a],mode);
		}
		glFrontFace(GL_CW);
	}
}

void c3ds::updateMaterial(const char * name){
	const qLdata * temp, * temp2;
	const c3ds * mdl = this;//getModel(modelname);
	qLDatap * color;
	if (mdl){
		map<string,xMaterial*>::const_iterator iter = mdl->material.find(name);
		if (iter != mdl->material.end()){
			if (temp = exists(LuaData,"models")){
				if (temp = exists(temp->_table,"nokia6100")){
					if (temp2 = exists(temp->_table,"d_color")){
						if (color = temp2->_table){
							iter->second->diffuse_color.c.f[0] = (float)(*color)["r"]._number;
							iter->second->diffuse_color.c.f[1] = (float)(*color)["g"]._number;
							iter->second->diffuse_color.c.f[2] = (float)(*color)["b"]._number;
							iter->second->diffuse_color.c.f[3] = (float)(*color)["a"]._number;
						}
					}
					if (temp2 = exists(temp->_table,"a_color")){
						if (color = temp2->_table){
							iter->second->ambient_color.c.f[0] = (float)(*color)["r"]._number;
							iter->second->ambient_color.c.f[1] = (float)(*color)["g"]._number;
							iter->second->ambient_color.c.f[2] = (float)(*color)["b"]._number;
							iter->second->ambient_color.c.f[3] = (float)(*color)["a"]._number;
						}
					}
				}
			}
		}
	}

}

void c3ds::setFrame(const char * obj_name, int num ){

}

void c3ds::setAnimation(const char * obj_name, const char * name ){

}

void c3ds::update(){

}