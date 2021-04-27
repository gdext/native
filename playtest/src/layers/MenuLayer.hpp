#pragma once

#include "../offsets.hpp"

class MenuLayer : public cocos2d::CCLayer {
    public:
        // this is terrible code (ew static global)
        // i agree?
        inline static gd::PlayLayer*   s_scheduledLayer;
        inline static bool isEnterable = false;

    private:
        static inline bool (__thiscall* init)(MenuLayer*);
        static bool __fastcall initHook(MenuLayer*);
    
        static void enterLevel();

    public:
        static void loadAll();
        static void pollEntry();

        static inline bool loadHook() {
            if (MH_CreateHook(
                (PVOID)(gd::base + 0x1907b0),
                as<LPVOID>(MenuLayer::initHook),
                as<LPVOID*>(&MenuLayer::init)
            ) != MH_OK)
                return false;
            
            return true;
        }
};
