#include "Enemy1ShootingState.h"
#include "Enemy1StandingState.h"

#include "../../GameDefines/GameDefine.h"
#include "../../GameComponents/GameLog.h"

Enemy1ShootingState::Enemy1ShootingState(EnemyData1 *playerData)
{
    this->mEnemyData1 = playerData;
	this->mEnemyData1->Enemy1->SetVx(0);
    this->mEnemyData1->Enemy1->SetVy(0);
	count = 0;
	time = 200;
    
	    noPressed = false;
}


Enemy1ShootingState::~Enemy1ShootingState()
{

}

void Enemy1ShootingState::Update(float dt)
{
	count++; 
	if (count == time) this->mEnemyData1->Enemy1->SetState(new Enemy1JumpingState(this->mEnemyData1));
	

 
}



Enemy1State::StateName Enemy1ShootingState::GetState()
{
    return Enemy1State::Shooting;
}
