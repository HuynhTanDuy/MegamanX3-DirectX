#include "PlayerBullet.h"
#include "../../GameObjects/Player/Player.h"


PlayerBullet::PlayerBullet() {
	
	BulletMoving = new Animation("Resources/Bullets/PlayerBulletMoving.png", 1, 1, 1, 0.2f);
	BulletDie = new Animation("Resources/Bullets/PlayerBulletDie.png", 3, 1, 3, 0.05f);
	//this->playerBulletData = new EnemyData3();
	//this->playerBulletData->PlayerBullet = this;
	this->playerBulletData = new PlayerBulletData();
	this->playerBulletData->PlayerBullet = this;
	
	this->SetVx(0);
	this->SetVy(0);
	this->isDestroyed = false;
	
	this->SetState(new PlayerBulletMovingState(this->playerBulletData));
	//this->SetState(new BulletMovingState(this->playerBulletData));
	//this->mEnemyData->PlayerShot = new PlayerShot(this);
	
	
}
PlayerBullet::~PlayerBullet()
{	
}
void PlayerBullet::Update(float dt)
{
	
	CurrentAnimation->Update(dt);
	Entity::Update(dt);
	//this->mEnemyData->PlayerShot->Update(dt);
	if (this->playerBulletData->PlayerBulletState)
	{
		this->playerBulletData->PlayerBulletState->Update(dt);
	}
	
	
	
}







void PlayerBullet::SetState(BulletState *newState)
{
	delete this->playerBulletData->PlayerBulletState;

	this->playerBulletData->PlayerBulletState = newState;

	this->changeAnimation(newState->GetState()); 

	mCurrentState = newState->GetState();
}
