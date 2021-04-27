#include "mod.hpp"
#include "layers/MenuLayer.hpp"

bool mod::load() {
    if (MH_Initialize() != MH_OK) [[unlikely]]
        return false;
    
    if (!MenuLayer::loadHook())
        return false;

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) [[unlikely]] {
        MH_Uninitialize();
        return false;
    }
    
    // is this illegal in here?
    MenuLayer::pollEntry();
    
    return true;
}

void mod::unload() {
    MH_DisableHook(MH_ALL_HOOKS);

    MH_Uninitialize();
}
