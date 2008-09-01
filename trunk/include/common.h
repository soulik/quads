#ifndef H_COMMON
#define H_COMMON

typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned char		uchar;
typedef unsigned long		ulong;

#define SCRIPT ((cScript*)parent->getModule(0))
#define TEXTURE ((qTexturer*)parent->getModule(2))
#define GAME ((cGame*)parent->getModule(3))
#define RENDER ((cRenderer*)parent->getModule(1))
#define CONTROL ((cControler*)parent->getModule(4))

#define __SCRIPT ((cScript*)__parent->getModule(0))
#define __TEXTURE ((qTexturer*)__parent->getModule(2))
#define __GAME ((cGame*)__parent->getModule(3))
#define __RENDER ((cRenderer*)__parent->getModule(1))
#define __CONTROL ((cControler*)__parent->getModule(4))

#endif