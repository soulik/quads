#ifndef H_QTEXTURER
#define H_QTEXTURER

#include "utils/qImage.h"
#include <vector>
#include <string>
#include <map>
#include <SDL/SDL.h>
#include "module.h"
#include "common.h"

using namespace std;

class qTexturer: public cModule {
protected:
	vector <qImage*> images;
	map <string, size_t> image_names;
	char * base_dir;
public:
	SDL_mutex * tex_mutex;
	qTexturer(cBase * parent);
	~qTexturer();
	unsigned int addImage(float width, float height, int flags, const char * name="unnamed");
	unsigned int loadImage(const char * fname, unsigned int flags);
	qImage * getImage(unsigned int id);
	qImage * getImage(const char * name);
	void setBaseDir(const char * dir);
	void init();
	void stop();
};

#endif