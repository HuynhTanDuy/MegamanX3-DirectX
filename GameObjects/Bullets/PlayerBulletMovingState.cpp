#include "PlayerBulletMovingState.h"
#include "PlayerBullet.h"
//#include "PlayerFallingState.h"
#include "../../GameComponents/GameLog.h"
#include "../../GameDefines/GameDefine.h"
#include "dinput.h"
PlayerBulletMovingState::PlayerBulletMovingState(PlayerBulletData *playerData)
{
    this->playerBulletData = playerData;
	if (this->playerBulletData->PlayerBullet->mCurrentReverse) 	this->playerBulletData->PlayerBullet->SetVx(-Define::PLAYER_BULLET_VELOCITY);
	else this->playerBulletData->PlayerBullet->SetVx(Define::PLAYER_BULLET_VELOCITY);
   
   
	count = 0;
	time = 400;
}


PlayerBulletMovingState::~PlayerBulletMovingState()
{
}

void PlayerBulletMovingState::Update(float dt)
{
	
	count++;
	if (count == time) this->playerBulletData->PlayerBullet->SetState(new PlayerBulletDieState(this->playerBulletData));
}



BulletState::StateName PlayerBulletMovingState::GetState()
{
    return BulletState::Moving;
}
