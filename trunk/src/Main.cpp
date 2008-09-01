#include "main.h"
#include "modules/game.h"
#include "modules/script.h"
#include "modules/renderer.h"
#include "modules/controler.h"
#include "modules/qTexturer.h"

cMain::cMain(){
	stddbg = stdout;
	try{
		SDL_Init(0);
		modules.push_back(new cScript(this));
		modules.push_back(new cRenderer(this));
		modules.push_back(new qTexturer(this));
		modules.push_back(new cGame(this));
		modules.push_back(new cControler(this));
	}DEFAULT_EXCEPTION(this->stddbg)
}

cMain::~cMain(){
	try{
		for (uint i=(uint)modules.size()-1; i>0;i--){
			delete modules[i];
		}
		SDL_Quit();
	}DEFAULT_EXCEPTION(this->stddbg)
}

int wmain(){
	cMain main;
	main.initModules();
	main.runModules();
	main.quitModules();
	return 0;
}
