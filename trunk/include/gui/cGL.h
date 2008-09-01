#ifndef H_CGL
#define H_CGL

#include "cChild.h"
#include "qDebug.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

class cGL : public cChild, public qDebug{
public:
	cGL();
	void initGL(int scr_width, int scr_height, int red_size=8, int green_size=8, int blue_size=8, int depth_size=16, int doublebuffer=1);
	void guitGL();
	SDL_Surface * surface;
};

void prepareGLSLFunctions();
void prepareMultiTexturingFunctions();
void prepareVBOfunctions();

extern PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgramObjectARB;
extern PFNGLDELETEOBJECTARBPROC				glDeleteObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC		glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC				glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC			glCompileShaderARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC		glGetObjectParameterivARB;
extern PFNGLATTACHOBJECTARBPROC				glAttachObjectARB;
extern PFNGLGETINFOLOGARBPROC				glGetInfoLogARB;
extern PFNGLLINKPROGRAMARBPROC				glLinkProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC			glUseProgramObjectARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC		glGetUniformLocationARB;
extern PFNGLUNIFORM1FARBPROC				glUniform1fARB;
extern PFNGLUNIFORM1IARBPROC				glUniform1iARB;
extern PFNGLUNIFORM2FARBPROC				glUniform2fARB;
extern PFNGLUNIFORM2IARBPROC				glUniform2iARB;
extern PFNGLUNIFORM3FARBPROC				glUniform3fARB;
extern PFNGLUNIFORM3IARBPROC				glUniform3iARB;
extern PFNGLUNIFORM4FARBPROC				glUniform4fARB;
extern PFNGLUNIFORM4IARBPROC				glUniform4iARB;
extern PFNGLDETACHOBJECTARBPROC				glDetachObjectARB;

extern PFNGLACTIVETEXTUREARBPROC			glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREPROC			glClientActivetexture;
extern PFNGLMULTITEXCOORD1DPROC				glMultitexCoord1d;
extern PFNGLMULTITEXCOORD1DVPROC			glMultitexCoord1dv;
extern PFNGLMULTITEXCOORD1FPROC				glMultitexCoord1;
extern PFNGLMULTITEXCOORD1FVPROC			glMultitexCoord1fv;
extern PFNGLMULTITEXCOORD1IPROC				glMultitexCoord1i;
extern PFNGLMULTITEXCOORD1IVPROC			glMultitexCoord1iv;
extern PFNGLMULTITEXCOORD1SPROC				glMultitexCoord1s;
extern PFNGLMULTITEXCOORD1SVPROC			glMultitexCoord1sv;
extern PFNGLMULTITEXCOORD2DPROC				glMultitexCoord2d;
extern PFNGLMULTITEXCOORD2DVPROC			glMultitexCoord2dv;
extern PFNGLMULTITEXCOORD2FPROC				glMultitexCoord2f;
extern PFNGLMULTITEXCOORD2FVPROC			glMultitexCoord2fv;
extern PFNGLMULTITEXCOORD2IPROC				glMultitexCoord2i;
extern PFNGLMULTITEXCOORD2IVPROC			glMultitexCoord2iv;
extern PFNGLMULTITEXCOORD2SPROC				glMultitexCoord2s;
extern PFNGLMULTITEXCOORD2SVPROC			glMultitexCoord2sv;
extern PFNGLMULTITEXCOORD3DPROC				glMultitexCoord3d;
extern PFNGLMULTITEXCOORD3DVPROC			glMultitexCoord3dv;
extern PFNGLMULTITEXCOORD3FPROC				glMultitexCoord3f;
extern PFNGLMULTITEXCOORD3FVPROC			glMultitexCoord3fv;
extern PFNGLMULTITEXCOORD3IPROC				glMultitexCoord3i;
extern PFNGLMULTITEXCOORD3IVPROC			glMultitexCoord3iv;
extern PFNGLMULTITEXCOORD3SPROC				glMultitexCoord3s;
extern PFNGLMULTITEXCOORD3SVPROC			glMultitexCoord3sv;
extern PFNGLMULTITEXCOORD4DPROC				glMultitexCoord4d;
extern PFNGLMULTITEXCOORD4DVPROC			glMultitexCoord4dv;
extern PFNGLMULTITEXCOORD4FPROC				glMultitexCoord4f;
extern PFNGLMULTITEXCOORD4FVPROC			glMultitexCoord4fv;
extern PFNGLMULTITEXCOORD4IPROC				glMultitexCoord4i;
extern PFNGLMULTITEXCOORD4IVPROC			glMultitexCoord4iv;
extern PFNGLMULTITEXCOORD4SPROC				glMultitexCoord4s;
extern PFNGLMULTITEXCOORD4SVPROC			glMultitexCoord4sv;
extern PFNGLLOADTRANSPOSEMATRIXFPROC		glLoadTransposeMatrixf;
extern PFNGLLOADTRANSPOSEMATRIXDPROC		glLoadTransposeMatrixd;
extern PFNGLMULTTRANSPOSEMATRIXFPROC		glMultTransposeMatrixf;
extern PFNGLMULTTRANSPOSEMATRIXDPROC		glMultTransposeMatrixd;
extern PFNGLSAMPLECOVERAGEPROC				glSampleCoverage;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC		glCompressedTexImage3D;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC		glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC		glCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC		glCompressedTexSubImage3D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC		glCompressedTexSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC		glCompressedTexSubImage1D;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC		glGetCompressedTexImage;
extern PFNGLBINDBUFFERARBPROC				glBindBufferARB;
extern PFNGLGENBUFFERSARBPROC				glGenBuffersARB;
extern PFNGLBUFFERDATAARBPROC				glBufferDataARB;
extern PFNGLGETBUFFERPARAMETERIVPROC		glGetBufferParameterivARB;
extern PFNGLDELETEBUFFERSARBPROC			glDeleteBuffersARB;
extern PFNGLMAPBUFFERARBPROC				glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC				glUnmapBufferARB;

#endif