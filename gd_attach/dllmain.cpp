// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "extern/gd.h"
#include "hooks.h"

#include <chrono>
#include <thread>

#include <fstream>
#include <streambuf>

gd::PlayLayer* curr_pl = nullptr;

void showPlayLayer(gd::PlayLayer* pl) {
    curr_pl = pl;
}

DWORD WINAPI my_thread(void* hModule) {
    using namespace cocos2d;

    //Your code goes here
    //====================
    if (!GD_Extern::init()) {
        MessageBox(0, L"ERROR!", L"Failed to obtain internal base address!", MB_OK | MB_ICONERROR);
        return -1;
    }

    if (MH_Initialize() != MH_OK) {
        MessageBox(0, L"ERROR!", L"Failed to initialize MinHook!", MB_OK | MB_ICONERROR);
        return -1;
    }

    if (MH_CreateHook(
        GD_Extern::MenuLayer::init,
        hooks::MenuLayer::init,
        reinterpret_cast<LPVOID*>(&gates::MenuLayer::init)) != MH_OK) {
        MessageBox(0, L"ERROR!", L"Failed to create hook!", MB_OK | MB_ICONERROR);
        MH_Uninitialize();
        return -1;
    }

    if (MH_EnableHook(GD_Extern::MenuLayer::init) != MH_OK) {
        MessageBox(0, L"ERROR!", L"Failed to enable hook!", MB_OK | MB_ICONERROR);
        MH_Uninitialize();
        return -1;
    }

    while (curr_pl == nullptr) std::this_thread::sleep_for(std::chrono::milliseconds(400));

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    auto dir = CCDirector::sharedDirector();

    auto scene = CCScene::create();
    scene->addChild((CCNode*)curr_pl);

    dir->replaceScene(
        CCTransitionFade::create(0.5, scene)
    );

    //This line will dettach your DLL when executed. Remove if needed
    //FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0x1000, my_thread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

