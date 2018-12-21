#include "PlayerStandingState.h"
#include "Player.h"
//#include "PlayerFallingState.h"
#include "PlayerSurfingState .h"
#include "PlayerClimbingState .h"
#include "../../GameDefines/GameDefine.h"
#include "dinput.h"
PlayerStandingState::PlayerStandingState(PlayerData *playerData)
{
    this->mPlayerData = playerData;
    this->mPlayerData->player->SetVx(0);
    this->mPlayerData->player->SetVy(0);
}


PlayerStandingState::~PlayerStandingState()
{
}

void PlayerStandingState::Update(float dt)
{

}

void PlayerStandingState::HandleKeyboard(std::map<int, bool> keys)
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
	if (keys[VK_UP] || keys[VK_DOWN])
	{
		this->mPlayerData->player->SetState(new PlayerClimbingState(this->mPlayerData));
	}
	
	if (keys[0x58])
	{
		this->mPlayerData->player->SetState(new PlayerStandingShotState(this->mPlayerData));
	}
}

PlayerState::StateName PlayerStandingState::GetState()
{
    return PlayerState::Standing;
}
