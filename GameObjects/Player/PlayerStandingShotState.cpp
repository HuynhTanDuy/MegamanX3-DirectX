#include "PlayerStandingShotState.h"
#include "PlayerStandingState.h"
#include "Player.h"
#include "PlayerFallingState.h"
#include "PlayerSurfingState .h"
#include "PlayerClimbingState .h"
#include "../../GameDefines/GameDefine.h"
#include "dinput.h"
PlayerStandingShotState::PlayerStandingShotState(PlayerData *playerData)
{
    this->mPlayerData = playerData;
    this->mPlayerData->player->SetVx(0);
    this->mPlayerData->player->SetVy(0);
}


PlayerStandingShotState::~PlayerStandingShotState()
{
}

void PlayerStandingShotState::Update(float dt)
{

}

void PlayerStandingShotState::HandleKeyboard(std::map<int, bool> keys)
{
    if (keys[VK_LEFT] || keys[VK_RIGHT])
    {
       this->mPlayerData->player->SetState(new PlayerRunningState(this->mPlayerData));
        return;
    }
	if (keys[0x5A])
	{
		this->mPlayerData->player->SetState(new PlayerSurfingState(this->mPlayerData));
	}
	if (keys[VK_UP])
	{
		this->mPlayerData->player->SetState(new PlayerClimbingState(this->mPlayerData));
	}
	if (keys[0x58]);
	else {
		this->mPlayerData->player->SetState(new PlayerStandingState(this->mPlayerData));
		return;
	}
	
}

PlayerState::StateName PlayerStandingShotState::GetState()
{
    return PlayerState::StandingShot;
}
