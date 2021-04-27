#include "MenuLayer.hpp"

#include <fstream>
#include <iostream>

#include "../utils/base64.h"

void SetStringPointer(gd::GDString* str, const char* text) {
	int len = strlen(text);

	str->length   = len;
	str->capacity = max(len, 15);

	if (len <= 15)
		memcpy((void*)str, text, len + 1);
	else {
		void* newstr = malloc(len + 1);
		memcpy(newstr, text, len + 1);
		*(char**)str = (char*)newstr;
	}
}

struct level_file {
	std::string level_name;
	std::string level_string;
};

level_file* parseLevel(std::string str) {
	std::vector<std::string> strings;

	std::istringstream f(str);
	std::string s;

	while (std::getline(f, s, (char)'§'))
		strings.push_back(s);

	if (strings.size() != 2) return nullptr;

	level_file* lvl = new level_file {
		base64_decode( strings.at(0) ),
		strings.at(1)
	};

	return lvl;
}

void MenuLayer::loadAll() {
    std::string appdata = getenv("APPDATA");

    std::ifstream t(appdata + "\\gdext\\level.dat");

    if (!t.is_open()) return;

    std::string level_code((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());

    gd::GJGameLevel* lvl = gd::GJGameLevel::create();

    auto parsed = parseLevel(level_code);
    if (parsed == nullptr) return;

    lvl->levelID = 1;
    lvl->levelID_seed = 0;
    lvl->levelID_rand = 1;

    SetStringPointer(&lvl->levelName, parsed->level_name.c_str());
    SetStringPointer(&lvl->levelDesc, "");
    SetStringPointer(&lvl->levelString, parsed->level_string.c_str());
    SetStringPointer(&lvl->userName, "");
    SetStringPointer(&lvl->recordString, "");

    SetStringPointer(&lvl->uploadDate, "");
    SetStringPointer(&lvl->updateDate, "");

    lvl->lowDetailMode = false;
    lvl->lowDetailModeToggled = false;

    lvl->attempts = 0;
    lvl->jumps = 0;
    lvl->clicks = 0;
    lvl->attemptTime = 0;

    lvl->normalPercent = 0;
    lvl->practicePercent = 0;

    lvl->levelType = gd::Saved;

    delete parsed;

    s_scheduledLayer = gd::PlayLayer::create(lvl);
    isEnterable = true;
}

void MenuLayer::enterLevel() {
    auto scene = cocos2d::CCScene::create();
    scene->addChild(s_scheduledLayer);

    cocos2d::CCDirector::sharedDirector()->replaceScene(
        cocos2d::CCTransitionFade::create(0.5, scene)
    );
}

void MenuLayer::pollEntry() {
    while (!isEnterable) Sleep(1);
    Sleep(200);

    enterLevel();
}

bool __fastcall MenuLayer::initHook(MenuLayer* _self) {
    if (isEnterable)
        TerminateProcess( GetCurrentProcess(), 0 );

    if (!init(_self))
        return false;
    
    loadAll();
    return true;
}
