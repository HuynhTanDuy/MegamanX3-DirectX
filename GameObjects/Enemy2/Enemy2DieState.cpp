
#include "Enemy2DieState.h"
#include "Enemy2.h"
//#include "PlayerFallingState.h"
#include "../../GameComponents/GameLog.h"
#include "../../GameDefines/GameDefine.h"
#include "dinput.h"
Enemy2DieState::Enemy2DieState(EnemyData2 *playerData)
{
    this->mEnemyData2 = playerData;
    this->mEnemyData2->Enemy2->SetVx(0);
	this->mEnemyData2->Enemy2->isDestroyed = true;
	
}


Enemy2DieState::~Enemy2DieState()
{
}

void Enemy2DieState::Update(float dt)
{
	
	dem++;
	if (dem == 10) this->mEnemyData2->Enemy2->isDeleted = true;
	
}



Enemy2DieState::StateName Enemy2DieState::GetState()
{
    return Enemy2DieState::Die;
}
