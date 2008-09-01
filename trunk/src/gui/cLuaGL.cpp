#include "gui/cGL.h"
#include "gui/cLuaGL.h"
#include <SDL/SDL_opengl.h>
#include "modules/renderer.h"
#include "modules/qTexturer.h"
#include "modules/script.h"
#include "common.h"
#include "base.h"

#define	SELECT_BUF_SIZE	512

extern cBase * __parent;

GLuint	selectBuffer[SELECT_BUF_SIZE];

static void setTableValuei(lua_State * L, const char * name, int value){
	lua_pushstring(L,name);
	lua_pushinteger(L,value);
	lua_settable(L,-3);
}

static void setTableValuef(lua_State * L, const char * name, float value){
	lua_pushstring(L,name);
	lua_pushnumber(L,value);
	lua_settable(L,-3);
}

static void setTableValues(lua_State * L, const char * name, const char * value){
	lua_pushstring(L,name);
	lua_pushstring(L,value);
	lua_settable(L,-3);
}

static int gl_GetImage(lua_State * L){
	try{
		qImage * image = __TEXTURE->getImage(lua_tostring(L,1));
		if (image){
			lua_newtable(L);
			setTableValuei(L,"id",image->getID());
			setTableValues(L,"name",image->getName());
			setTableValuei(L,"flags",image->getFlags());
			setTableValuei(L,"width",image->getWidth());
			setTableValuei(L,"height",image->getHeight());
			//lua_settable(L,-6);
		}else{
			lua_pushnil(L);
		}
		return 1;
	}catch (cException e) {
		fprintf(stderr,"%s",e.what());
		return 0;
	}
}

static void processHits (GLint hits, GLuint buffer[]){
	unsigned int i, j;
	GLuint names, *ptr, minZ,*ptrNames=NULL, numberOfNames=0;
	if (!buffer) return;
	ptr = (GLuint *) buffer;//=NULL;
	minZ = 0xffffffff;
	for (i = 0; i < (unsigned int)hits; i++) {	
		names = *ptr;
		ptr++;
		if (*ptr < minZ) {
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr+2;
		}
		ptr += names+2;
	}
	ptr = ptrNames;
	for (j = 0; j < numberOfNames; j++,ptr++) {
		__SCRIPT->call("pickedObject",*ptr);
	}
}

static int startPicking(lua_State * L){
	GLint viewport[4];
	GLdouble cursorX = lua_tonumber(L,1);
	GLdouble cursorY = lua_tonumber(L,2);

	glSelectBuffer(SELECT_BUF_SIZE,selectBuffer);
	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glGetIntegerv(GL_VIEWPORT,viewport);
	gluPickMatrix(cursorX,viewport[3]-cursorY,
			5,5,viewport);
	
	//gluPerspective(45,(GLfloat) (viewport[2]-viewport[0])/(GLfloat) (viewport[3]-viewport[1]),0.1,1000);
	
	//__RENDER->drawPlayerView();
	//glMatrixMode(GL_MODELVIEW);
	return 0;
}

static int stopPicking(lua_State * L){
	int hits;
	// restoring the original projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	// returning to normal rendering mode
	hits = glRenderMode(GL_RENDER);
	// if there are hits process them
	if (hits != 0){
		processHits(hits,selectBuffer);
	}
	return 0;
}

static int gl_MatrixMode(lua_State * L){
	glMatrixMode((GLuint)lua_tonumber(L,1));
	return 0;
}
static int gl_LoadIdentity(lua_State * L){
	glLoadIdentity();
	return 0;
}
static int gl_Viewport(lua_State * L){
	glViewport((GLuint)lua_tonumber(L,1),(GLuint)lua_tonumber(L,2),(GLuint)lua_tonumber(L,3),(GLuint)lua_tonumber(L,4));
	return 0;
}
static int gl_Ortho(lua_State * L){
	glOrtho((GLdouble)lua_tonumber(L,1),(GLdouble)lua_tonumber(L,2),(GLdouble)lua_tonumber(L,3),(GLdouble)lua_tonumber(L,4),(GLdouble)lua_tonumber(L,5),(GLdouble)lua_tonumber(L,6));
	return 0;
}
static int gl_FrontFace(lua_State * L){
	glFrontFace((GLuint)lua_tonumber(L,1));
	return 0;
}
static int gl_ClearColor(lua_State * L){
	glClearColor((GLclampf)lua_tonumber(L,1),(GLclampf)lua_tonumber(L,2),(GLclampf)lua_tonumber(L,3),(GLclampf)lua_tonumber(L,4));
	return 0;
}
static int gl_Clear(lua_State * L){
	glClear((GLuint)lua_tonumber(L,1));
	return 0;
}
static int gl_Scalef(lua_State * L){
	glScalef((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3));
	return 0;
}
static int gl_Rotatef(lua_State * L){
	glRotatef((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3),(GLfloat)lua_tonumber(L,4));
	return 0;
}
static int gl_Translatef(lua_State * L){
	glTranslatef((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3));
	return 0;
}

static int gl_CopyTexSubImage2D(lua_State * L){
	glCopyTexSubImage2D((GLuint)lua_tonumber(L,1),(GLuint)lua_tonumber(L,2),(GLuint)lua_tonumber(L,3),(GLuint)lua_tonumber(L,4),(GLuint)lua_tonumber(L,5),(GLuint)lua_tonumber(L,6),(GLuint)lua_tonumber(L,7),(GLuint)lua_tonumber(L,8));
	return 0;
}

static int gl_BindTexture(lua_State * L){
	glBindTexture((GLuint)lua_tonumber(L,1),(GLuint)lua_tonumber(L,2));
	return 0;
}

static int gl_Enable(lua_State * L){
	glEnable((GLuint)lua_tonumber(L,1));
	return 0;
}

static int gl_Disable(lua_State * L){
	glDisable((GLuint)lua_tonumber(L,1));
	return 0;
}

static int gl_TexParameteri(lua_State * L){
	glTexParameteri((GLuint)lua_tonumber(L,1),(GLuint)lua_tonumber(L,2),(GLint)lua_tonumber(L,3));
	return 0;
}
static int gl_BlendFunc(lua_State * L){
	glBlendFunc((GLuint)lua_tonumber(L,1),(GLuint)lua_tonumber(L,2));
	return 0;
}
static int gl_Begin(lua_State * L){
	uint temp= (GLuint)lua_tonumber(L,1);
	glBegin(temp);
	return 0;
}
static int gl_End(lua_State * L){
	glEnd();
	return 0;
}

static int gl_EndList(lua_State * L){
	glEndList();
	return 0;
}

static int gl_Color3f(lua_State * L){
	glColor3f((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3));
	return 0;
}

static int gl_Color4f(lua_State * L){
	glColor4f((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3),(GLfloat)lua_tonumber(L,4));
	return 0;
}
static int gl_Vertex2f(lua_State * L){
	glVertex2f((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2));
	return 0;
}
static int gl_Vertex2i(lua_State * L){
	glVertex2i((GLint)lua_tonumber(L,1),(GLint)lua_tonumber(L,2));
	return 0;
}
static int gl_Vertex3f(lua_State * L){
	glVertex3f((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3));
	return 0;
}

static int gl_TexCoord2f(lua_State * L){
	glTexCoord2f((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2));
	return 0;
}
static int gl_PrepareOrthogonal(lua_State * L){
	__RENDER->prepareOrthogonal();
	return 0;
}

static int gl_printf(lua_State * L){
	__RENDER->font->printf((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),qRGBA((uint)lua_tonumber(L,3)),lua_tostring(L,4));
	return 0;
}

static int gl_tprintfEx(lua_State * L){
	__RENDER->font->tprintfEx((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),qRGBA((uint)lua_tonumber(L,3)),lua_tostring(L,4));
	return 0;
}

static int gl_tprintf(lua_State * L){
	qFont * _font = __RENDER->font;
	try{
		_font->tprintf((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3),(GLfloat)lua_tonumber(L,4),qRGBA((uint)lua_tonumber(L,5)),lua_tostring(L,6));
	}catch (cException e){
		__RENDER->dprintf("Exception: %s",e.what());
	}
	return 0;
}

static int gl_tglyph(lua_State * L){
	qFont * _font = __RENDER->font;
	float w = (GLfloat)lua_tonumber(L,3);
	float h = (GLfloat)lua_tonumber(L,4);

	try{
		//_font->tglyph((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),w,h,(GLfloat)lua_tonumber(L,5),qRGBA((uint)lua_tonumber(L,6)),(unsigned short)lua_tointeger(L,7));
	}catch (cException e){
		__RENDER->dprintf("Exception: %s",e.what());
	}
	lua_pushnumber(L,w);
	lua_pushnumber(L,h);
	return 2;
}

static int gl_load_font(lua_State * L){
	qFont * _font = __RENDER->font;
	_font->loadFont(lua_tostring(L,1),(int)lua_tonumber(L,2));
	return 0;
}

static int gl_Accum(lua_State * L){
	glAccum((GLuint)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2));
	return 0;
}

static int gl_AreTexturesResident(lua_State * L){
	//glAreTextureResident((GLuint)lua_tonumber(L,1),(GLuint)lua_tonumber(L,2));
	__RENDER->dprintf("Stub(%s)","glAreTexturesResident()");
	return 0;
}

static int gl_AlphaFunc(lua_State * L){
	glAlphaFunc((GLenum)lua_tonumber(L,1),(GLclampf)lua_tonumber(L,2));
	return 0;
}
static int gl_ArrayElement(lua_State * L){
	glArrayElement((GLint)lua_tonumber(L,1));
	return 0;
}
static int gl_BeginCurve(lua_State * L){
	//glBeginCurve(
	__RENDER->dprintf("Stub(%s)","glBeginCurve()");
	return 0;
}
static int gl_BeginPolygon(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glBeginPolygon()");
	return 0;
}
static int gl_BeginSurface(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glBeginSurface()");
	return 0;
}
static int gl_BeginTrim(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glBeginTrim()");
	return 0;
}
static int gl_Bitmap(lua_State * L){
	//glBitmap(
	__RENDER->dprintf("Stub(%s)","glBeginBitmap()");
	return 0;
}
static int gl_BlendColorEXT(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glBlendColorEXT()");
	return 0;
}
static int glu_Build1DMipmaps(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glBuild1DMipmaps()");
	return 0;
}
static int glu_Build2DMipmaps(lua_State * L){
	int ret = gluBuild2DMipmaps((GLenum)lua_tonumber(L,1),(GLint)lua_tonumber(L,2),(GLint)lua_tonumber(L,3),(GLint)lua_tonumber(L,4),(GLenum)lua_tonumber(L,5),(GLenum)lua_tonumber(L,6),(const void*)lua_topointer(L,7));
	lua_pushnumber(L,ret);
	return 1;
}
static int gl_CallList(lua_State * L){
	glCallList((GLuint)lua_tonumber(L,1));
	return 0;
}
static int gl_CallLists(lua_State * L){
	glCallLists((GLsizei)lua_tonumber(L,1),(GLenum)lua_tonumber(L,2),(const GLvoid*)lua_topointer(L,3));
	return 0;
}
static int gl_ClearAccum(lua_State * L){
	glClearAccum((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3),(GLfloat)lua_tonumber(L,4));
	return 0;
}
static int gl_ClearDepth(lua_State * L){
	glClearDepth((GLclampd)lua_tonumber(L,1));
	return 0;
}
static int gl_ClearIndex(lua_State * L){
	glClearIndex((GLfloat)lua_tonumber(L,1));
	return 0;
}
static int gl_ClearStencil(lua_State * L){
	glClearStencil((GLint)lua_tonumber(L,1));
	return 0;
}

double lGetFieldd(lua_State * L, int i){
	lua_pushnumber(L,1);
	lua_gettable(L,-2);
	double ret = lua_tonumber(L,1);
	lua_pop(L,1);
	return ret;
}

static int gl_ClipPlane(lua_State * L){
	GLdouble eq[4];
	int plane = (int)lua_tonumber(L,1);
	lua_pushvalue(L,2);
	if (lua_istable(L, -1)){
		eq[0] = lGetFieldd(L,1);
		eq[1] = lGetFieldd(L,2);
		eq[2] = lGetFieldd(L,3);
		eq[3] = lGetFieldd(L,4);
	}
	glClipPlane((GLenum)plane,(GLdouble*)eq);
	return 0;
}
static int gl_Color3d(lua_State * L){
	glColor3d((GLdouble)lua_tonumber(L,1),(GLdouble)lua_tonumber(L,2),(GLdouble)lua_tonumber(L,3));
	return 0;
}
static int gl_Color3dv(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glColor3dv()");
	return 0;
}
static int gl_Color3fv(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glColor3fv()");
	return 0;
}
static int gl_Color4d(lua_State * L){
	glColor3d((GLdouble)lua_tonumber(L,1),(GLdouble)lua_tonumber(L,2),(GLdouble)lua_tonumber(L,3));
	return 0;
}
static int gl_Color4dv(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glColor3dv()");
	return 0;
}
static int gl_Color4fv(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glColor4fv()");
	return 0;
}

static int gl_ColorMask(lua_State * L){
	glColorMask((GLboolean)lua_tonumber(L,1),(GLboolean)lua_tonumber(L,2),(GLboolean)lua_tonumber(L,3),(GLboolean)lua_tonumber(L,4));
	return 0;
}
static int gl_ColorMaterial(lua_State * L){
	glColorMaterial((GLenum)lua_tonumber(L,1),(GLenum)lua_tonumber(L,2));
	return 0;
}
static int gl_ColorPointer(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glColorPointer()");
	//glColorPointer(
	return 0;
}
static int gl_CopyPixels(lua_State * L){
	glCopyPixels((GLint)lua_tonumber(L,1),(GLint)lua_tonumber(L,2),(GLsizei)lua_tonumber(L,3),(GLsizei)lua_tonumber(L,4),(GLenum)lua_tonumber(L,5));
	return 0;
}
static int gl_CopyTexImage1D(lua_State * L){
	glCopyTexImage1D((GLenum)lua_tonumber(L,1),(GLint)lua_tonumber(L,2),(GLenum)lua_tonumber(L,3),(GLint)lua_tonumber(L,4),(GLint)lua_tonumber(L,5),(GLsizei)lua_tonumber(L,6),(GLint)lua_tonumber(L,7));
	return 0;
}
static int gl_CopyTexImage2D(lua_State * L){
	glCopyTexImage2D((GLenum)lua_tonumber(L,1),(GLint)lua_tonumber(L,2),(GLenum)lua_tonumber(L,3),(GLint)lua_tonumber(L,4),(GLint)lua_tonumber(L,5),(GLsizei)lua_tonumber(L,6),(GLsizei)lua_tonumber(L,7),(GLint)lua_tonumber(L,8));
	return 0;
}
static int gl_CopyTexSubImage1D(lua_State * L){
	glCopyTexSubImage1D((GLenum)lua_tonumber(L,1),(GLint)lua_tonumber(L,2),(GLint)lua_tonumber(L,3),(GLint)lua_tonumber(L,4),(GLsizei)lua_tonumber(L,5),(GLsizei)lua_tonumber(L,6));
	return 0;
}
static int gl_CullFace(lua_State * L){
	glCullFace((GLenum)lua_tonumber(L,1));
	return 0;
}
static int gl_DeleteLists(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glDeleteLists()");
	//glDeleteList(
	return 0;
}
static int gl_DeleteTextures(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glDeleteTextures()");
	//glDeleteTextures(
	return 0;
}
static int gl_DepthFunc(lua_State * L){
	glDepthFunc((GLenum)lua_tonumber(L,1));
	return 0;
}
static int gl_DepthMask(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glDepthMask()");
	//glDepthMask(
	return 0;
}
static int gl_DepthRange(lua_State * L){
	glDepthRange((GLclampd)lua_tonumber(L,1),(GLclampd)lua_tonumber(L,2));
	return 0;
}
static int gl_DepthTextures(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glDepthTextures()");
	//glDepthTextures(
	return 0;
}
static int gl_DrawArrays(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glDrawArrays()");
	return 0;
}
static int gl_DrawBuffer(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glDrawBuffer()");
	return 0;
}
static int gl_DrawElements(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glDrawElements()");
	//glDrawElements(
	return 0;
}
static int gl_DrawPixels(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glDrawPixels()");
	return 0;
}
static int gl_EdgeFlag(lua_State * L){
	glEdgeFlag((GLboolean)lua_toboolean(L,1));
	return 0;
}
static int gl_EdgeFlagPointer(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glEdgeFlagPointer()");
	return 0;
}
static int gl_EnableClientState(lua_State * L){
	glEnableClientState((GLenum)lua_tonumber(L,1));
	return 0;
}

static int gl_DisableClientState(lua_State * L){
	glDisableClientState((GLenum)lua_tonumber(L,1));
	return 0;
}

static int glu_ErrorString(lua_State * L){
	int name = (int)lua_tonumber(L,1);
	const char * str = (const char *)gluErrorString(name);
	if (!str) str= "(NULL)";
	lua_pushstring(L,str);
	return 1;
}
static int gl_EvalCoord(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glEvalCoord()");
	return 0;
}
static int gl_EvalMesh(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glEvalMesh()");
	return 0;
}
static int gl_EvalPoint(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glEvalPoint()");
	return 0;
}
static int gl_FeedbackBuffer(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glFeedbackBuffer()");
	//glFeedbackBuffer((GLdouble)lua_tonumber(L,1),(GLdouble)lua_tonumber(L,1),(GLdouble)lua_tonumber(L,1));
	return 0;
}
static int gl_Fog(lua_State * L){
	glFogf((GLenum)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2));
	return 0;
}
static int gl_Finish(lua_State * L){
	glFinish();
	return 0;
}
static int gl_Flush(lua_State * L){
	glFlush();
	return 0;
}
static int gl_Frustum(lua_State * L){
	glFrustum((GLdouble)lua_tonumber(L,1),(GLdouble)lua_tonumber(L,2),(GLdouble)lua_tonumber(L,3),(GLdouble)lua_tonumber(L,4),(GLdouble)lua_tonumber(L,5),(GLdouble)lua_tonumber(L,6));
	return 0;
}
static int gl_GenLists(lua_State * L){
	GLuint ret = glGenLists((GLsizei)lua_tonumber(L,1));
	lua_pushnumber(L,ret);
	return 1;
}
static int gl_GenTextures(lua_State * L){
	GLuint tex[255];
	int	count = (GLsizei)lua_tonumber(L,1);
	if (count<=255){
		glGenTextures(count,tex);
		for (int i=0; i <count; i++){
			lua_pushnumber(L,tex[i]);
		}
	}else{
		count = 0;
	}
	return count;
}
static int gl_Get(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glGet()");
	//glGet(
	return 0;
}
static int gl_GetClipPlane(lua_State * L){
	__RENDER->dprintf("Stub(%s)","glGetClipPlane()");
	return 0;
}
static int gl_GetError(lua_State * L){
	int error = glGetError();
	lua_pushnumber(L,error);
	return 1;
}
static int gl_GetLight(lua_State * L){
	//glGetLight(
	return 0;
}
static int gl_GetMap(lua_State * L){
	//glGetMapf(
	return 0;
}
static int gl_GetMaterial(lua_State * L){
	//glGetMateriald(
	return 0;
}
static int gl_GetPixelMap(lua_State * L){
	return 0;
}
static int gl_GetPointer(lua_State * L){
	return 0;
}
static int gl_GetPolygonStipple(lua_State * L){
	return 0;
}
static int gl_GetString(lua_State * L){
	int name = (int)lua_tonumber(L,1);
	const char * str = (const char *)glGetString(name);
	if (!str) str= "(NULL)";
	lua_pushstring(L,str);
	return 1;
}
static int glu_GetString(lua_State * L){
	return 0;
}
static int gl_GetTessProperty(lua_State * L){
	return 0;
}
static int gl_GetTexEnv(lua_State * L){
	return 0;
}
static int gl_GetTexGen(lua_State * L){
	return 0;
}
static int gl_GetTexImage(lua_State * L){
	return 0;
}
static int gl_GetTexLevelParameter(lua_State * L){
	return 0;
}
static int gl_GetTexParameter(lua_State * L){
	return 0;
}
static int gl_Hint(lua_State * L){
	glHint((GLenum)lua_tonumber(L,1),(GLenum)lua_tonumber(L,2));
	return 0;
}
static int gl_Index(lua_State * L){
	glIndexf((GLfloat)lua_tonumber(L,1));
	return 0;
}
static int gl_IndexMask(lua_State * L){
	glIndexMask((GLuint)lua_tonumber(L,1));
	return 0;
}
static int gl_IndexPointer(lua_State * L){
	return 0;
}
static int gl_InitNames(lua_State * L){
	glInitNames();
	return 0;
}
static int gl_InterleavedArrays(lua_State * L){
	return 0;
}
static int gl_IsEnabled(lua_State * L){
	GLboolean ret = glIsEnabled((GLenum)lua_tonumber(L,1));
	lua_pushboolean(L,ret);
	return 1;
}
static int gl_IsList(lua_State * L){
	GLboolean ret = glIsList((GLuint)lua_tonumber(L,1));
	lua_pushboolean(L,ret);
	return 1;
}
static int gl_IsTexture(lua_State * L){
	//glIsTexure((GLuint)lua_tonumber(L,1));
	return 0;
}
static int gl_Lighti(lua_State * L){
	glLighti((GLenum)lua_tonumber(L,1),(GLenum)lua_tonumber(L,2),(GLint)lua_tonumber(L,3));
	return 0;
}
static int gl_Lightf(lua_State * L){
	glLightf((GLenum)lua_tonumber(L,1),(GLenum)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3));
	return 0;
}
static int gl_Lightfv(lua_State * L){
	GLenum p1 = (GLenum)lua_tonumber(L,1);
	GLenum p2 = (GLenum)lua_tonumber(L,2);
	GLfloat * param = getArrayf(L,3);
	glLightfv(p1,p2,param);
	delete[] param;
	return 0;
}
static int gl_LightModelf(lua_State * L){
	glLightModelf((GLenum)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2));
	return 0;
}
static int gl_LightModelfv(lua_State * L){
	GLfloat param [4];
	param[0] = (GLfloat)lua_tonumber(L,2);
	param[1] = (GLfloat)lua_tonumber(L,3);
	param[2] = (GLfloat)lua_tonumber(L,4);
	param[3] = (GLfloat)lua_tonumber(L,5);
	glLightModelfv((GLenum)lua_tonumber(L,1),param);
	return 0;
}
static int gl_LineStipple(lua_State * L){
	glLineStipple((GLint)lua_tonumber(L,1),(GLushort)lua_tonumber(L,2));
	return 0;
}
static int gl_LineWidth(lua_State * L){
	glLineWidth((GLfloat)lua_tonumber(L,1));
	return 0;
}
static int gl_ListBase(lua_State * L){
	glListBase((GLuint)lua_tonumber(L,1));
	return 0;
}
static int gl_LoadMatrix(lua_State * L){
	//glLoadMatrix(
	return 0;
}
static int gl_LoadName(lua_State * L){
	glLoadName((GLuint)lua_tonumber(L,1));
	return 0;
}
static int glu_LoadSamplingMatrices(lua_State * L){
	return 0;
}
static int glu_LookAt(lua_State * L){
	gluLookAt((GLdouble)lua_tonumber(L,1),(GLdouble)lua_tonumber(L,2),(GLdouble)lua_tonumber(L,3),(GLdouble)lua_tonumber(L,4),(GLdouble)lua_tonumber(L,5),(GLdouble)lua_tonumber(L,6),(GLdouble)lua_tonumber(L,7),(GLdouble)lua_tonumber(L,8),(GLdouble)lua_tonumber(L,9));
	return 0;
}
static int gl_Map1(lua_State * L){
	//glMap1f(
	return 0;
}
static int gl_Map2(lua_State * L){
	return 0;
}
static int gl_MapGrid(lua_State * L){
	return 0;
}
static int gl_Materialf(lua_State * L){
	glMaterialf((GLenum)lua_tonumber(L,1),(GLenum)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3));
	return 0;
}

static int gl_Materialfv(lua_State * L){
	GLfloat * param = getArrayf(L,3);
	glMaterialfv((GLenum)lua_tonumber(L,1),(GLenum)lua_tonumber(L,2),param);
	delete[] param;
	return 0;
}

static int gl_MultMatrix(lua_State * L){
	//glMultMatrixf(
	return 0;
}
static int gl_NewList(lua_State * L){
	glNewList((GLuint)lua_tonumber(L,1),(GLenum)lua_tonumber(L,2));
	return 0;
}
static int glu_NewNurbsRenderer(lua_State * L){
	return 0;
}
static int glu_NewQuadric(lua_State * L){
	GLUquadric * ret = gluNewQuadric();
	lua_pushlightuserdata(L,ret);
	return 1;
}
static int glu_NewTess(lua_State * L){
	GLUtesselator * ret = gluNewTess();
	lua_pushlightuserdata(L,ret);
	return 1;
}
static int glu_NextContour(lua_State * L){
	return 0;
}
static int gl_Normal3f(lua_State * L){
	glNormal3f((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3));
	return 0;
}
static int gl_Normal3d(lua_State * L){
	glNormal3d((GLdouble)lua_tonumber(L,1),(GLdouble)lua_tonumber(L,2),(GLdouble)lua_tonumber(L,3));
	return 0;
}
static int gl_NormalPointer(lua_State * L){
	return 0;
}

static int gl_NurbsCallback(lua_State * L){
	return 0;
}
static int gl_NurbsCurve(lua_State * L){
	return 0;
}
static int gl_NurbsProperty(lua_State * L){
	return 0;
}
static int gl_NurbsSurface(lua_State * L){
	return 0;
}
static int glu_Ortho2D(lua_State * L){
	gluOrtho2D((GLdouble)lua_tonumber(L,1),(GLdouble)lua_tonumber(L,2),(GLdouble)lua_tonumber(L,3),(GLdouble)lua_tonumber(L,4));
	return 0;
}
static int glu_PartialDisk(lua_State * L){
	gluPartialDisk((GLUquadric*)lua_touserdata(L,1),lua_tonumber(L,2),lua_tonumber(L,3),(int)lua_tonumber(L,4),(int)lua_tonumber(L,5),lua_tonumber(L,6),lua_tonumber(L,7));
	return 0;
}
static int gl_PassThrough(lua_State * L){
	glPassThrough((GLfloat)lua_tonumber(L,1));
	return 0;
}
static int glu_Perspective(lua_State * L){
	gluPerspective((GLdouble)lua_tonumber(L,1),(GLdouble)lua_tonumber(L,2),(GLdouble)lua_tonumber(L,3),(GLdouble)lua_tonumber(L,4));
	return 0;
}
static int glu_PickMatrix(lua_State * L){
	//gluPickMatrix(
	return 0;
}
static int gl_PixelMap(lua_State * L){
	return 0;
}
static int gl_PixelStore(lua_State * L){
	//glPixelStore(
	return 0;
}
static int gl_PixelTransfer(lua_State * L){
	//glPixelTransfer(
	return 0;
}
static int gl_PixelZoom(lua_State * L){
	glPixelZoom((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2));
	return 0;
}
static int gl_PointSize(lua_State * L){
	glPointSize((GLfloat)lua_tonumber(L,1));
	return 0;
}
static int gl_PolygonMode(lua_State * L){
	glPolygonMode((GLenum)lua_tonumber(L,1),(GLenum)lua_tonumber(L,2));
	return 0;
}
static int gl_PolygonOffset(lua_State * L){
	glPolygonOffset((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2));
	return 0;
}
static int gl_PolygonStipple(lua_State * L){
	//glPolygonStipple(
	return 0;
}
static int gl_PrioritizeTextures(lua_State * L){
	//glPrioritizeTextures(
	return 0;
}
static int glu_Project(lua_State * L){
	//gluProject(
	return 0;
}
static int gl_PopAttrib(lua_State * L){
	glPopAttrib();
	return 0;
}
static int gl_PopClientAttrib(lua_State * L){
	glPopClientAttrib();
	return 0;
}
static int gl_PopMatrix(lua_State * L){
	glPopMatrix();
	return 0;
}
static int gl_PopName(lua_State * L){
	glPopName();
	return 0;
}

static int gl_PushAttrib(lua_State * L){
	glPushAttrib((GLbitfield)lua_tonumber(L,1));
	return 0;
}
static int gl_PushClientAttrib(lua_State * L){
	glPushClientAttrib((GLbitfield)lua_tonumber(L,1));
	return 0;
}
static int gl_PushMatrix(lua_State * L){
	glPushMatrix();
	return 0;
}
static int gl_PushName(lua_State * L){
	glPushName((GLuint)lua_tonumber(L,1));
	return 0;
}
static int glu_PwlCurve(lua_State * L){
	//gluPwlCurve(
	return 0;
}
static int glu_QuadricCallback(lua_State * L){
	//gluQuadricCallback(
	return 0;
}
static int glu_QuadricDrawStyle(lua_State * L){
	gluQuadricDrawStyle((GLUquadric*)lua_touserdata(L,1),(GLenum)lua_tonumber(L,2));
	return 0;
}
static int glu_QuadricNormals(lua_State * L){
	GLUquadric * ptr = (GLUquadric *) lua_touserdata(L,1);
	int normals = (int)lua_tonumber(L,1);
	gluQuadricNormals(ptr,normals);
	return 0;
}
static int glu_QuadricOrientation(lua_State * L){
	GLUquadric * ptr = (GLUquadric *) lua_touserdata(L,1);
	int orientation = (int)lua_tonumber(L,1);
	gluQuadricOrientation(ptr,orientation);
	return 0;
}
static int glu_QuadricTexture(lua_State * L){
	GLUquadric * ptr = (GLUquadric *) lua_touserdata(L,1);
	int textures = (int)lua_toboolean(L,1);
	gluQuadricTexture(ptr,textures);
	return 0;
}
static int gl_RasterPos(lua_State * L){
	return 0;
}
static int gl_ReadBuffer(lua_State * L){
	return 0;
}
static int gl_ReadPixels(lua_State * L){
	return 0;
}
static int gl_Rect(lua_State * L){
	return 0;
}
static int gl_RenderMode(lua_State * L){
	GLint ret = glRenderMode((GLenum)lua_tonumber(L,1));
	lua_pushnumber(L,ret);
	return 1;
}
static int glu_ScaleImage(lua_State * L){
	//gluScaleImage(
	return 0;
}
static int gl_Scissor(lua_State * L){
	glScissor((GLint)lua_tonumber(L,1),(GLint)lua_tonumber(L,2),(GLsizei)lua_tonumber(L,3),(GLsizei)lua_tonumber(L,4));
	return 0;
}
static int gl_SelectBuffer(lua_State * L){
	//glSelectBuffer(
	return 0;
}
static int gl_ShadeModel(lua_State * L){
	glShadeModel((GLenum)lua_tonumber(L,1));
	return 0;
}
static int glu_Sphere(lua_State * L){
	gluSphere((GLUquadric*)lua_touserdata(L,1),lua_tonumber(L,2),(int)lua_tonumber(L,3),(int)lua_tonumber(L,4));
	return 0;
}
static int gl_StencilFunc(lua_State * L){
	glStencilFunc((GLenum)lua_tonumber(L,1),(GLint)lua_tonumber(L,2),(GLuint)lua_tonumber(L,3));
	return 0;
}
static int gl_StencilMask(lua_State * L){
	glStencilMask((GLuint)lua_tonumber(L,1));
	return 0;
}
static int gl_StencilOp(lua_State * L){
	glStencilOp((GLenum)lua_tonumber(L,1),(GLenum)lua_tonumber(L,2),(GLenum)lua_tonumber(L,3));
	return 0;
}
static int glu_TessBeginContour(lua_State * L){
	return 0;
}
static int glu_TessBeginPolygon(lua_State * L){
	return 0;
}
static int glu_TessCallback(lua_State * L){
	return 0;
}
static int glu_TessEndPolygon(lua_State * L){
	return 0;
}
static int glu_TessNormal(lua_State * L){
	return 0;
}
static int glu_TessProperty(lua_State * L){
	return 0;
}
static int glu_TessVertex(lua_State * L){
	return 0;
}
static int gl_TexCoordPointer(lua_State * L){
	return 0;
}
static int gl_TexEnvi(lua_State * L){
	glTexEnvi((GLenum)lua_tonumber(L,1),(GLenum)lua_tonumber(L,2),(GLint)lua_tonumber(L,3));
	return 0;
}

static int gl_TexEnvf(lua_State * L){
	glTexEnvf((GLenum)lua_tonumber(L,1),(GLenum)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3));
	return 0;
}

static int gl_TexGen(lua_State * L){
	//glTexGen(
	return 0;
}
static int gl_TexImage1D(lua_State * L){
	return 0;
}
static int gl_TexImage2D(lua_State * L){
	//glTexImage2D(
	return 0;
}
static int gl_TexParameter(lua_State * L){
	//glTexParameter(
	return 0;
}
static int gl_TexSubImage1D(lua_State * L){
	return 0;
}
static int gl_TexSubImage2D(lua_State * L){
	return 0;
}
static int glu_UnloadProject(lua_State * L){
	return 0;
}
static int gl_VertexPointer(lua_State * L){
	return 0;
}

static int gl_CreateProgramObject(lua_State * L){
	int so = glCreateProgramObjectARB();
	lua_pushnumber(L,so);
	return 1;
}

static int gl_CreateShaderObject(lua_State * L){
	int so = glCreateShaderObjectARB((GLenum)lua_tonumber(L,1));
	lua_pushnumber(L,so);
	return 1;
}
static int gl_ShaderSource(lua_State * L){
	const char * shader_source = lua_tostring(L,3);
	glShaderSourceARB((GLhandleARB)lua_tonumber(L,1), (int)lua_tonumber(L,2), (const char **)&shader_source, NULL);
	return 0;
}

static int gl_CompileShader(lua_State * L){
	glCompileShaderARB((GLuint) lua_tonumber(L,1));
	return 0;
}

static int gl_UseProgram(lua_State * L){
	glUseProgramObjectARB((GLuint) lua_tonumber(L,1));
	return 0;
}

static int gl_AttachObject(lua_State * L){
	glAttachObjectARB((GLhandleARB) lua_tonumber(L,1),(GLhandleARB) lua_tonumber(L,2));
	return 0;
}

static int gl_LinkProgram(lua_State * L){
	glLinkProgramARB((GLhandleARB) lua_tonumber(L,1));
	return 0;
}
static int gl_GetInfoLog(lua_State * L){
	int len;
	char buffer[1000];
	glGetInfoLogARB((GLhandleARB) lua_tonumber(L,1),(GLsizei)lua_tonumber(L,2),&len,buffer);
	lua_pushstring(L,buffer);
	return 1;
}
static int gl_DeleteObject(lua_State * L){
	glDeleteObjectARB((GLhandleARB) lua_tonumber(L,1));
	return 0;
}

static int gl_DetachShader(lua_State * L){
	glDetachObjectARB((GLhandleARB) lua_tonumber(L,1), (GLhandleARB) lua_tonumber(L,2));
	return 0;
}

static int gl_GetProgramiv(lua_State * L){
	//glGetProgramiv(program, GL_LINK_STATUS, compileResults);
	return 0;
}

static int gl_GetUniformLocation(lua_State * L){
	int ret = (int) glGetUniformLocationARB((GLhandleARB)lua_tonumber(L,1),(const GLcharARB*) lua_tostring(L,2));
	lua_pushnumber(L,ret);
	return 1;
}

static int gl_Uniform1f(lua_State * L){
	glUniform1fARB((GLint)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2));
	return 0;
}

static int gl_Uniform1i(lua_State * L){
	glUniform1iARB((GLint)lua_tonumber(L,1),(GLint)lua_tonumber(L,2));
	return 0;
}

static int gl_Uniform2f(lua_State * L){
	glUniform2fARB((GLint)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3));
	return 0;
}

static int gl_Uniform2i(lua_State * L){
	glUniform2iARB((GLint)lua_tonumber(L,1),(GLint)lua_tonumber(L,2),(GLint)lua_tonumber(L,3));
	return 0;
}
static int gl_Uniform3f(lua_State * L){
	glUniform3fARB((GLint)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3),(GLfloat)lua_tonumber(L,4));
	return 0;
}

static int gl_Uniform3i(lua_State * L){
	glUniform3iARB((GLint)lua_tonumber(L,1),(GLint)lua_tonumber(L,2),(GLint)lua_tonumber(L,3),(GLint)lua_tonumber(L,4));
	return 0;
}
static int gl_Uniform4f(lua_State * L){
	glUniform4fARB((GLint)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3),(GLfloat)lua_tonumber(L,4),(GLfloat)lua_tonumber(L,5));
	return 0;
}

static int gl_Uniform4i(lua_State * L){
	glUniform4iARB((GLint)lua_tonumber(L,1),(GLint)lua_tonumber(L,2),(GLint)lua_tonumber(L,3),(GLint)lua_tonumber(L,4),(GLint)lua_tonumber(L,5));
	return 0;
}

static int gl_ActiveTexture(lua_State * L){
	glActiveTextureARB((GLenum) lua_tonumber(L,1));
	return 0;
}

static int gl_ClientActivetexture(lua_State * L){
	glClientActivetexture((GLenum) lua_tonumber(L,1));
	return 0;
}

static int gl_MultitexCoord2f(lua_State * L){
	glMultitexCoord2f((GLenum) lua_tonumber(L,1),(float) lua_tonumber(L,2),(float) lua_tonumber(L,3));
	return 0;
}

static int setFontStyle(lua_State * L){
	qFont * _font = __RENDER->font;
	_font->setStyle(lua_tointeger(L,1));
	return 0;
}

static int getFontStyle(lua_State * L){
	qFont * _font = __RENDER->font;
	int style = _font->getStyle();
	lua_pushnumber(L,style);
	return 1;
}

static int getGlyphsCached(lua_State * L){
	qFont * _font = __RENDER->font;
	int num = 0;
	if (_font){
		num = _font->getGlyphsCached();
	}
	lua_pushnumber(L,num);
	return 1;
}

static int setupView(lua_State * L){
	__RENDER->drawPlayerView((int)lua_tonumber(L,1));
	return 0;
}

static int prepareGLSL(lua_State * L){
	prepareGLSLFunctions();
	return 0;
}

static int prepareMultiTexturing(lua_State * L){
	prepareMultiTexturingFunctions();
	return 0;
}

static int prepareVBO(lua_State * L){
	prepareVBOfunctions();
	return 0;
}

static const luaL_reg luaGL[] = {
	{"Accum",gl_Accum},
	{"ActiveTexture",gl_ActiveTexture},
	{"AreTexturesResident",gl_AreTexturesResident},
	{"AlphaFunc",gl_AlphaFunc},
	{"ArrayElement",gl_ArrayElement},
	{"AttachObject",gl_AttachObject},
	//---------------------------
	{"Begin",gl_Begin},
	{"BeginCurve",gl_BeginCurve},
	{"BeginPolygon",gl_BeginPolygon},
	{"BeginSurface",gl_BeginSurface},
	{"BeginTrim",gl_BeginTrim},
	{"BindTexture",gl_BindTexture},
	{"Bitmap",gl_Bitmap},
	{"BlendColorEXT",gl_BlendColorEXT},
	{"BlendFunc",gl_BlendFunc},
	{"Build1DMipmaps",glu_Build1DMipmaps},
	{"Build2DMipmaps",glu_Build2DMipmaps},
	//---------------------------
	{"CallList",gl_CallList},
	{"CallLists",gl_CallLists},
	{"Clear",gl_Clear},
	{"ClearAccum",gl_ClearAccum},
	{"ClearColor",gl_ClearColor},
	{"ClearDepth",gl_ClearDepth},
	{"ClearIndex",gl_ClearIndex},
	{"ClearStencil",gl_ClearStencil},
	{"ClientActivetexture",gl_ClientActivetexture},
	{"ClipPlane",gl_ClipPlane},
	{"Color3d",gl_Color3d},
	{"Color3dv",gl_Color3dv},
	{"Color3f",gl_Color3f},
	{"Color3fv",gl_Color3fv},
	{"Color4d",gl_Color4d},
	{"Color4dv",gl_Color4dv},
	{"Color4f",gl_Color4f},
	{"Color4fv",gl_Color4fv},
	{"ColorMask",gl_ColorMask},
	{"ColorMaterial",gl_ColorMaterial},
	{"ColorPointer",gl_ColorPointer},
	{"CompileShader",gl_CompileShader},
	{"CopyPixels",gl_CopyPixels},
	{"CopyTexImage1D",gl_CopyTexImage1D},
	{"CopyTexImage2D",gl_CopyTexImage2D},
	{"CopyTexSubImage1D",gl_CopyTexSubImage1D},
	{"CopyTexSubImage2D",gl_CopyTexSubImage2D},
	{"CopyTexSubImage1D",gl_CopyTexSubImage1D},
	{"CreateProgramObject",gl_CreateProgramObject},
	{"CreateShaderObject",gl_CreateShaderObject},
	{"CullFace",gl_CullFace},
	//---------------------------
	{"DeleteLists",gl_DeleteLists},
	{"DeleteObject",gl_DeleteObject},
	{"DeleteTextures",gl_DeleteTextures},
	{"DepthTextures",gl_DepthTextures},
	{"DepthFunc",gl_DepthFunc},
	{"DepthMask",gl_DepthMask},
	{"DepthRange",gl_DepthRange},
	{"DetachShader",gl_DetachShader},
	{"Disable",gl_Disable},
	{"DisableClientState",gl_DisableClientState},
	{"DrawArrays",gl_DrawArrays},
	{"DrawBuffer",gl_DrawBuffer},
	{"DrawElements",gl_DrawElements},
	{"DrawPixels",gl_DrawPixels},
	//---------------------------
	{"EdgeFlag",gl_EdgeFlag},
	{"EdgeFlagPointer",gl_EdgeFlagPointer},
	{"Enable",gl_Enable},
	{"EnableClientState",gl_EnableClientState},
	{"End",gl_End},
	{"EndList",gl_EndList},
	{"ErrorString",glu_ErrorString},
	{"EvalCoord",gl_EvalCoord},
	{"EvalMesh",gl_EvalMesh},
	{"EvalPoint",gl_EvalPoint},
	//---------------------------
	{"FeedbackBuffer",gl_FeedbackBuffer},
	{"Finish",gl_Finish},
	{"Flush",gl_Flush},
	{"Fog",gl_Fog},
	{"FrontFace",gl_FrontFace},
	{"Frustum",gl_Frustum},
	//---------------------------
	{"GenLists",gl_GenLists},
	{"GenTextures",gl_GenTextures},
	{"Get",gl_Get},
	{"GetClipPlane",gl_GetClipPlane},
	{"GetError",gl_GetError},
	{"GetFontStyle",getFontStyle},
	{"GetGlyphsCached",getGlyphsCached},
	{"GetImage",gl_GetImage},
	{"GetInfoLog",gl_GetInfoLog},
	{"GetLight",gl_GetLight},
	{"GetMap",gl_GetMap},
	{"GetMaterial",gl_GetMaterial},
	{"GetPixelMap",gl_GetPixelMap},
	{"GetPointer",gl_GetPointer},
	{"GetPolygonStipple",gl_GetPolygonStipple},
	{"GetString",gl_GetString},
	{"GetString2",glu_GetString},
	{"GetTessProperty",gl_GetTessProperty},
	{"GetTexEnv",gl_GetTexEnv},
	{"GetTexGen",gl_GetTexGen},
	{"GetTexImage",gl_GetTexImage},
	{"GetTexLevelParameter",gl_GetTexLevelParameter},
	{"GetTexParameter",gl_GetTexParameter},
	{"GetUniformLocation",gl_GetUniformLocation},
	//---------------------------
	{"Hint",gl_Hint},
	//---------------------------
	{"Index",gl_Index},
	{"IndexMask",gl_IndexMask},
	{"IndexPointer",gl_IndexPointer},
	{"InitNames",gl_InitNames},
	{"InterleavedArrays",gl_InterleavedArrays},
	{"IsEnabled",gl_IsEnabled},
	{"IsList",gl_IsList},
	{"IsTexture",gl_IsTexture},
	//---------------------------
	{"Lighti",gl_Lighti},
	{"Lightf",gl_Lightf},
	{"Lightfv",gl_Lightfv},
	{"LightModelf",gl_LightModelf},
	{"LightModelfv",gl_LightModelfv},
	{"LineStipple",gl_LineStipple},
	{"LineWidth",gl_LineWidth},
	{"LinkProgram",gl_LinkProgram},
	{"ListBase",gl_ListBase},
	{"LoadFont",gl_load_font},
	{"LoadIdentity",gl_LoadIdentity},
	{"LoadMatrix",gl_LoadMatrix},
	{"LoadName",gl_LoadName},
	{"LoadSamplingMatrices",glu_LoadSamplingMatrices},
	{"LookAt",glu_LookAt},
	//---------------------------
	{"Map1",gl_Map1},
	{"Map2",gl_Map2},
	{"MapGrid",gl_MapGrid},
	{"Materialf",gl_Materialf},
	{"Materialfv",gl_Materialfv},
	{"MatrixMode",gl_MatrixMode},
	{"MultMatrix",gl_MultMatrix},
	{"MultitexCoord2f",gl_MultitexCoord2f},
	//---------------------------
	{"NewList",gl_NewList},
	{"NewNurbsRenderer",glu_NewNurbsRenderer},
	{"NewQuadric",glu_NewQuadric},
	{"NewTess",glu_NewTess},
	{"NextContour",glu_NextContour},
	{"Normal3f",gl_Normal3f},
	{"Normal3d",gl_Normal3d},
	{"NormalPointer",gl_NormalPointer},
	{"NurbsCallback",gl_NurbsCallback},
	{"NurbsCurve",gl_NurbsCurve},
	{"NurbsProperty",gl_NurbsProperty},
	{"NurbsSurface",gl_NurbsSurface},
	//---------------------------
	{"Ortho",gl_Ortho},
	{"Ortho2D",glu_Ortho2D},
	//---------------------------
	{"PartialDisk",glu_PartialDisk},
	{"PassThrough",gl_PassThrough},
	{"Perspective",glu_Perspective},
	{"PickMatrix",glu_PickMatrix},
	{"PixelMap",gl_PixelMap},
	{"PixelStore",gl_PixelStore},
	{"PixelTransfer",gl_PixelTransfer},
	{"PixelZoom",gl_PixelZoom},
	{"PointSize",gl_PointSize},
	{"PolygonMode",gl_PolygonMode},
	{"PolygonOffset",gl_PolygonOffset},
	{"PolygonStipple",gl_PolygonStipple},
	{"PrepareOrthogonal",gl_PrepareOrthogonal},
	{"printf",gl_printf},
	{"PrioritizeTextures",gl_PrioritizeTextures},
	{"Project",glu_Project},
	{"PopAttrib",gl_PopAttrib},
	{"PopClientAttrib",gl_PopClientAttrib},
	{"PopMatrix",gl_PopMatrix},
	{"PopName",gl_PopName},
	{"prepareGLSL",prepareGLSL},
	{"prepareMultiTexturing",prepareMultiTexturing},
	{"prepareVBO",prepareVBO},
	{"PushAttrib",gl_PushAttrib},
	{"PushClientAttrib",gl_PushClientAttrib},
	{"PushMatrix",gl_PushMatrix},
	{"PushName",gl_PushName},
	{"PwlCurve",glu_PwlCurve},
	//---------------------------
	{"QuadricCallback",glu_QuadricCallback},
	{"QuadricDrawStyle",glu_QuadricDrawStyle},
	{"QuadricNormals",glu_QuadricNormals},
	{"QuadricOrientation",glu_QuadricOrientation},
	{"QuadricTexture",glu_QuadricTexture},
	//---------------------------
	{"RasterPos",gl_RasterPos},
	{"ReadBuffer",gl_ReadBuffer},
	{"ReadPixels",gl_ReadPixels},
	{"Rect",gl_Rect},
	{"RenderMode",gl_RenderMode},
	{"Rotatef",gl_Rotatef},
	//---------------------------
	{"Scalef",gl_Scalef},
	{"ScaleImage",glu_ScaleImage},
	{"Scissor",gl_Scissor},
	{"SelectBuffer",gl_SelectBuffer},
	{"SetFontStyle",setFontStyle},
	{"SetupView",setupView},
	{"ShadeModel",gl_ShadeModel},
	{"ShaderSource",gl_ShaderSource},
	{"Sphere",glu_Sphere},
	{"StartPicking",startPicking},
	{"StencilFunc",gl_StencilFunc},
	{"StencilMask",gl_StencilMask},
	{"StencilOp",gl_StencilOp},
	{"StopPicking",stopPicking},
	//---------------------------
	{"TessBeginContour",glu_TessBeginContour},
	{"TessBeginPolygon",glu_TessBeginPolygon},
	{"TessCallback",glu_TessCallback},
	{"TessEndPolygon",glu_TessEndPolygon},
	{"TessNormal",glu_TessNormal},
	{"TessProperty",glu_TessProperty},
	{"TessVertex",glu_TessVertex},
	{"TexCoord2f",gl_TexCoord2f},
	{"TexCoordPointer",gl_TexCoordPointer},
	{"TexParameteri",gl_TexParameteri},
	{"TexEnvf",gl_TexEnvf},
	{"TexEnvi",gl_TexEnvi},
	{"TexGen",gl_TexGen},
	{"TexImage1D",gl_TexImage1D},
	{"TexImage2D",gl_TexImage2D},
	{"TexParameter",gl_TexParameter},
	{"TexSubImage1D",gl_TexSubImage1D},
	{"TexSubImage2D",gl_TexSubImage2D},
	{"tglyph",gl_tglyph},
	{"tprintf",gl_tprintf},
	{"tprintfEx",gl_tprintfEx},
	{"Translatef",gl_Translatef},
	//---------------------------
	{"Uniform1f",gl_Uniform1f},
	{"Uniform1i",gl_Uniform1i},
	{"Uniform2f",gl_Uniform2f},
	{"Uniform2i",gl_Uniform2i},
	{"Uniform3f",gl_Uniform3f},
	{"Uniform3i",gl_Uniform3i},
	{"Uniform4f",gl_Uniform4f},
	{"Uniform4i",gl_Uniform4i},
	{"UnloadProject",glu_UnloadProject},
	{"UseProgram",gl_UseProgram},
	//---------------------------
	{"Vertex2f",gl_Vertex2f},
	{"Vertex2i",gl_Vertex2i},
	{"Vertex3f",gl_Vertex3f},
	{"VertexPointer",gl_VertexPointer},
	{"Viewport",gl_Viewport},
	//---------------------------
	{NULL,NULL}
};

void cLuaGL::prepareFunctions(lua_State * Lua){
	luaL_openlib(Lua, "gl", luaGL, 0);
}
