#pragma once

#include "pch.h"
#include "gjgamelevel.h"

namespace GD_Extern {
	const inline char* base = reinterpret_cast<char*>(GetModuleHandle(0));

	namespace MenuLayer {
		inline int(__thiscall const* init)(cocos2d::CCLayer* MenuLayer) = 
			reinterpret_cast<decltype(init)>(base + 0x1907B0);
	}

	namespace PlayLayer {
		inline int(__thiscall const* create)(cocos2d::CCObject* level) =
			reinterpret_cast<decltype(create)>(base + 0x1FB6D0);
	}

	inline bool init() {
		return base;
	}
}