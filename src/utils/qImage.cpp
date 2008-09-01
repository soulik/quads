#include <stdlib.h>
#include <stdio.h>
#include <SDL/sdl.h>
#include <SDL/SDL_opengl.h>
#include <SDL_image.h>
#include "utils/qImage.h"
#include "errors/exception.h"

qImage::qImage(){
	id = 0;
	name = NULL;
	rowSpan = 0;
	channels = 0;
	width = 0;
	height = 0;
	data = NULL;
	flags = 0;
	setName("qImage");
	state = 0;
}

qImage::~qImage(){
	if (data) free(data);
	if (name) free(name);
	glDeleteTextures(1,&id);
}

void qImage::createEmpty(float width, float height, int flags){
	int res=0;
	id = glGenerateTexture(flags);
	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, (int)width, (int)height, 0, GL_RGBA,GL_UNSIGNED_BYTE, NULL);
	GLenum glE = glGetError();
	if (glE!=GL_NO_ERROR)
		printf("Can't build 2D Texture: %d: %s: %d: %s\n",res,gluErrorString(res),glE,gluErrorString(glE));
	state = 1;
	glFinish();
	
}

GLuint qImage::glGenerateTexture(unsigned int flags){
	GLuint id=0;
	glGenTextures(1,&id);
	glBindTexture(GL_TEXTURE_2D,id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	if (flags & 0x02){
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	}else{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	}
	  
	if (flags & 0x01){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	state = 1;
	glFinish();
	return id;
}

int qImage::load(const char * fname, unsigned int flags){
	int res=0;
	SDL_Surface * temp = IMG_Load(fname), * image = NULL;
	if (!temp) return -1;
	
	//image = SDL_CreateRGBSurface(0, temp->w, temp->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	SDL_PixelFormat pf;

	memcpy(&pf,temp->format,sizeof(SDL_PixelFormat));
		pf.Rmask = 0x000000ff;
		pf.Gmask = 0x0000ff00;
		pf.Bmask = 0x00ff0000;

	if (pf.BytesPerPixel>3){
		pf.Amask = 0xff000000;
	}

	image = SDL_ConvertSurface(temp,&pf,0);
	if (!image){
		SDL_FreeSurface(temp);
		return -1;
	}


	this->channels = image->format->BytesPerPixel;
	this->width = image->w;
	this->height = image->h;
	this->name = _strdup(fname);
	this->data = (unsigned char*)image->pixels;
	GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
	
	SDL_BlitSurface(temp, 0, image, 0);


	id = glGenerateTexture(flags);
	dprintf("%s - %dx%dx%d as %d\n",fname,width,height,channels*8,id);

	if (flags & 0x02){
		glTexImage2D(GL_TEXTURE_2D,0,channels, width, height, 0, format ,GL_UNSIGNED_BYTE, data);
		GLenum glE = glGetError();
		if (glE!=GL_NO_ERROR)
			dprintf("Can't build 2D Texture: %d: %s: %d: %s\n",res,gluErrorString(res),glE,gluErrorString(glE));
	}
	else{
		if ((res = gluBuild2DMipmaps(GL_TEXTURE_2D, channels, width, height, format, GL_UNSIGNED_BYTE, data))!=0){
			GLenum glE = glGetError();
			dprintf("Can't build 2D Mipmaps: %d: %s: %d: %s\n",res,gluErrorString(res),glE,gluErrorString(glE));
		}
	}
	glFinish();
	SDL_FreeSurface(image);
	SDL_FreeSurface(temp);
	data = NULL;
	this->flags = flags;
	state = 1;
	return 1;
}

unsigned int qImage::getFlags(){
	return this->flags;
}

GLuint qImage::getID(){
	if (!this) throw (cException("Image NULL"));
	return this->id;
}