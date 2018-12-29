#include "Boss1State.h"
#include "Boss1BornState.h"
#include "Boss1.h"
//#include "PlayerFallingState.h"
#include "../../GameComponents/GameLog.h"
#include "../../GameDefines/GameDefine.h"
#include "dinput.h"
Boss1BornState::Boss1BornState(Boss1Data *Boss1Data)
{
	this->mBoss1Data = Boss1Data;
	this->mBoss1Data->Boss1->SetVx(0);
	this->mBoss1Data->Boss1->SetVy(0);
	count = 0;
	time = 50;
}


Boss1BornState::~Boss1BornState()
{

}

void Boss1BornState::Update(float dt)
{
	count++;
	if (count == time)
		this->mBoss1Data->Boss1->SetVx(200);
	if (count == time + 30)
		this->mBoss1Data->Boss1->SetVx(0);
	if (count == time * 8)
		this->mBoss1Data->Boss1->SetState(new Boss1StandingState(this->mBoss1Data));

}



Boss1State::StateName Boss1BornState::GetState()
{
	return Boss1State::Born;
}

