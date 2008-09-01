#ifndef H_QRGBA
#define H_QRGBA

class qRGBA{
public:
	float r,g,b,a;
	qRGBA();
	qRGBA(unsigned int rgba);
	qRGBA(float r, float g, float b);
	qRGBA(float r, float g, float b, float a);
	void setRGBA(unsigned int rgba);
};

#endif