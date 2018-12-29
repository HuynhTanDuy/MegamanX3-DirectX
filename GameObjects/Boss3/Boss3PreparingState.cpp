#include "Boss3State.h"
#include "Boss3PreparingState.h"
#include "Boss3.h"

#include "../../GameComponents/GameLog.h"
#include "../../GameDefines/GameDefine.h"
#include "dinput.h"
Boss3PreparingState::Boss3PreparingState(Boss3Data *Boss3Data)
{
	this->mBoss3Data = Boss3Data;
	this->mBoss3Data->Boss3->SetVx(0);
	this->mBoss3Data->Boss3->SetVy(0);
	count = 0;
	time = 200;
	this->mBoss3Data->attackTime++;
}


Boss3PreparingState::~Boss3PreparingState()
{

}

void Boss3PreparingState::Update(float dt)
{
	count++;
	/*if (count==50)
	{
		this->mBoss3Data->Boss3->SetVy(-100);
	}*/

	if (count == time)
	{
		if (this->mBoss3Data->attackTime==4)
		{
			this->mBoss3Data->Boss3->SetVy(0);
			this->mBoss3Data->Boss3->SetState(new Boss3GenerateBeeState(this->mBoss3Data));
		}
		else
		this->mBoss3Data->Boss3->SetState(new Boss3AttackState(this->mBoss3Data));
	}


}



Boss3State::StateName Boss3PreparingState::GetState()
{
	return Boss3State::Preparing;
}

