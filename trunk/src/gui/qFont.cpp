#include "gui/qFont.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <math.h>
#include <stdio.h>
#include "errors/exception.h"
#include "common.h"
#include "base.h"
#include "modules/renderer.h"
#include "modules/script.h"

int round(double x)
{
	return (int)(x + 0.5);
}
int nextpoweroftwo(int x)
{
	double logbase2 = log((float)x) / log(2.0f);
	return round(pow(2,ceil(logbase2)));
}

qFont::qFont(){
	texture_id = 0;
	TTF_Init();
	font = NULL;
}

qFont::qFont(unsigned int id){
	setTexture(id);
	TTF_Init();
	font = NULL;
}

qFont::~qFont(){
	hash_map<unsigned long,glyph*>::iterator iter;
	for( iter = glyphs.begin(); iter != glyphs.end(); iter++ ) {
		glDeleteTextures(1, &iter->second->id);	
		delete iter->second;
	}

	if (font){
			TTF_CloseFont(font);
	}
	TTF_Quit();
}

void qFont::loadFont(const char * fname, int size){
	try{
		if (font){
			TTF_CloseFont(font);
		}
		font = TTF_OpenFont(fname,size);
		if (!font)	throw cException("Can't open font");
	}catch (cException e){
		throw e;
	}
}

void qFont::setTexture(unsigned int id){
	texture_id = id;
}

void qFont::_drawChar(int character, float x, float y, float width, float height, qRGBA * color[4]){
	float fx,fy,fw,fh;

	fw = 16.0f/256.0f;
	fh = 16.0f/256.0f;

	fx = float(character%16)*fw;
	fy = 1.0f - (float)floor(float(character)/16.0f)*fh;

	glBegin(GL_QUADS);
		glColor4f(color[0]->r,color[0]->g,color[0]->b,color[0]->a);
		glTexCoord2f(fx,fy);
		glVertex2f((float)x,(float)y);

		glColor4f(color[1]->r,color[1]->g,color[1]->b,color[1]->a);
		glTexCoord2f( fx+fw,fy);
		glVertex2f((float)(x+width),(float)y);

		glColor4f(color[2]->r,color[2]->g,color[2]->b,color[2]->a);
		glTexCoord2f(fx+fw,fy-fh);
		glVertex2f((float)(x+width),(float)(y+height));

		glColor4f(color[3]->r,color[3]->g,color[3]->b,color[3]->a);
		glTexCoord2f(fx,fy-fh);
		glVertex2f((float)x,(float)(y+height));
	glEnd();

}

void qFont::drawChar(int character, float x, float y, qRGBA color){
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	qRGBA * _color[4] = {&color,&color,&color,&color};

	_drawChar(character,x,y,DEFAULT_FONT_WIDTH, DEFAULT_FONT_HEIGHT,_color);
	
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
 	glDisable(GL_BLEND);
}

void qFont::printf(float x, float y, qRGBA color,const char * fmt, ...){
	char text[MAX_TEXT_SIZE];
	DWORD pos;
	int width = 16,height = 16;
	
	memset(text,0,MAX_TEXT_SIZE);

	va_list ap;
	if (fmt==NULL) return;
	va_start(ap,fmt);
	vsprintf(text,fmt,ap);
	va_end(ap);

	qRGBA * _color[4] = {&color,&color,&color,&color};

	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	float posx=0,posy=0;
	char seq[16]={};
	char seq_num=0;

	for(pos=0;pos<strlen(text);pos++){
		unsigned int nval=0;
		switch (text[pos]){
			case '\n':
					posx=0;
					posy+=(height*DEFAULT_CHAR_HEIGHT);
					continue;
				break;
			case '\001':
					memcpy(seq,text+pos+1,8);
					seq[8]=0;
					if (sscanf(seq,"%X",&nval)){
						color.setRGBA(nval);
						pos+=8;
					}
					continue;
				break;
				
			case '\002':
					memcpy(seq,text+pos+1,4);
					seq[8]=0;
					
					if (sscanf(seq,"%X",&nval)){
						width = nval;
						height = nval;
						pos+=4;
					}
					continue;
				break;
		}

		_drawChar(text[pos],float(x+posx),float(y+posy),float(width),float(height),_color);
		posx+=(width*DEFAULT_CHAR_WIDTH);
	}
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

void qFont::tprintf(float x, float y, float _w,float _h, qRGBA color,const char * fmt, ...){
	try {
		if (!font){
			throw cException("Font is not loaded");
		}
		if (!fmt){
			throw cException("NULL text");
		}
		char text[MAX_TEXT_SIZE];
		memset(text,0,MAX_TEXT_SIZE);

		va_list ap;
		if (fmt==NULL) return;
		va_start(ap,fmt);
		vsprintf(text,fmt,ap);
		va_end(ap);
	
		//SDL_Color _color={(Uint8)color.r,(Uint8)color.g,(Uint8)color.b};
		SDL_Color _color={255,255,255};
		SDL_Surface *text_surface;
		SDL_Surface *temp;

		int h,w;
		GLuint texture;
	
		if(!(text_surface=TTF_RenderUTF8_Blended(font,text,_color))) {
		//if (!(text_surface = TTF_RenderUTF8_Solid(font,text,_color))){
			throw cException("Can't draw TTF text");
		} else {
			w = nextpoweroftwo(text_surface->w);
			h = nextpoweroftwo(text_surface->h);
	
			temp = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00FFFFFF);

			if (SDL_BlitSurface(text_surface, 0, temp, 0)<0){
				throw cException("Can't blit TTF surface");
			}
			
			/* Tell GL about our new texture */
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels );
	
			/* GL_NEAREST looks horrible, if scaled... */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

			/* prepare to render our texture */
			//glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA);
			glEnable(GL_BLEND);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);
	
			/* Draw a quad at location */
			glBegin(GL_QUADS);
				glColor4f(color.r, color.g, color.b, color.a);
				glTexCoord2f(0.0f, -1.0f); 
					glVertex2f(x    , y);
				glTexCoord2f(1.0f, -1.0f); 
					glVertex2f(x + w*_w, y);
				glTexCoord2f(1.0f, 0.0f); 
					glVertex2f(x + w*_w, y + h*_h);
				glTexCoord2f(0.0f, 0.0f); 
					glVertex2f(x    , y + h*_h);
			glEnd();
			
			/* Bad things happen if we delete the texture before it finishes */
			glFinish();
			
			/* return the deltas in the unused w,h part of the rect */
			//w = text_surface->w;
			//h = text_surface->h;
			
			/* Clean up */
			SDL_FreeSurface(text_surface);
			SDL_FreeSurface(temp);
			glDeleteTextures(1, &texture);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA,GL_ONE);

		}
	}catch (cException e){
		throw (e);
	}
}

void qFont::tprintfEx(float x, float y, qRGBA color,const char * fmt, ...){
	unsigned char text[MAX_TEXT_SIZE];
	DWORD pos;
	
	va_list ap;
	if (fmt==NULL) return;
	va_start(ap,fmt);
	vsprintf((char*)text,fmt,ap);
	va_end(ap);

	qRGBA * _color[4] = {&color,&color,&color,&color};

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	float posx=0,posy=0;
	float prev_width=0, prev_height=0;
	char seq[16]={};
	char seq_num=0;
	glyph * p_glyph=NULL;
	y += TTF_FontAscent(font);

	for(pos=0;pos<strlen((const char*)text);pos++){
		float width = 1,height = 1;
		unsigned int nval=0;
		switch (text[pos]){
			case '\n':
					posx=0;
					posy+=TTF_FontLineSkip(font);
					continue;
				break;
			case '\001':
					memcpy(seq,text+pos+1,8);
					seq[8]=0;
					if (sscanf(seq,"%X",&nval)){
						color.setRGBA(nval);
						pos+=8;
					}
					continue;
				break;
				
			case '\002':
					memcpy(seq,text+pos+1,4);
					seq[8]=0;
					
					if (sscanf(seq,"%X",&nval)){
						width = (float)nval;
						height = (float)nval;
						pos+=4;
					}
					continue;
				break;
		}
		//convert UTF-8 characters to unicode
		unsigned short character = text[pos];
		if (text[pos] >= 0xF0) {
			character  =  (text[pos]&0x07) << 18;
			character |=  (text[++pos]&0x3F) << 12;
			character |=  (text[++pos]&0x3F) << 6;
			character |=  (text[++pos]&0x3F);
		} else
		if (text[pos] >= 0xE0) {
			character  =  (text[pos]&0x0F) << 12;
			character |=  (text[++pos]&0x3F) << 6;
			character |=  (text[++pos]&0x3F);
		} else
		if (text[pos]>=0xC0){
			character = (text[pos]&0x1F) << 6;
			character |= (text[++pos]&0x3F);
		}
		

		tglyph(float(x+posx),float(y+posy),width,height,&p_glyph,*(_color[0]),character,1);
		prev_width = width;
		prev_height = height;
		if (p_glyph){
			posx += p_glyph->advance;
		}
	}
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

void qFont::tglyph(float x, float y, float & _w,float & _h, glyph ** pd, qRGBA color, unsigned short character, int mode){
	try {
		if (!font){
			throw cException("Font is not loaded");
		}
	
		SDL_Color _color={255,255,255};
		unsigned long characterws = character + (getStyle()<<0x10);
		*pd = getGlyph(characterws);
		
		if (*pd){
			if (mode==0){
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
			}

			glBindTexture(GL_TEXTURE_2D, (*pd)->id);
		
			x += (*pd)->minx;
			y -= (*pd)->maxy;

			glBegin(GL_QUADS);
				glColor4f(color.r, color.g, color.b, color.a);
				glTexCoord2f(0.0f, 0.0f); 
				glVertex2f(x    , y);
				glTexCoord2f(1.0f, 0.0f); 
				glVertex2f(x + (*pd)->w*_w, y);
				glTexCoord2f(1.0f, 1.0f); 
				glVertex2f(x + (*pd)->w*_w, y + (*pd)->h*_h);
				glTexCoord2f(0.0f, 1.0f); 
				glVertex2f(x    , y + (*pd)->h*_h);
			glEnd();
		
			if (mode==0){
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_BLEND);
				glFinish();
			}
		}
	}catch (cException e){
			throw (e);
	}
}

glyph * qFont::getGlyph( unsigned short character ){
	GLuint texture;
	SDL_Surface *text_surface;
	SDL_Surface *temp;
	glyph * p_glyph=NULL;
	int h=0,w=0;
	SDL_Color _color={255,255,255};

	hash_map<unsigned long, glyph*>::iterator iter = glyphs.find(character);

	if (iter == glyphs.end()){
		if(!(text_surface=TTF_RenderGlyph_Blended(font, character,_color))) {
			throw cException("Can't draw TTF glyph");
		} else {
			w = nextpoweroftwo(text_surface->w);
			h = nextpoweroftwo(text_surface->h);
			temp = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
			if (SDL_BlitSurface(text_surface, 0, temp, 0)<0){
				throw cException("Can't blit TTF surface");
			}
		}
		glGenTextures(1, &texture);
		p_glyph = new glyph;
		p_glyph->id = texture;
		p_glyph->w = w;
		p_glyph->h = h;
		p_glyph->w2 = text_surface->w;
		p_glyph->h2= text_surface->h;
		TTF_GlyphMetrics(font,character,&p_glyph->minx,&p_glyph->maxx,&p_glyph->miny,&p_glyph->maxy,&p_glyph->advance);
		glyphs.insert(make_pair(character,p_glyph));

		/* Tell GL about our new texture */
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels );

		/* GL_NEAREST looks horrible, if scaled... */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
		/* return the deltas in the unused w,h part of the rect */

		SDL_FreeSurface(text_surface);
		SDL_FreeSurface(temp);
	}else{
		return iter->second;
	}
}

void qFont::setStyle(int style){
	try{
		if (!font) throw cException("TTF not loaded!");
		TTF_SetFontStyle(font,style);
	}catch (cException e){
		throw (e);
	}
}

int qFont::getStyle(){
	try{
		if (!font) throw cException("TTF not loaded!");
		return TTF_GetFontStyle(font);
	}catch (cException e){
		throw (e);
	}
	return 0;
}

int qFont::getGlyphsCached(){
	return glyphs.size();
}