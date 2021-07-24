#include "PlayLayer.hpp"

PlayLayerRef::PlayLayerRef(gd::PlayLayer* p) :
    isDead        (offset<bool>(p, 0x39C)),
    levelLength   (offset<float>(p, 0x3B4)),
    isPractice    (offset<bool>(p, 0x495)),
    currentAttempt(offset<int>(p, 0x4A8)),
    player1       (offset<gd::PlayerObject*>(p, 0x228)),
    player2       (offset<gd::PlayerObject*>(p, 0x224)),
    time          (offset<double>(p, 0x450)) 
{}