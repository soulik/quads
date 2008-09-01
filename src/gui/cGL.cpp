#include "gui/cGL.h"
#include <SDL/SDL_opengl.h>
#include "module.h"

#define STDDBG (stderr)

PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgramObjectARB = NULL;
PFNGLDELETEOBJECTARBPROC				glDeleteObjectARB = NULL;
PFNGLCREATESHADEROBJECTARBPROC		glCreateShaderObjectARB = NULL;
PFNGLSHADERSOURCEARBPROC				glShaderSourceARB = NULL;
PFNGLCOMPILESHADERARBPROC			glCompileShaderARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC		glGetObjectParameterivARB = NULL;
PFNGLATTACHOBJECTARBPROC				glAttachObjectARB = NULL;
PFNGLGETINFOLOGARBPROC				glGetInfoLogARB = NULL;
PFNGLLINKPROGRAMARBPROC				glLinkProgramARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC			glUseProgramObjectARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC		glGetUniformLocationARB = NULL;
PFNGLUNIFORM1FARBPROC				glUniform1fARB = NULL;
PFNGLUNIFORM1IARBPROC				glUniform1iARB = NULL;
PFNGLUNIFORM2FARBPROC				glUniform2fARB = NULL;
PFNGLUNIFORM2IARBPROC				glUniform2iARB = NULL;
PFNGLUNIFORM3FARBPROC				glUniform3fARB = NULL;
PFNGLUNIFORM3IARBPROC				glUniform3iARB = NULL;
PFNGLUNIFORM4FARBPROC				glUniform4fARB = NULL;
PFNGLUNIFORM4IARBPROC				glUniform4iARB = NULL;
PFNGLDETACHOBJECTARBPROC				glDetachObjectARB = NULL;

PFNGLACTIVETEXTUREARBPROC			glActiveTextureARB = NULL;
PFNGLCLIENTACTIVETEXTUREPROC			glClientActivetexture = NULL;
PFNGLMULTITEXCOORD1DPROC				glMultitexCoord1d = NULL;
PFNGLMULTITEXCOORD1DVPROC			glMultitexCoord1dv = NULL;
PFNGLMULTITEXCOORD1FPROC				glMultitexCoord1 = NULL;
PFNGLMULTITEXCOORD1FVPROC			glMultitexCoord1fv = NULL;
PFNGLMULTITEXCOORD1IPROC				glMultitexCoord1i = NULL;
PFNGLMULTITEXCOORD1IVPROC			glMultitexCoord1iv = NULL;
PFNGLMULTITEXCOORD1SPROC				glMultitexCoord1s = NULL;
PFNGLMULTITEXCOORD1SVPROC			glMultitexCoord1sv = NULL;
PFNGLMULTITEXCOORD2DPROC				glMultitexCoord2d = NULL;
PFNGLMULTITEXCOORD2DVPROC			glMultitexCoord2dv = NULL;
PFNGLMULTITEXCOORD2FPROC				glMultitexCoord2f = NULL;
PFNGLMULTITEXCOORD2FVPROC			glMultitexCoord2fv = NULL;
PFNGLMULTITEXCOORD2IPROC				glMultitexCoord2i = NULL;
PFNGLMULTITEXCOORD2IVPROC			glMultitexCoord2iv = NULL;
PFNGLMULTITEXCOORD2SPROC				glMultitexCoord2s = NULL;
PFNGLMULTITEXCOORD2SVPROC			glMultitexCoord2sv = NULL;
PFNGLMULTITEXCOORD3DPROC				glMultitexCoord3d = NULL;
PFNGLMULTITEXCOORD3DVPROC			glMultitexCoord3dv = NULL;
PFNGLMULTITEXCOORD3FPROC				glMultitexCoord3f = NULL;
PFNGLMULTITEXCOORD3FVPROC			glMultitexCoord3fv = NULL;
PFNGLMULTITEXCOORD3IPROC				glMultitexCoord3i = NULL;
PFNGLMULTITEXCOORD3IVPROC			glMultitexCoord3iv = NULL;
PFNGLMULTITEXCOORD3SPROC				glMultitexCoord3s = NULL;
PFNGLMULTITEXCOORD3SVPROC			glMultitexCoord3sv = NULL;
PFNGLMULTITEXCOORD4DPROC				glMultitexCoord4d = NULL;
PFNGLMULTITEXCOORD4DVPROC			glMultitexCoord4dv = NULL;
PFNGLMULTITEXCOORD4FPROC				glMultitexCoord4f = NULL;
PFNGLMULTITEXCOORD4FVPROC			glMultitexCoord4fv = NULL;
PFNGLMULTITEXCOORD4IPROC				glMultitexCoord4i = NULL;
PFNGLMULTITEXCOORD4IVPROC			glMultitexCoord4iv = NULL;
PFNGLMULTITEXCOORD4SPROC				glMultitexCoord4s = NULL;
PFNGLMULTITEXCOORD4SVPROC			glMultitexCoord4sv = NULL;
PFNGLLOADTRANSPOSEMATRIXFPROC		glLoadTransposeMatrixf = NULL;
PFNGLLOADTRANSPOSEMATRIXDPROC		glLoadTransposeMatrixd = NULL;
PFNGLMULTTRANSPOSEMATRIXFPROC		glMultTransposeMatrixf = NULL;
PFNGLMULTTRANSPOSEMATRIXDPROC		glMultTransposeMatrixd = NULL;
PFNGLSAMPLECOVERAGEPROC				glSampleCoverage = NULL;
PFNGLCOMPRESSEDTEXIMAGE3DPROC		glCompressedTexImage3D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC		glCompressedTexImage2D = NULL;
PFNGLCOMPRESSEDTEXIMAGE1DPROC		glCompressedTexImage1D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC		glCompressedTexSubImage3D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC		glCompressedTexSubImage2D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC		glCompressedTexSubImage1D = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEPROC		glGetCompressedTexImage = NULL;
PFNGLBINDBUFFERARBPROC					glBindBufferARB = NULL;
PFNGLGENBUFFERSARBPROC					glGenBuffersARB = NULL;
PFNGLBUFFERDATAARBPROC					glBufferDataARB = NULL;
PFNGLGETBUFFERPARAMETERIVPROC		glGetBufferParameterivARB = NULL;
PFNGLDELETEBUFFERSARBPROC			glDeleteBuffersARB = NULL;
PFNGLMAPBUFFERARBPROC				glMapBufferARB = NULL;
PFNGLUNMAPBUFFERARBPROC				glUnmapBufferARB = NULL;

cGL::cGL(){
	setName("cGL");
}

void cGL::initGL(int scr_width, int scr_height, int red_size, int green_size, int blue_size, int depth_size, int doublebuffer){
	try{
		if (SDL_InitSubSystem(SDL_INIT_VIDEO)!=0){
			throw cException(SDL_GetError());
		}
		/*
		if (SDL_Init(SDL_INIT_VIDEO)!=0){
			throw cException(SDL_GetError());
		}
		*/
		dprintf("SDL_Init() VIDEO\n");

		// opengl pixel-mode setup
		
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, red_size );
	    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, green_size );
    	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, blue_size );
	    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, depth_size );
	    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, doublebuffer );
		dprintf("OpenGL Attributes set\n");
		
		// opengl video-mode
		
		dprintf("Trying to set video mode: %dx%dx%dx%d\n",scr_width, scr_height, red_size + green_size + blue_size + 8,depth_size);
		if (!(surface = SDL_SetVideoMode(scr_width, scr_height, red_size + green_size + blue_size + 8, SDL_OPENGL))){
			dprintf("Error\n");
			throw cException(SDL_GetError());
		}
		dprintf("SDL_SetVideoMode()\n");
		

		/* VIDEO MODE */


		//char * gl_ver = (char*)glGetString(GL_VERSION);
		//dprintf("OpenGL version: %s\n",gl_ver);
		
		glClearColor(0,0,0.25f,1);
		glShadeModel(GL_SMOOTH);
		
		glFrontFace(GL_CW);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glPointSize(3.0f);
		glEnable(GL_POINT_SMOOTH);
		
		glViewport(0,0,scr_width,scr_height);
		
		dprintf("OpenGL settings loaded\n");
		dprintf("------------------------\n");
	}DEFAULT_EXCEPTION(STDDBG)
}

void cGL::guitGL(){
	//SDL_Quit();
	//printf("SDL_Quit()\n");
}

void prepareGLSLFunctions(){
	glCreateProgramObjectARB	= (PFNGLCREATEPROGRAMOBJECTARBPROC) SDL_GL_GetProcAddress("glCreateProgramObjectARB");
	glDeleteObjectARB			= (PFNGLDELETEOBJECTARBPROC) SDL_GL_GetProcAddress("glDeleteObjectARB");
	glCreateShaderObjectARB		= (PFNGLCREATESHADEROBJECTARBPROC) SDL_GL_GetProcAddress("glCreateShaderObjectARB");
	glShaderSourceARB			= (PFNGLSHADERSOURCEARBPROC) SDL_GL_GetProcAddress("glShaderSourceARB");
	glCompileShaderARB			= (PFNGLCOMPILESHADERARBPROC) SDL_GL_GetProcAddress("glCompileShaderARB");
	glGetObjectParameterivARB	= (PFNGLGETOBJECTPARAMETERIVARBPROC) SDL_GL_GetProcAddress("glGetObjectParameterivARB");
	glAttachObjectARB			= (PFNGLATTACHOBJECTARBPROC) SDL_GL_GetProcAddress("glAttachObjectARB");
	glGetInfoLogARB				= (PFNGLGETINFOLOGARBPROC) SDL_GL_GetProcAddress("glGetInfoLogARB");
	glLinkProgramARB			= (PFNGLLINKPROGRAMARBPROC) SDL_GL_GetProcAddress("glLinkProgramARB");
	glUseProgramObjectARB		= (PFNGLUSEPROGRAMOBJECTARBPROC) SDL_GL_GetProcAddress("glUseProgramObjectARB");
	glGetUniformLocationARB		= (PFNGLGETUNIFORMLOCATIONARBPROC) SDL_GL_GetProcAddress("glGetUniformLocationARB");
	glUniform1fARB				= (PFNGLUNIFORM1FARBPROC) SDL_GL_GetProcAddress("glUniform1fARB");
	glUniform1iARB				= (PFNGLUNIFORM1IARBPROC) SDL_GL_GetProcAddress("glUniform1iARB");
	glUniform2fARB				= (PFNGLUNIFORM2FARBPROC) SDL_GL_GetProcAddress("glUniform2fARB");
	glUniform2iARB				= (PFNGLUNIFORM2IARBPROC) SDL_GL_GetProcAddress("glUniform2iARB");
	glUniform3fARB				= (PFNGLUNIFORM3FARBPROC) SDL_GL_GetProcAddress("glUniform3fARB");
	glUniform3iARB				= (PFNGLUNIFORM3IARBPROC) SDL_GL_GetProcAddress("glUniform3iARB");
	glUniform4fARB				= (PFNGLUNIFORM4FARBPROC) SDL_GL_GetProcAddress("glUniform4fARB");
	glUniform4iARB				= (PFNGLUNIFORM4IARBPROC) SDL_GL_GetProcAddress("glUniform4iARB");
	glDetachObjectARB			= (PFNGLDETACHOBJECTARBPROC) SDL_GL_GetProcAddress("glDetachObjectARB");
}

void prepareMultiTexturingFunctions(){
	glActiveTextureARB			= (PFNGLACTIVETEXTUREARBPROC) SDL_GL_GetProcAddress("glActiveTextureARB");
	glClientActivetexture		= (PFNGLCLIENTACTIVETEXTUREPROC) SDL_GL_GetProcAddress("glClientActivetexture");
	glMultitexCoord2f			= (PFNGLMULTITEXCOORD2FPROC) SDL_GL_GetProcAddress("glMultitexCoord2f");
	glMultitexCoord2fv			= (PFNGLMULTITEXCOORD2FVPROC) SDL_GL_GetProcAddress("glMultitexCoord2fv");
}

void prepareVBOfunctions(){
	glBindBufferARB				= (PFNGLBINDBUFFERARBPROC) SDL_GL_GetProcAddress("glBindBufferARB");
	glGenBuffersARB				= (PFNGLGENBUFFERSARBPROC) SDL_GL_GetProcAddress("glGenBuffersARB");
	glBufferDataARB				= (PFNGLBUFFERDATAARBPROC) SDL_GL_GetProcAddress("glBufferDataARB");
	glGetBufferParameterivARB	= (PFNGLGETBUFFERPARAMETERIVPROC) SDL_GL_GetProcAddress("glGetBufferParameterivARB");
	glDeleteBuffersARB			= (PFNGLDELETEBUFFERSARBPROC) SDL_GL_GetProcAddress("glDeleteBuffersARB");
	glMapBufferARB				= (PFNGLMAPBUFFERARBPROC)SDL_GL_GetProcAddress("glMapBufferARB");
	glUnmapBufferARB			= (PFNGLUNMAPBUFFERARBPROC)SDL_GL_GetProcAddress("glUnmapBufferARB");
}