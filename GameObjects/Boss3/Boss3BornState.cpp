#include "Boss3State.h"
#include "Boss3BornState.h"
#include "Boss3.h"
//#include "PlayerFallingState.h"
#include "../../GameComponents/GameLog.h"
#include "../../GameDefines/GameDefine.h"
#include "dinput.h"
Boss3BornState::Boss3BornState(Boss3Data *Boss3Data)
{
	this->mBoss3Data = Boss3Data;
	this->mBoss3Data->Boss3->SetVx(0);
	this->mBoss3Data->Boss3->SetVy(0);
	count = 0;
	time = 200;
}


Boss3BornState::~Boss3BornState()
{

}

void Boss3BornState::Update(float dt)
{
	if (this->mBoss3Data->Boss3->OnRight())
		this->mBoss3Data->Boss3->SetVx(-200);
	
	if (this->mBoss3Data->Boss3->GetPosition().x <= 19509.3 - 420)
	{
		this->mBoss3Data->Boss3->SetVx(0);
		this->mBoss3Data->Boss3->SetVy(0);
		this->mBoss3Data->Boss3->SetState(new Boss3GenerateBeeState(this->mBoss3Data));
	}
	

}



Boss3State::StateName Boss3BornState::GetState()
{
	return Boss3State::Born;
}

