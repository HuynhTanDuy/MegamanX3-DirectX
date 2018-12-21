#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#include "Enemy1.h"
#include "../../GameComponents/Animation.h"
#include "../../GameComponents/GameGlobal.h"
#include "../../GameComponents/Camera.h"
#include "EnemyData1.h"
#include "../Entity.h"
#include "Enemy1StandingState.h"

class Enemy1 : public Entity {
public: Enemy1();
		~Enemy1();
		enum MoveDirection
		{
			MoveToLeft, //chay tu phai sang trai
			MoveToRight, //chay tu trai sang phai
			None //dung im
		};
		void Update(float dt);

		void Draw(D3DXVECTOR3 position = D3DXVECTOR3(), RECT sourceRect = RECT(), D3DXVECTOR2 scale = D3DXVECTOR2(), D3DXVECTOR2 transform = D3DXVECTOR2(), float angle = 0, D3DXVECTOR2 rotationCenter = D3DXVECTOR2(), D3DXCOLOR colorKey = D3DCOLOR_XRGB(255, 255, 255));

		void SetState(Enemy1State *newState);

		bool mCurrentReverse;
		
		void SetReverse(bool flag);
		
		Enemy1State::StateName Enemy1::getState();

		Enemy1State::StateName mCurrentState;

		bool isLeft;

		void OnCollision(Entity *impactor, Entity::CollisionReturn data, Entity::SideCollisions side);
	
protected:
	EnemyData1 *mEnemyData1;

	Animation *Enemy1Standing,
			  *Enemy1Jumping,
			  *Enemy1Falling,
			  *Enemy1Shooting,	
			  *CurrentAnimation;
	
	void changeAnimation(Enemy1State::StateName state);
	//Animation *bullet;
	//ShotData *PlayerShot;

};