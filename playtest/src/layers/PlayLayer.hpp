#pragma once

#include "../offsets.hpp"

class PlayLayerRef {
public:
    PlayLayerRef(gd::PlayLayer* pl);

    bool&  isDead;
    float& levelLength;
    bool&  isPractice;
    int&   currentAttempt;
    gd::PlayerObject*& player1;
    gd::PlayerObject*& player2;
    double& time;
};