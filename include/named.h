#ifndef H_NAMED
#define H_NAMED

class cNamed {
protected:
	char * name;
public:
	cNamed();
	~cNamed();
	const char * getName();
	void setName(const char * _name);
};

#endif