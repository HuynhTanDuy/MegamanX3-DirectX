#include "BrickDieState.h"


#include "Brick.h"
#include "../../GameComponents/GameLog.h"
#include "../../GameDefines/GameDefine.h"
#include "dinput.h"
BrickDieState::BrickDieState(BrickData *playerData)
{
	this->mBrickData = playerData;
	count = 0;
}


BrickDieState::~BrickDieState()
{
}

void BrickDieState::Update(float dt)
{
	count++;
	if (count == 10) this->mBrickData->Brick->isDeleted = true;
	
	

	
}



BrickState::StateName BrickDieState::GetState()
{
	return BrickState::Die;
}
