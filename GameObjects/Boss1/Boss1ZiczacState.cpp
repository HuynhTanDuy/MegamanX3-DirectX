#include "Boss1State.h"
#include "Boss1ZiczacState.h"
#include "Boss1.h"
//#include "PlayerFallingState.h"
#include "../../GameComponents/GameLog.h"
#include "../../GameDefines/GameDefine.h"
#include "dinput.h"
Boss1ZiczacState::Boss1ZiczacState(Boss1Data *Boss1Data)
{
	this->mBoss1Data = Boss1Data;
	this->mBoss1Data->Boss1->SetVx(0);
	this->mBoss1Data->Boss1->SetVy(0);
	count = 0;
	time = 50;
}


Boss1ZiczacState::~Boss1ZiczacState()
{

}

void Boss1ZiczacState::Update(float dt)
{

	

}



Boss1State::StateName Boss1ZiczacState::GetState()
{
	return Boss1State::Ziczac;
}

