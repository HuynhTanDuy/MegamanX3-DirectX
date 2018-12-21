#include "Enemy3.h"
#include "../../GameObjects/Player/Player.h"


Enemy3::Enemy3() {
	
	Enemy3Standing = new Animation("Resources/enemy3/Enemy3Standing.png", 5, 1, 5, 0.2f);
	Enemy3Flying = new Animation("Resources/enemy3/Enemy3Flying.png", 5, 1, 5, 0.2f);
	
	Enemy3Shooting = new Animation("Resources/enemy3/Enemy3Shooting.png", 2, 1, 2, 0.2f);
	//this->mEnemyData3 = new EnemyData3();
	//this->mEnemyData3->Enemy3 = this;
	this->mEnemyData3 = new Enemy3Data();
	this->mEnemyData3->Enemy3 = this;
	
	this->SetVx(0);
	this->SetVy(0);
	
	
	this->SetState(new Enemy3StandingState(this->mEnemyData3));
	//this->SetState(new Enemy3StandingState(this->mEnemyData3));
	//this->mEnemyData->PlayerShot = new PlayerShot(this);
	
	
}
Enemy3::~Enemy3()
{	
}
void Enemy3::Update(float dt)
{
	
	CurrentAnimation->Update(dt);
	Entity::Update(dt);
	//this->mEnemyData->PlayerShot->Update(dt);
	if (this->mEnemyData3->Enemy3State)
	{
		this->mEnemyData3->Enemy3State->Update(dt);
	}
	
	
	
}


void Enemy3::SetReverse(bool flag)
{
	mCurrentReverse = flag;
}



void Enemy3::Draw(D3DXVECTOR3 position, RECT sourceRect, D3DXVECTOR2 scale, D3DXVECTOR2 transform, float angle, D3DXVECTOR2 rotationCenter, D3DXCOLOR colorKey)
{
	
	CurrentAnimation->FlipVertical(mCurrentReverse);
	//CurrentAnimation->SetPosition(this->GetPosition());
	CurrentAnimation->SetPosition(position);
	
	//    CurrentAnimation->Draw(D3DXVECTOR3(posX, posY, 0));
	CurrentAnimation->Draw(position, sourceRect, scale, transform, angle, rotationCenter, colorKey);
	
	
}

void Enemy3::changeAnimation(Enemy3State::StateName state)
{
	switch (state)
	{
	case Enemy3State::Standing:
		CurrentAnimation = Enemy3Standing;
		break;
	case Enemy3State::Flying:
		CurrentAnimation = Enemy3Flying;
		break;
	
	case Enemy3State::Shooting:
		CurrentAnimation = Enemy3Shooting;
		break;
	}

	this->width = CurrentAnimation->GetWidth();
	this->height = CurrentAnimation->GetHeight();
}

void Enemy3::SetState(Enemy3State *newState)
{
	delete this->mEnemyData3->Enemy3State;

	this->mEnemyData3->Enemy3State = newState;

	this->changeAnimation(newState->GetState()); 

	mCurrentState = newState->GetState();
}
Enemy3State::StateName Enemy3::getState()
{
	return mCurrentState;
}
void Enemy3::OnCollision(Entity *impactor, Entity::CollisionReturn data, Entity::SideCollisions side)
{
	this->mEnemyData3->Enemy3State->OnCollision(impactor, side, data);
}