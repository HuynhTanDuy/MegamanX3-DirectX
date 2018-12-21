#pragma once
#include "PlayerState.h"
#include "PlayerStandingShotState.h"
class PlayerStandingState : public PlayerState
{
public:
    PlayerStandingState(PlayerData *playerData);
    ~PlayerStandingState();

    void Update(float dt);

    void HandleKeyboard(std::map<int, bool> keys);

    virtual StateName GetState(); // lay state nhan vat

protected:
};

