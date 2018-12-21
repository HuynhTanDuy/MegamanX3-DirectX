#pragma once
#include "PlayerState.h"

class PlayerStandingShotState : public PlayerState
{
public:
    PlayerStandingShotState(PlayerData *playerData);
    ~PlayerStandingShotState();

    void Update(float dt);

    void HandleKeyboard(std::map<int, bool> keys);

    virtual StateName GetState(); // lay state nhan vat

protected:
};

