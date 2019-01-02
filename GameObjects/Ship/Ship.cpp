#include "Ship.h"
Ship::Ship() {

	//Ship = new Animation("Resources/Enemy/Ship.png", 5, 1, 5, 0.035f);
	CurrentAnimation = new Animation("Resources/ship.png", 1, 1, 1, 0.0f);
	this->width = CurrentAnimation->GetWidth();
	this->height = CurrentAnimation->GetHeight();
	count = 0;
	time = 200;
	
}
void Ship::Update(float dt)
{
	CurrentAnimation->Update(dt);
	Entity::Update(dt);
	count++;
	if (count < time) this->SetVy(100);
	else {
		this->SetVy(0);
		
	}
	
}

void Ship::SetReverse(bool flag)
{
	mCurrentReverse = flag;
}

void Ship::Draw(D3DXVECTOR3 position, RECT sourceRect, D3DXVECTOR2 scale, D3DXVECTOR2 transform, float angle, D3DXVECTOR2 rotationCenter, D3DXCOLOR colorKey)
{
	CurrentAnimation->FlipVertical(mCurrentReverse);
	//CurrentAnimation->SetPosition(this->GetPosition());
	CurrentAnimation->SetPosition(position);

	//    CurrentAnimation->Draw(D3DXVECTOR3(posX, posY, 0));
	CurrentAnimation->Draw(position, sourceRect, scale, transform, angle, rotationCenter, colorKey);
}
void Ship::onFired()
{

}