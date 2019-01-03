
#include "Boss2DieState.h"
#include "Boss2.h"
//#include "PlayerFallingState.h"
#include "../../GameComponents/GameLog.h"
#include "../../GameDefines/GameDefine.h"
#include "dinput.h"
Boss2DieState::Boss2DieState(Boss2Data *playerData)
{
    this->mBoss2Data = playerData;
    this->mBoss2Data->Boss2->SetVx(0);
	this->mBoss2Data->Boss2->isDestroyed = true;
	dem = 0;
}


Boss2DieState::~Boss2DieState()
{
}

void Boss2DieState::Update(float dt)
{
	
	dem++;
	if (dem == 10) this->mBoss2Data->Boss2->isDeleted = true;
	
}



Boss2DieState::StateName Boss2DieState::GetState()
{
    return Boss2DieState::Die;
}
