#include "mod.hpp"
#include "layers/MenuLayer.hpp"

#include "connection.hpp"

Connection* con;

struct PlayerPacketData {
    double time;
    float  x;
    float  y;
};

float lastXSent = 0;

void sendData(double time) {
    float x = offset<float>(cpl->player2, 52);
    float y = offset<float>(cpl->player2, 56);

    if (lastXSent == x) return;
    lastXSent = x;

    PlayerPacketData packet = { time, x, y };
    con->send_packet(21, &packet, sizeof(packet));
}

bool mod::load() {
    if (MH_Initialize() != MH_OK) [[unlikely]]
        return false;
    
    if (!MenuLayer::loadHook())
        return false;

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) [[unlikely]] {
        MH_Uninitialize();
        return false;
    }

    //con = new Connection("127.0.0.1", 12069);
    
    MenuLayer::pollEntry();

    return true;

    double frameDelay  = 1 / 60;
    double timeCounter = 0;

    double lastTime = 0;
    bool isDead = false;

    while (true) {
        double time = cpl->time;

        if (time < lastTime) {
            con->send_packet(22, "", 0);

            timeCounter = 0;
            lastTime = 0;
        }

        timeCounter += time - lastTime;
        lastTime = time;

        if (!cpl->isDead && isDead) {
            con->send_packet(22, "", 0);

            timeCounter = 0;
            lastTime = 0;
        }

        isDead = cpl->isDead;

        if (!isDead) {
            if (timeCounter > frameDelay) sendData(time);
            while (timeCounter > frameDelay) timeCounter = 0;
        }

        Sleep(1);
    }
    
    return true;
}

void mod::unload() {
    MH_DisableHook(MH_ALL_HOOKS);

    MH_Uninitialize();

    con->close();
}

