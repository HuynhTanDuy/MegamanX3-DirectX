#include "BossHP.h"
#include <iostream>
#include <string>
#include "../../../GameComponents/GameLog.h"
BossHP::BossHP()
{
	
	CurrentAnimation = new Animation("Resources/BossHP/BossHP16.png", 1, 1, 1, 0.0f);

	
}
void BossHP::Update(float dt,int hp)
{
	switch (hp)
	{
	case 16:
		CurrentAnimation = new Animation("Resources/HPTaskBar/bossHP/bossHP.png", 1, 1, 1, 0.0f);
		break;
	case 14:
		CurrentAnimation = new Animation("Resources/HPTaskBar/bossHP/bossHP10.png", 1, 1, 1, 0.0f);
		break;
	default:
		CurrentAnimation = new Animation("Resources/HPTaskBar/bossHP/bossHP10.png", 1, 1, 1, 0.0f);
		break;
	}

	
	
	
	
	//CurrentAnimation = new Animation("Resources/BossHP"+HP, 1, 1, 1, 0.0f);
	
	
}


void BossHP::Draw(D3DXVECTOR3 position, RECT sourceRect, D3DXVECTOR2 scale, D3DXVECTOR2 transform, float angle, D3DXVECTOR2 rotationCenter, D3DXCOLOR colorKey)
{

	
	//CurrentAnimation->SetPosition(this->GetPosition());
	//CurrentAnimation->SetPosition(D3DXVECTOR3(GameGlobal::GetWidth() / 2, GameGlobal::GetHeight() / 2, 0));

	//    CurrentAnimation->Draw(D3DXVECTOR3(posX, posY, 0));
	CurrentAnimation->Draw(D3DXVECTOR3(GameGlobal::GetWidth() / 2+400, GameGlobal::GetHeight() / 2-270, 0));
}
