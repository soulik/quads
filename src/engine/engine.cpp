#include "engine/engine.h"
#include "modules/script.h"
#include "modules/game.h"
#include "modules/renderer.h"
#include "modules/qTexturer.h"
#include "modules/controler.h"
#include "gui/layers/qScene.h"

#include "base.h"
#include "common.h"

static int getStackInfo(lua_State * L){
	__SCRIPT->lockMutex();
	lua_pushnumber(L,lua_gettop(L));
	__SCRIPT->unlockMutex();
	return 1;
}
static int l_gettickcount(lua_State * L){
	__SCRIPT->lockMutex();
	lua_pushnumber(L,SDL_GetTicks());
	__SCRIPT->unlockMutex();
	return 1;
}

static int l_get_key(lua_State * L){
	__SCRIPT->lockMutex();
	int key = (int)lua_tointeger(L,1);
	lua_pop(L,1);
	lua_pushinteger(L,__CONTROL->getKeyboard()->GetKey(key));
	__SCRIPT->unlockMutex();
	return 1;
}

static int l_get_keymod(lua_State * L){
	__SCRIPT->lockMutex();
	lua_pushinteger(L,__CONTROL->getKeyboard()->GetMod());
	__SCRIPT->unlockMutex();
	return 1;
}

static int l_get_mouse_button(lua_State * L){
	__SCRIPT->lockMutex();
	lua_pushinteger(L,__CONTROL->getMouse()->getButtons());
	__SCRIPT->unlockMutex();
	return 1;
}

static int l_get_mouse_button2(lua_State * L){
	__SCRIPT->lockMutex();
	lua_pushinteger(L,SDL_GetMouseState(NULL, NULL));
	__SCRIPT->unlockMutex();
	return 1;
}

static int l_load_texture(lua_State *L){
	__SCRIPT->lockMutex();
	const char * name = lua_tostring(L,1);
	int type = (int)lua_tointeger(L,2);
	char _fname[512];
	strcpy(_fname,name);
	char * spos;
	while (spos = strchr(_fname,'/')){
		(*spos) = '\\';
	}
	if (!__TEXTURE->loadImage(name,type)){
		fprintf(stderr,"Can't load image : '%s'",name);
	}
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_create_texture(lua_State *L){
	__SCRIPT->lockMutex();
	int width = (int)lua_tonumber(L,1);
	int height = (int)lua_tonumber(L,2);
	int flags = (int)lua_tointeger(L,3);
	const char * name = lua_tostring(L,4);
	__TEXTURE->addImage((float)width,(float)height, flags,name);
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_load_map(lua_State *L){
	__SCRIPT->lockMutex();
	const char * name = lua_tostring(L,1);
 	__GAME->glogic->map.LoadMap(name);
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_load_map_v2(lua_State *L){
	__SCRIPT->lockMutex();
	const char * name = lua_tostring(L,1);
 	__GAME->glogic->map.LoadMapV2(name);
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_save_map_v2(lua_State *L){
	__SCRIPT->lockMutex();
	const char * name = lua_tostring(L,1);
 	__GAME->glogic->map.SaveMapV2(name);
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_gen_map_v2(lua_State *L){
	__SCRIPT->lockMutex();
	int width = (int)lua_tointeger(L,1);
	int height = (int)lua_tointeger(L,2);
	__GAME->glogic->map.GenerateMapV2(width,height);
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_clear_map(lua_State *L){
	__SCRIPT->lockMutex();
	int width = (int)lua_tointeger(L,1);
	int height = (int)lua_tointeger(L,2);
	__GAME->glogic->map.ClearMap(width,height);
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_get_map(lua_State *L){
	__SCRIPT->lockMutex();
	long x = (long)lua_tointeger(L,1);
	long y = (long)lua_tointeger(L,2);
	long z = (long)lua_tointeger(L,3);
	qTile * tile = __GAME->glogic->map.GetMap(x,y);
	if (tile){
		lua_pushnumber(L,tile->z);
	}
	__SCRIPT->unlockMutex();
	return 1;
}

static int l_set_map(lua_State *L){
	__SCRIPT->lockMutex();
	long x = (long)lua_tointeger(L,1);
	long y = (long)lua_tointeger(L,2);
	long z = (long)lua_tointeger(L,3);
	qTile * tile = __GAME->glogic->map.GetMap(x,y);
	if (tile){
		tile->z = z;
	}
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_set_map_flags(lua_State *L){
	__SCRIPT->lockMutex();
	unsigned long flags = (unsigned long)lua_tointeger(L,1);
	__GAME->glogic->map.flags = flags;
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_load_test_map(lua_State *L){
	__SCRIPT->lockMutex();
	int width = (int)lua_tointeger(L,1);
	int height = (int)lua_tointeger(L,2);
	__GAME->glogic->map.LoadTestMap(width,height);
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_init_data(lua_State *L){
	__SCRIPT->lockMutex();
	const char * data_dir = "";
	lua_pushvalue (L, 1);
	if (!lua_istable(L, -1)){
		__SCRIPT->dprintf("the parameter is not a table!\n");
	}else{
		data_dir = __SCRIPT->getFields(L,"data_dir");
		__SCRIPT->dprintf("Setting data dir to: %s\n", data_dir);
	}
	__TEXTURE->setBaseDir(data_dir);
	lua_pop(L,1);
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_init_renderer(lua_State *L){
	__SCRIPT->lockMutex();
	int width = 640;
	int height = 480;
	int bpp = 32;
	int red_size = 8;
	int green_size = 8;
	int blue_size = 8;
	int depth_size = 16;
	int doublebuffer = 1;
	lua_pushvalue (L, 1);
	if (!lua_istable(L, -1)){
		__SCRIPT->dprintf("the parameter is not a table!\n");
	}else{
		width = __SCRIPT->getFieldi(L,"width");
		height = __SCRIPT->getFieldi(L,"height");
		red_size = __SCRIPT->getFieldi(L,"red_size");
		green_size = __SCRIPT->getFieldi(L,"green_size");
		blue_size = __SCRIPT->getFieldi(L,"blue_size");
		depth_size = __SCRIPT->getFieldi(L,"depth_size");
		doublebuffer = __SCRIPT->getFieldi(L,"doublebuffer");
		__SCRIPT->dprintf("Setting video mode: %dx%dx%dx%d\n", width, height , red_size+green_size+blue_size,depth_size);
	}
	lua_pop(L,1);
	__RENDER->init(width,height,red_size,green_size,blue_size,depth_size,doublebuffer);
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_init_keyboard(lua_State *L){
	__SCRIPT->lockMutex();
	int keys[256];

	lua_pushvalue (L, 1);
	if (!lua_istable(L, -1)){
		__SCRIPT->dprintf("the parameter is not a table!\n");
	}else{
		keys[K_FORWARD] = __SCRIPT->getFieldi(L,"forward");
		keys[K_BACKWARD] = __SCRIPT->getFieldi(L,"backward");
		keys[K_STRAFE_LEFT] = __SCRIPT->getFieldi(L,"strafe_left");
		keys[K_STRAFE_RIGHT] = __SCRIPT->getFieldi(L,"strafe_right");
		keys[K_UP] = __SCRIPT->getFieldi(L,"fly_up");
		keys[K_DOWN] = __SCRIPT->getFieldi(L,"fly_down");
		keys[K_QUIT] = __SCRIPT->getFieldi(L,"quit");
		__CONTROL->setKeys(keys);
	}
	lua_pop(L,1);
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_debug(lua_State *L){
	__SCRIPT->lockMutex();
	lua_newtable(L);
	__SCRIPT->setFieldValue(L,"name","testicek");

	lua_pushstring(L,"data");
	lua_newtable(L);

	__SCRIPT->setFieldValue(L,"name","testicek2");
	__SCRIPT->setFieldValue(L,"data","nothing's here");

	lua_settable(L,-3);
	__SCRIPT->unlockMutex();
	return 1;
}
static int l_get_data(lua_State *L){
	__SCRIPT->lockMutex();
	const char * name = lua_tostring(L,1);
	sValue * data = __SCRIPT->mainTable.getValue(name);
	if (data){
		switch (data->guessType()){
			case LUA_TSTRING:
				lua_pushstring(L,data->string);
				break;
			case LUA_TFUNCTION:
				lua_pushlightuserdata(L,(void*)data->pointer);
				break;
			case LUA_TUSERDATA:
				lua_pushlightuserdata(L,(void*)data->pointer);
				break;
			case LUA_TNUMBER:
				if (data->number!=0.0){
					lua_pushnumber(L,data->number);
				}else if (data->integer!=0){
					lua_pushinteger(L,data->integer);
				}else{
					lua_pushnumber(L,0.0);
				}
				break;
			default:
				__SCRIPT->dprintf("Unknown data type! (%s)\n",name);
		}
	}else{
		lua_pushnil(L);
	}
	__SCRIPT->unlockMutex();
	return 1;
}

static int l_set_data(lua_State *L){
	__SCRIPT->lockMutex();
	if (!lua_isnil(L,1)){
		const char * name = lua_tostring(L,1);
		char * temp_name = (char*)name;
		sValue data;
		double temp,ipart,fpart;
		int type = lua_type(L,2);
		switch (type){
			case LUA_TNUMBER:
				temp = lua_tonumber(L,2);
				fpart = modf(temp, &ipart );
				//test if it's integer or double
				if (fpart==0.0){
					data.integer = (int)lua_tointeger(L,2);
				}else{
					data.number = temp;
				}
				break;
			case LUA_TSTRING:
				data.string = lua_tostring(L,2);
				break;
			case LUA_TFUNCTION:
				data.pointer = (void*)luaL_ref(L, LUA_REGISTRYINDEX);
				break;
			case LUA_TTABLE:
				temp_name = (char*)malloc(strlen(name)+7);
				sprintf(temp_name,"%s::self",name);
				data.pointer = (void*)luaL_ref(L, LUA_REGISTRYINDEX);
				break;
			default:
				__SCRIPT->dprintf("Unknown type: %d\n",type);
		}
		__SCRIPT->mainTable.setValue(temp_name,data);
	}else{
		__SCRIPT->dprintf("setData(): first parameter is NULL\n");
	}
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_set_font_texture(lua_State * L){
	__SCRIPT->lockMutex();
	unsigned int tex = (unsigned int)lua_tonumber(L,1);
	if (__RENDER->font){
		__RENDER->font->setTexture(tex);
	}
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_dprintf(lua_State *L){
	return 0;
}

static int l_tuserdata(lua_State * L){
	const void * ptr = lua_topointer(L,1);
	return 0;
}

static int l_load_model(lua_State * L){
	__SCRIPT->lockMutex();
	const char * fname = (const char *)lua_tostring(L,1);
	int type = lua_isnil(L,2) ? 0 : (int)lua_tonumber(L,2);
	const char * param = lua_isnil(L,3) ? NULL : (const char *)lua_tostring(L,3);
	if (fname){
		qScene * models = (qScene* )__RENDER->dObjects[0];
		if (models){
			try{
				models->loadModel(fname,type,param);
			}catch (cException e){
				__SCRIPT->dprintf("Exception '%s'\n",e.what());
			}
		}
	}
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_model_update(lua_State * L){
	__SCRIPT->lockMutex();
	const char * name = (const char *)lua_tostring(L,1);
	if (name){
		qScene * models = (qScene* )__RENDER->dObjects[0];
		if (models){
			try{
				qModel * model = models->getModel(name);
				if (model){
					model->update();
				}
			}catch (cException e){
				__SCRIPT->dprintf("Exception '%s'\n",e.what());
			}
		}
	}
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_model_set_frame(lua_State * L){
	__SCRIPT->lockMutex();
	const char * name = (const char *)lua_tostring(L,1);
	const char * objname = (const char *)lua_tostring(L,2);
	int frame = (int)lua_tonumber(L,3);
	if (name){
		qScene * models = (qScene* )__RENDER->dObjects[0];
		if (models){
			try{
				qModel * model = models->getModel(name);
				if (model){
					model->setFrame(objname,frame);
				}
			}catch (cException e){
				__SCRIPT->dprintf("Exception '%s'\n",e.what());
			}
		}
	}
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_model_set_animation(lua_State * L){
	__SCRIPT->lockMutex();
	const char * mname = (const char *)lua_tostring(L,1);
	const char * objname = (const char *)lua_tostring(L,2);
	const char * name = (const char *)lua_tostring(L,3);
	if (mname && objname && name){
		qScene * models = (qScene* )__RENDER->dObjects[0];
		if (models){
			try{
				qModel * model = models->getModel(mname);
				if (model){
					model->setAnimation(objname,name);
				}
			}catch (cException e){
				__SCRIPT->dprintf("Exception '%s'\n",e.what());
			}
		}
	}
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_draw_model(lua_State * L){
	__SCRIPT->lockMutex();
	const char * fname = (const char *)lua_tostring(L,1);
	int mode = (int)lua_tointeger(L,2);
	if (fname){
		qScene * models = (qScene* )__RENDER->dObjects[0];
		if (models){
			try{
				models->drawModel(fname,mode);
			}catch (cException e){
				__SCRIPT->dprintf("Exception '%s'\n",e.what());
			}
		}
	}
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_update_model_material(lua_State * L){
	__SCRIPT->lockMutex();
	const char * mname = (const char *)lua_tostring(L,1);
	const char * mat_name = (const char *)lua_tostring(L,2);
	if (mname && mat_name){
		qScene * models = (qScene* )__RENDER->dObjects[0];
		if (models){
			try{
				models->updateMaterial(mname,mat_name);
			}catch (cException e){
				__SCRIPT->dprintf("Exception '%s'\n",e.what());
			}
		}
	}
	__SCRIPT->unlockMutex();
	return 0;
}

static int l_set_pos(lua_State * L){
	__GAME->glogic->curObject()->setPosition(qVector((float)lua_tonumber(L,1),(float)lua_tonumber(L,2),(float)lua_tonumber(L,3)));
	return 0;
}

static int l_set_angle(lua_State * L){
	__GAME->glogic->curObject()->setAngle(qVector((float)lua_tonumber(L,1),(float)lua_tonumber(L,2),(float)lua_tonumber(L,3)));
	return 0;
}

static int l_look_at(lua_State * L){
	__GAME->glogic->camera->lookAt(qVector((float)lua_tonumber(L,1),(float)lua_tonumber(L,2),(float)lua_tonumber(L,3)),qVector((float)lua_tonumber(L,4),(float)lua_tonumber(L,5),(float)lua_tonumber(L,6)),(float)lua_tonumber(L,7));
	return 0;
}

static int l_set_camera_pos(lua_State * L){
	__GAME->glogic->camera->setPosition(qVector((float)lua_tonumber(L,1),(float)lua_tonumber(L,2),(float)lua_tonumber(L,3)));
	return 0;
}

static int l_set_angle_from_rel_mouse(lua_State * L){
	int rx=(int)lua_tonumber(L,1),ry=(int)lua_tonumber(L,2);

	__SCRIPT->dprintf("RelMouse [%d,%d]\n",rx,ry);
	__GAME->glogic->camera->setAngleFromRelMouse(qPointi(rx,ry));
	return 0;
}

static int l_get_fps(lua_State * L){
	lua_pushnumber(L, __RENDER->getFPS());
	return 1;
}

static int l_set_walk_speed(lua_State * L){
	__GAME->glogic->setSpeed(lua_tonumber(L,1));
	return 0;
}

static int l_get_walk_speed(lua_State * L){
	lua_pushnumber(L,__GAME->glogic->getSpeed());
	return 1;
}

static int l_move_forward(lua_State * L){
	__GAME->glogic->walkForward();
	return 0;
}

static int l_move_backward(lua_State * L){
	__GAME->glogic->walkBackward();
	return 0;
}
static int l_strafe_left(lua_State * L){
	__GAME->glogic->strafeLeft();
	return 0;
}
static int l_strafe_right(lua_State * L){
	__GAME->glogic->strafeRight();
	return 0;
}
static int l_fly_up(lua_State * L){
	__GAME->glogic->walkUp();
	return 0;
}
static int l_fly_down(lua_State * L){
	__GAME->glogic->walkDown();
	return 0;
}

static int l_quit(lua_State * L){
	__GAME->glogic->quit();
	return 0;
}

static int l_gravity_ON(lua_State * L){
	__GAME->glogic->gravityON();
	return 0;
}

static int l_gravity_OFF(lua_State * L){
	__GAME->glogic->gravityOFF();
	return 0;
}

static int l_set_cursor(lua_State * L){
	SDL_Cursor * cur = __RENDER->getCursor((int)lua_tonumber(L,1));
	if (cur){
		SDL_SetCursor(cur);
	}
	return 0;
}

static int l_load_cursor(lua_State * L){
	const char ** image = getArrays(L,1);
	int width = (int)lua_tonumber(L,2);
	int height = (int)lua_tonumber(L,3);
	int hotx = (int)lua_tonumber(L,4);
	int hoty = (int)lua_tonumber(L,5);
	int id=0;
	unsigned char * data = (unsigned char *) malloc((width/8)*height*2);
	if (data){
		unsigned char * mask = data+(width/8)*height;
		int i = -1,x,y;
		for ( y=0; y<height; ++y ) {
			for ( x=0; x<width; ++x ) {
				if ( x % 8 ) {
					data[i] <<= 1;
					mask[i] <<= 1;
				} else {
					++i;
					data[i] = mask[i] = 0;
				}
				switch (image[y][x]) {
					case 'X':
						data[i] |= 0x01;
						mask[i] |= 0x01;
						break;
					case 'O':
						data[i] |= 0x01;
						break;
					case '.':
						mask[i] |= 0x01;
						break;
					case ' ':
						break;
				}
			}
		}
		id = __RENDER->addCursor(data,mask,width,height,hotx,hoty);
		free(data);
	}
	delete[] image;
	lua_pushnumber(L,id);
	return 1;
}

static int l_get_cursor(lua_State * L){
	SDL_Cursor * cur;
	cur = SDL_GetCursor();
	return 0;
}

/*
{"",},
*/
static const luaL_reg luaEngine[] = {

	{"getStackInfo",getStackInfo},
	{"getTickCount",l_gettickcount},

	{"clearMap",l_clear_map},
	{"genMap2",l_gen_map_v2},
	{"getMap",l_get_map},
	{"loadMap",l_load_map},
	{"loadMap2",l_load_map_v2},
	{"loadTestMap",l_load_test_map},
	{"saveMap2",l_save_map_v2},
	{"setMap",l_set_map},
	{"setMapFlags",l_set_map_flags},
	{"loadModel",l_load_model},
	{"drawModel",l_draw_model},
	{"updateModelMaterial",l_update_model_material},
	{"updateModel",l_model_update},
	{"setModelAnimation",l_model_set_animation},
	{"setModelFrame",l_model_set_frame},
	

	{"initData",l_init_data},
	{"getData",l_get_data},
	{"setData",l_set_data},

	{"getKey",l_get_key},
	{"getKeymod",l_get_keymod},
	{"getMouseButton",l_get_mouse_button},
	{"getMouseButton2",l_get_mouse_button2},
	{"initKeyboard",l_init_keyboard},
	{"initRenderer",l_init_renderer},
	{"createTexture",l_create_texture},
	{"loadTexture",l_load_texture},
	{"setFontTexture",l_set_font_texture},
	{"dprintf",l_dprintf},
	{"debug",l_debug},
	{"tuserdata",l_tuserdata},

	{"setSpeed",l_set_walk_speed},
	{"getSpeed",l_get_walk_speed},
	{"moveForward",l_move_forward},
	{"moveBackward",l_move_backward},
	{"strafeLeft",l_strafe_left},
	{"strafeRight",l_strafe_right},
	{"flyUp",l_fly_up},
	{"flyDown",l_fly_down},
	{"gravityON",l_gravity_ON},
	{"gravityOFF",l_gravity_OFF},
	{"setPosition",l_set_pos},
	{"setCameraPosition",l_set_camera_pos},
	{"setAngle",l_set_angle},
	{"setAngleFromRelMouse",l_set_angle_from_rel_mouse},
	{"lookAt",l_look_at},

	{"loadCursor",l_load_cursor},
	{"setCursor",l_set_cursor},

	{"getFPS",l_get_fps},

	{"quit",l_quit},

	{NULL,NULL}
};

void qEngine::prepareFunctions(lua_State * L){
	luaL_openlib(L, "qE", luaEngine, 0);
}
