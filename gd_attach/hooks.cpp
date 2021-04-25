#include "pch.h"

#include "hooks.h"
#include "extern/gd.h"

#include <fstream>
#include <streambuf>

#include <chrono>
#include <thread>

#include "base64.h"

gd::PlayLayer* play_layer;

struct level_file {
	std::string level_name;
	std::string level_string;
};

level_file* parseLevel(std::string str) {
	std::vector<std::string> strings;

	std::istringstream f(str);
	std::string s;

	while (getline(f, s, '§'))
		strings.push_back(s);

	if (strings.size() != 2) return nullptr;

	level_file* lvl = new level_file {
		base64_decode( strings.at(0) ),
		strings.at(1)
	};

	return lvl;
}

bool level_loaded = false;

namespace hooks {
	namespace MenuLayer {

		int __fastcall init(cocos2d::CCLayer* MenuLayer) {
			using namespace cocos2d;
			if (level_loaded) {
				TerminateProcess(GetCurrentProcess(), 0);
				return 0;
			}

			int ret = gates::MenuLayer::init(MenuLayer);

			std::string appdata = getenv("APPDATA");

			std::ifstream t(appdata + "\\gdext\\level.dat");

			if (!t.is_open()) return gates::MenuLayer::init(MenuLayer);

			std::string level_code((std::istreambuf_iterator<char>(t)),
				std::istreambuf_iterator<char>());

			gd::GJGameLevel* lvl = gd::GJGameLevel::create();

			auto parsed = parseLevel(level_code);
			if (parsed == nullptr) return gates::MenuLayer::init(MenuLayer);

			lvl->levelID = 1;
			lvl->levelID_seed = 0;
			lvl->levelID_rand = 1;

			SetStringPointer((int*)&lvl->levelName, parsed->level_name.c_str());
			SetStringPointer((int*)&lvl->levelDesc, "");
			SetStringPointer((int*)&lvl->levelString, parsed->level_string.c_str());
			SetStringPointer((int*)&lvl->userName, "");
			SetStringPointer((int*)&lvl->recordString, "");

			SetStringPointer((int*)&lvl->uploadDate, "");
			SetStringPointer((int*)&lvl->updateDate, "");

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

			showPlayLayer((gd::PlayLayer*)GD_Extern::PlayLayer::create(lvl));
			level_loaded = true;
			return ret;
		}

	}
}
