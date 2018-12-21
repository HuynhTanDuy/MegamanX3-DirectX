#include "Enemy1FallingState.h"
#include "Enemy1StandingState.h"

#include "../../GameDefines/GameDefine.h"
#include "../../GameComponents/GameLog.h"

Enemy1FallingState::Enemy1FallingState(EnemyData1 *playerData)
{
    this->mEnemyData1 = playerData;
    this->mEnemyData1->Enemy1->SetVy(-Define::ENEMY1_JUMP_VELOCITY);

    acceleratorY = -10.0f;
    acceleratorX = 0.0f;
	    noPressed = false;
}


Enemy1FallingState::~Enemy1FallingState()
{

}

void Enemy1FallingState::Update(float dt)
{
	//GAMELOG("vy :%d", mEnemyData1->Enemy1->GetVy());
	if (mEnemyData1->Enemy1->GetVy() > 0 )
	{
		this->mEnemyData1->Enemy1->AddVy(acceleratorY );
		//this->mEnemyData1->Enemy1->AddVx(acceleratorX * 3);
	}
	else mEnemyData1->Enemy1->SetState(new Enemy1StandingState(this->mEnemyData1));
    

 
}

void Enemy1FallingState::OnCollision(Entity *impactor, Entity::SideCollisions side, Entity::CollisionReturn data)
{
	switch (side)
	{
	case Entity::Left:
	{
		this->mEnemyData1->Enemy1->AddPosition(data.RegionCollision.right - data.RegionCollision.left, 0);
		this->mEnemyData1->Enemy1->SetVx(0);
		break;
	}

	case Entity::Right:
	{
		this->mEnemyData1->Enemy1->AddPosition(-(data.RegionCollision.right - data.RegionCollision.left), 0);
		this->mEnemyData1->Enemy1->SetVx(0);
		break;
	}

	case Entity::TopRight: case Entity::TopLeft: case Entity::Top:
	{
		this->mEnemyData1->Enemy1->AddPosition(0, data.RegionCollision.bottom - data.RegionCollision.top);
		this->mEnemyData1->Enemy1->SetVy(0);
		break;
	}

	case Entity::BottomRight: case Entity::BottomLeft: case Entity::Bottom:
	{
		this->mEnemyData1->Enemy1->AddPosition(0, -(data.RegionCollision.bottom - data.RegionCollision.top));
	}

	default:
		break;
	}
}

Enemy1State::StateName Enemy1FallingState::GetState()
{
    return Enemy1State::Falling;
}
