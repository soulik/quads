#ifndef H_QIMAGE
#define H_QIMAGE

#include "qDebug.h"
#include "common.h"

#ifndef GLuint
typedef unsigned int GLuint;
#endif

#define IMG_TGA		1
#define IMG_JPEG	2

class qImage: public qDebug{
	friend int LoadTGA(const char *filename, qImage * pImageData);
	friend int LoadJPG(const char *filename, qImage * pImageData);
private:
	GLuint id;
	char * name;
	GLuint glGenerateTexture(unsigned int flags);
	unsigned int flags;

protected:
	int rowSpan;
	int channels;
	int width;
	int height;
	unsigned char *data;
public:
	qImage();
	~qImage();
	void createEmpty(float width, float height, int flags);
	int load(const char * fname, unsigned int flags);
	unsigned int getFlags();
	inline int getWidth(){
		return width;
	}
	inline int getHeight(){
		return height;
	}
	GLuint getID();
	uchar state;
};


#endif