#include "Boss3State.h"
#include "Boss3AttackState.h"
#include "Boss3.h"
//#include "PlayerFallingState.h"
#include "../../GameComponents/GameLog.h"
#include "../../GameDefines/GameDefine.h"
#include "dinput.h"
Boss3AttackState::Boss3AttackState(Boss3Data *Boss3Data)
{
	this->mBoss3Data = Boss3Data;
	this->mBoss3Data->Boss3->SetVx(0);
	this->mBoss3Data->Boss3->SetVy(0);
	count = 0;
	time = 300;
	playerPosX = this->mBoss3Data->Player->GetPosition().x;
	playerPosY = this->mBoss3Data->Player->GetPosition().y;
	distanceX = this->mBoss3Data->Boss3->GetPosition().x - playerPosX;
	distanceY = playerPosY-this->mBoss3Data->Boss3->GetPosition().y;
	this->mBoss3Data->mCurrentReverse = !this->mBoss3Data->mCurrentReverse;
}


Boss3AttackState::~Boss3AttackState()
{

}

void Boss3AttackState::Update(float dt)
{
	
	
	
	count++;
	if (this->mBoss3Data->mCurrentReverse==false)
	
	if (this->mBoss3Data->Boss3->GetPosition().x >= playerPosX) // tan cong muc tieu ben trai
	{
		if (this->mBoss3Data->Boss3->GetPosition().y <= playerPosY+200)
		{
			this->mBoss3Data->Boss3->SetVy(distanceY);
			this->mBoss3Data->Boss3->SetVx(-distanceX);
			

			
			
		}
	}
	else {
		
		this->mBoss3Data->Boss3->SetPosition(this->mBoss3Data->Boss3->GetPosition().x, this->mBoss3Data->Boss3->GetPosition().y);
		ReturnPointLeft();
		
		//count++;
	}
	
	else {
	if (this->mBoss3Data->Boss3->GetPosition().x <= playerPosX) // tan cong muc tieu ben phai 
	{
		if (this->mBoss3Data->Boss3->GetPosition().y <= playerPosY+200)
		{
			this->mBoss3Data->Boss3->SetVy(distanceY);
			this->mBoss3Data->Boss3->SetVx(-distanceX);
			
		
		}
	}
	else {
		this->mBoss3Data->Boss3->SetPosition(this->mBoss3Data->Boss3->GetPosition().x, this->mBoss3Data->Boss3->GetPosition().y);
		
		ReturnPointRight();
		
	}

	}
	/*if (this->mBoss3Data->attackTime == 4)
		this->mBoss3Data->Boss3->SetState(new Boss3GenerateBeeState(this->mBoss3Data));*/
	
	

}



Boss3State::StateName Boss3AttackState::GetState()
{
	return Boss3State::Attack;
}

void Boss3AttackState::ReturnPointLeft()
{
	this->mBoss3Data->Boss3->SetVx(-50);
	this->mBoss3Data->Boss3->SetVy(-100);
	if (mBoss3Data->Boss3->GetPosition().y <= 3980)
	{
		this->mBoss3Data->Boss3->SetVy(0);

	     
	}
	if (this->mBoss3Data->Boss3->GetPosition().x <= 19509.3 - 400)
		this->mBoss3Data->Boss3->SetVx(0);
	if(count==time)
	this->mBoss3Data->Boss3->SetState(new Boss3PreparingState(this->mBoss3Data));
	
}

void Boss3AttackState::ReturnPointRight()
{
	this->mBoss3Data->Boss3->SetVx(100);
	this->mBoss3Data->Boss3->SetVy(-100);
	if (mBoss3Data->Boss3->GetPosition().y <= 3980)
	
	
		this->mBoss3Data->Boss3->SetVy(0);
		
    if (mBoss3Data->Boss3->GetPosition().x >= (19509.3+30))
	    this->mBoss3Data->Boss3->SetVx(0);
		
	
	if (count == time)
		this->mBoss3Data->Boss3->SetState(new Boss3PreparingState(this->mBoss3Data));
}

