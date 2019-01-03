#include "Boss3State.h"

#include "Boss3.h"
#include "Boss3ZiczacBeeState.h"
#include "../../GameComponents/GameLog.h"
#include "../../GameDefines/GameDefine.h"
#include "dinput.h"
Boss3GenerateBeeState::Boss3GenerateBeeState(Boss3Data *Boss3Data)
{
	
	this->mBoss3Data = Boss3Data;
	this->mBoss3Data->Boss3->SetVx(0);
	this->mBoss3Data->Boss3->SetVy(0);
	this->mBoss3Data->generateBee++;
	count = 0;
	time = 300;
	this->mBoss3Data->BeeRight = !this->mBoss3Data->BeeRight;
	this->mBoss3Data->attackTime2 += 2;
}


Boss3GenerateBeeState::~Boss3GenerateBeeState()
{

}

void Boss3GenerateBeeState::Update(float dt)
{
	
	count++;
	if (count == time)
		if (this->mBoss3Data->generateBee == 1)
			this->mBoss3Data->Boss3->SetState(new Boss3BornState(this->mBoss3Data));//xong
	
		else
	    this->mBoss3Data->Boss3->SetState(new Boss3ZiczacBeeState(this->mBoss3Data));

	
	/*count++;
	if (count == time)
		this->mBoss3Data->Boss3->SetState(new Boss3PreparingState(this->mBoss3Data));*/
	

}



Boss3State::StateName Boss3GenerateBeeState::GetState()
{
	return Boss3State::GenerateBee;
}

	