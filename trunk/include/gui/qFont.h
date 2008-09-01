#ifndef H_QFONT
#define H_QFONT

#define DEFAULT_FONT_WIDTH		16
#define DEFAULT_FONT_HEIGHT		16
#define DEFAULT_CHAR_WIDTH		0.7f
#define DEFAULT_CHAR_HEIGHT		1.0f
#define MAX_TEXT_SIZE			1024

#include "../utils/qRGBA.h"
#include "SDL_ttf.h"
#include <map>
#include <hash_map>
using namespace stdext;

struct glyph {
	unsigned int id;
	int w;
	int h;
	int w2;
	int h2;
	int minx;
	int maxx;
	int miny;
	int maxy;
	int advance;
};

class qFont {
protected:
	unsigned int texture_id;
	TTF_Font *font;
	void _drawChar(int character, float x, float y, float width, float height, qRGBA * color[4]);
	hash_map<unsigned long,glyph*> glyphs;
public :
	qFont();
	qFont(unsigned int id);
	~qFont();
	void setTexture(unsigned int id);
	void drawChar(int character, float x, float y, qRGBA color);
	void printf(float x, float y,  qRGBA color, const char * text, ...);
	void tprintf(float x, float y,float w, float h,  qRGBA color, const char * text, ...);
	void tprintfEx(float x, float y,  qRGBA color, const char * text, ...);
	void tglyph(float x, float y, float &w, float &h, glyph ** pd, qRGBA color, unsigned short character, int mode=0);
	glyph * getGlyph(unsigned short character);
	void loadFont(const char * fname, int size);
	void setStyle(int style);
	int getStyle();
	int getGlyphsCached();
};

#endif