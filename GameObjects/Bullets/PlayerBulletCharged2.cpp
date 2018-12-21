#include "PlayerBulletCharged2.h"
#include "../../GameObjects/Player/Player.h"


PlayerBulletCharged2::PlayerBulletCharged2() {
	
	BulletMoving = new Animation("Resources/Bullets/PlayerBulletCharged2Moving.png", 1, 1, 1, 0.2f);
	BulletDie = new Animation("Resources/Bullets/PlayerBulletCharged2Die.png", 3, 1, 3, 0.05f);
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
PlayerBulletCharged2::~PlayerBulletCharged2()
{	
}




