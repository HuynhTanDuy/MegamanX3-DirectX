#include "DemoScene.h"
#include "../GameDefines/GameDefine.h"


DemoScene::DemoScene()
{
    LoadContent();
	
	this->outfile.open("quadtree.txt");
}

void DemoScene::LoadContent()
{
	mDebugDraw = new GameDebugDraw();
    //set mau backcolor cho scene o day la mau xanh
    mBackColor = 0x54acd2; 
	/*map = new Animation("Resources/background.png", 1, 1, 1, 0.0f);
	map->SetScale(D3DXVECTOR2(6, 6));*/
    mPlayer = new Player();
	
    
	//mPlayer->SetPosition(GameGlobal::GetWidth() / 2, GameGlobal::GetHeight()/2);
	//map = new Map("Resources/marioworld1-1.tmx");

	map = new Map("Resources/map1.tmx",mPlayer);
	//mPlayer->SetPosition(GameGlobal::GetWidth() / 2, GameGlobal::GetHeight()+500);
	//mPlayer->SetPosition(19509.3,4180); //boss 3
	// mPlayer->SetPosition(6304,2294.67); //boss 1
	mPlayer->SetPosition(14417.3,2254.67); //boss2
	camera = new Camera(GameGlobal::GetWidth(), GameGlobal::GetHeight());
	camera->SetPosition(GameGlobal::GetWidth()/2, GameGlobal::GetHeight()/2);

	map->SetCamera(camera);
	hpTaskBar = new HpTaskBar();
	
	
}

void DemoScene::Update(float dt)
{
	checkCollision();
	map->Update(dt);
	
    mPlayer->HandleKeyboard(keys);
    mPlayer->Update(dt);
	
	//mEnemy1->Update(dt);
	camera->SetPosition(mPlayer->GetPosition());
	//saveQuadTree(map->GetQuadTree());
	
	//CheckCameraAndWorldMap();
	for (size_t i = 0; i < mPlayer->mListPlayerBullet.size(); i++)
	{
		mPlayer->mListPlayerBullet.at(i)->Update(dt);
	}
	hpTaskBar->Update(dt,mPlayer->HP);
	
}

void DemoScene::Draw()
{
	
	map->Draw();
    mPlayer->Draw();
	hpTaskBar->Draw();
	//mEnemy1->Draw(mEnemy1->GetPosition(), RECT(), D3DXVECTOR2(), trans);
	
	//DrawQuadtree(map->GetQuadTree());
	
	//DrawCollidable();
}

void DemoScene::OnKeyDown(int keyCode)
{
    keys[keyCode] = true;

    mPlayer->OnKeyPressed(keyCode);
}

void DemoScene::OnKeyUp(int keyCode)
{
    keys[keyCode] = false;
	mPlayer->OnKeyUp(keyCode);
}

void DemoScene::OnMouseDown(float x, float y)
{
}

void DemoScene::CheckCameraAndWorldMap()
{
	camera->SetPosition(mPlayer->GetPosition());

	if (camera->GetBound().left < 0)
	{
		//vi position cua camera ma chinh giua camera
		//luc nay o vi tri goc ben trai cua the gioi thuc
		camera->SetPosition(camera->GetWidth() / 2, camera->GetPosition().y);
	}

	if (camera->GetBound().right > map->GetWidth())
	{
		//luc nay cham goc ben phai cua the gioi thuc
		camera->SetPosition(map->GetWidth() - camera->GetWidth() / 2,
			camera->GetPosition().y);
	}

	if (camera->GetBound().top < 0)
	{
		//luc nay cham goc tren the gioi thuc
		camera->SetPosition(camera->GetPosition().x, camera->GetHeight() / 2);
	}

	if (camera->GetBound().bottom > map->GetHeight())
	{
		//luc nay cham day cua the gioi thuc
		camera->SetPosition(camera->GetPosition().x,
			map->GetHeight() - camera->GetHeight() / 2);
	}
}

void DemoScene::checkCollision()
{
	mCollidable.clear();

	/*su dung de kiem tra xem khi nao mario khong dung tren 1 object hoac
	dung qua sat mep trai hoac phai cua object do thi se chuyen state la falling*/
	int widthBottom = 0;

	vector<Entity*> listCollision;

	map->GetQuadTree()->getEntitiesCollideAble(listCollision, mPlayer);

	for (auto child : listCollision)
	{
		mCollidable.push_back(child);
	}

	for (size_t i = 0; i < listCollision.size(); i++)
	{
		Entity::CollisionReturn r = GameCollision::RecteAndRect(mPlayer->GetBound(),
			listCollision.at(i)->GetBound()); //xac dinh Rect va cham

		if (r.IsCollided)
		{
			//lay phia va cham cua Entity so voi Player
			Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(mPlayer, r);

			//lay phia va cham cua Player so voi Entity
			Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollision.at(i), r);

			//goi den ham xu ly collision cua Player va Entity
			mPlayer->OnCollision(listCollision.at(i), r, sidePlayer);
			listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);

			//kiem tra neu va cham voi phia duoi cua Player 
			if (sidePlayer == Entity::Bottom || sidePlayer == Entity::BottomLeft
				|| sidePlayer == Entity::BottomRight)
			{
				//kiem cha do dai ma mario tiep xuc phia duoi day
				int bot = r.RegionCollision.right - r.RegionCollision.left;

				if (bot > widthBottom)
					widthBottom = bot;
			}
		}
	}
	
	//Neu megaman dung ngoai mep thi luc nay cho megaman rot xuong duoi dat    
	if (widthBottom < Define::PLAYER_BOTTOM_RANGE_FALLING && widthBottom !=0)
	{
		
		mPlayer->OnNoCollisionWithBottom();
		//LOG(widthBottom);
	}
	if (mPlayer->mCurrentState == PlayerState::Surfing)
		if (widthBottom < Define::PLAYER_BOTTOM_RANGE_FALLING)
		{
			mPlayer->OnNoCollisionWithBottom();
			//LOG(widthBottom);
		}

#pragma region XU LY VA CHAM CUA ENEMY1
	//xu ly va cham voi enemy, duyet listenemy
	vector<Enemy1*> listEnemy;
	listEnemy = map->mListEnemy1;
	for (size_t i = 0; i < listEnemy.size(); i++)
	{
		Entity::CollisionReturn r = GameCollision::RecteAndRect(mPlayer->GetBound(),
			listEnemy.at(i)->getEntity()->GetBound()); //xac dinh Rect va cham
		
		if (r.IsCollided && mPlayer->nobody==false)
		{
			//lay phia va cham cua Entity so voi Player
			Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(mPlayer, r);

			//lay phia va cham cua Player so voi Entity
			//Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollision.at(i), r);

			//goi den ham xu ly collision cua Player va Entity
			mPlayer->OnCollisionWithEnemy();
			//listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);

			
		}
		
		//XU LY VA CHAM CUA BULLET ENEMY VS PLAYER
		for (size_t j = 0; j <listEnemy[i]->mListEnemy1Bullet.size();j++  )
		{
			Entity::CollisionReturn r = GameCollision::RecteAndRect(mPlayer->GetBound(),
				listEnemy.at(i)->mListEnemy1Bullet[j]->getEntity()->GetBound()); //xac dinh Rect va cham
			if (r.IsCollided && mPlayer->nobody == false)
			{
				mPlayer->OnCollisionWithEnemy();
			}
		}
	}

	//XU LY VA CHAM CUA ENEMY VS MAP
	for (size_t j = 0; j < listEnemy.size(); j++)
	{
		vector<Entity*> listCollisionEnemyvsMap;

		map->GetQuadTree()->getEntitiesCollideAble(listCollisionEnemyvsMap, listEnemy[j]);

		

		for (size_t i = 0;i <listCollisionEnemyvsMap.size(); i++)
		{
			Entity::CollisionReturn r = GameCollision::RecteAndRect(listEnemy[j]->getEntity()->GetBound(),
				listCollisionEnemyvsMap.at(i)->GetBound()); //xac dinh Rect va cham

			if (r.IsCollided)
			{
				//lay phia va cham cua Entity so voi Player
				Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(listEnemy[j], r);

				//lay phia va cham cua Player so voi Entity
				//Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollision.at(i), r);

				//goi den ham xu ly collision cua Player va Entity
				map->mListEnemy1[j] ->OnCollision(listCollisionEnemyvsMap.at(i), r, sidePlayer);
				//listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);

				//kiem tra neu va cham voi phia duoi cua Player 
				
			}
		}
		//XU LY VA CHAM CUA ENEMY1 VS BULLET
		for (size_t i = 0; i < mPlayer->mListPlayerBullet.size(); i++)
		{
			if (mPlayer->mListPlayerBullet.at(i))
			{
				Entity::CollisionReturn r = GameCollision::RecteAndRect(listEnemy[j]->GetBound(),
					mPlayer->mListPlayerBullet.at(i)->GetBound());
				if (r.IsCollided)
				{
					mPlayer->mListPlayerBullet.at(i)->OnCollision();
					listEnemy[j]->OnCollissionWithBullet(mPlayer->mListPlayerBullet.at(i)->damage);
				}
			}
		}
	}

	
#pragma endregion

#pragma region XU LY VA CHAM ENEMY2
	vector<Enemy2*> listEnemy2;
	listEnemy2 = map->mListEnemy2;
	for (size_t i = 0; i < listEnemy2.size(); i++)
	{
		Entity::CollisionReturn r = GameCollision::RecteAndRect(mPlayer->GetBound(),
			listEnemy2.at(i)->getEntity()->GetBound()); //xac dinh Rect va cham

		if (r.IsCollided && mPlayer->nobody == false)
		{
			//lay phia va cham cua Entity so voi Player
			Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(mPlayer, r);

			//lay phia va cham cua Player so voi Entity
			//Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollision.at(i), r);

			//goi den ham xu ly collision cua Player va Entity
			mPlayer->OnCollisionWithEnemy();
			//listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);


		}
		//XU LY VA CHAM CUA BULLET ENEMY VS PLAYER
		for (size_t j = 0; j < listEnemy2[i]->mListEnemy2Bullet.size(); j++)
		{
			Entity::CollisionReturn r = GameCollision::RecteAndRect(mPlayer->GetBound(),
				listEnemy2.at(i)->mListEnemy2Bullet[j]->getEntity()->GetBound()); //xac dinh Rect va cham
			if (r.IsCollided && mPlayer->nobody == false)
			{
				mPlayer->OnCollisionWithEnemy();
			}
		}
	}

	//XU LY VA CHAM CUA ENEMY VS MAP
	for (size_t j = 0; j < listEnemy2.size() - 1; j++)
	{
		vector<Entity*> listCollisionEnemyvsMap;

		map->GetQuadTree()->getEntitiesCollideAble(listCollisionEnemyvsMap, listEnemy2[j]);



		for (size_t i = 0; i < listCollisionEnemyvsMap.size(); i++)
		{
			Entity::CollisionReturn r = GameCollision::RecteAndRect(listEnemy2[j]->getEntity()->GetBound(),
				listCollisionEnemyvsMap.at(i)->GetBound()); //xac dinh Rect va cham

			if (r.IsCollided)
			{
				//lay phia va cham cua Entity so voi Player
				Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(listEnemy2[j], r);

				//lay phia va cham cua Player so voi Entity
				//Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollision.at(i), r);

				//goi den ham xu ly collision cua Player va Entity
				map->mListEnemy2[j]->OnCollision(listCollisionEnemyvsMap.at(i), r, sidePlayer);
				//listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);

				//kiem tra neu va cham voi phia duoi cua Player 

			}
		}
		//XU LY VA CHAM CUA ENEMY2 VS BULLET
		for (size_t i = 0; i < mPlayer->mListPlayerBullet.size(); i++)
		{
			if (mPlayer->mListPlayerBullet.at(i))
			{
				Entity::CollisionReturn r = GameCollision::RecteAndRect(listEnemy2[j]->GetBound(),
					mPlayer->mListPlayerBullet.at(i)->GetBound());
				if (r.IsCollided)
				{
					mPlayer->mListPlayerBullet.at(i)->OnCollision();
					listEnemy2[j]->OnCollissionWithBullet(mPlayer->mListPlayerBullet.at(i)->damage);
				}
			}
		}
	}
#pragma endregion

#pragma region XU LY VA CHAM CUA ENEMY3
	//xu ly va cham voi enemy, duyet listenemy
	vector<Enemy3*> listEnemy3;
	listEnemy3 = map->mListEnemy3;
	for (size_t i = 0; i < listEnemy3.size(); i++)
	{
		Entity::CollisionReturn r = GameCollision::RecteAndRect(mPlayer->GetBound(),
			listEnemy3.at(i)->getEntity()->GetBound()); //xac dinh Rect va cham

		if (r.IsCollided && mPlayer->nobody == false)
		{
			//lay phia va cham cua Entity so voi Player
			Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(mPlayer, r);

			//lay phia va cham cua Player so voi Entity
			//Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollision.at(i), r);

			//goi den ham xu ly collision cua Player va Entity
			mPlayer->OnCollisionWithEnemy();
			//listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);


		}
		//XU LY VA CHAM CUA BULLET ENEMY VS PLAYER
		for (size_t j = 0; j < listEnemy3[i]->mListEnemy3Bullet.size(); j++)
		{
			Entity::CollisionReturn r = GameCollision::RecteAndRect(mPlayer->GetBound(),
				listEnemy3.at(i)->mListEnemy3Bullet[j]->getEntity()->GetBound()); //xac dinh Rect va cham
			if (r.IsCollided && mPlayer->nobody == false)
			{
				mPlayer->OnCollisionWithEnemy();
			}
		}
	}

	//XU LY VA CHAM CUA ENEMY VS MAP
	for (size_t j = 0; j < listEnemy3.size(); j++)
	{
		vector<Entity*> listCollisionEnemyvsMap;

		map->GetQuadTree()->getEntitiesCollideAble(listCollisionEnemyvsMap, listEnemy3[j]);



		for (size_t i = 0; i < listCollisionEnemyvsMap.size(); i++)
		{
			Entity::CollisionReturn r = GameCollision::RecteAndRect(listEnemy3[j]->getEntity()->GetBound(),
				listCollisionEnemyvsMap.at(i)->GetBound()); //xac dinh Rect va cham

			if (r.IsCollided)
			{
				//lay phia va cham cua Entity so voi Player
				Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(listEnemy3[j], r);

				//lay phia va cham cua Player so voi Entity
				//Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollision.at(i), r);

				//goi den ham xu ly collision cua Player va Entity
				//map->mListEnemy3[j]->OnCollision(listCollisionEnemyvsMap.at(i), r, sidePlayer);
				//listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);

				//kiem tra neu va cham voi phia duoi cua Player 

			}
		}
		//XU LY VA CHAM CUA ENEMY3 VS BULLET
		for (size_t i = 0; i < mPlayer->mListPlayerBullet.size(); i++)
		{
			if (mPlayer->mListPlayerBullet.at(i))
			{
				Entity::CollisionReturn r = GameCollision::RecteAndRect(listEnemy3[j]->GetBound(),
					mPlayer->mListPlayerBullet.at(i)->GetBound());
				if (r.IsCollided)
				{
					mPlayer->mListPlayerBullet.at(i)->OnCollision();
					listEnemy3[j]->OnCollissionWithBullet(mPlayer->mListPlayerBullet.at(i)->damage);
				}
			}
		}
	}
#pragma endregion

#pragma region XU LY VA CHAM CUA ELEVATOR
	Entity::CollisionReturn r = GameCollision::RecteAndRect(mPlayer->GetBound(),
		map->elevator->GetBound());
	if (r.IsCollided)
	{
		map->elevator->SetVy(-50);
		//mPlayer->SetVy(-50);
		Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(mPlayer, r);
		mPlayer->OnCollision(map->elevator, r, sidePlayer);
		
	}
#pragma endregion	

#pragma region XU LY VA CHAM CUA DOOR
	for (size_t i = 0; i < map->mListDoor.size(); i++)
	{
		Entity::CollisionReturn r1 = GameCollision::RecteAndRect(mPlayer->GetBound(),
			map->mListDoor[i]->GetBound());
		if (r1.IsCollided)
		{
			Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(mPlayer, r1);
			mPlayer->OnCollision(map->mListDoor[i]->getEntity(), r1, sidePlayer);
			map->mListDoor[i]->isOpen = true;
		}
	}
#pragma endregion

#pragma region XU LY VA CHAM CUA BOSS 1
	if (map->mBoss1) {
		Entity::CollisionReturn r2 = GameCollision::RecteAndRect(mPlayer->GetBound(),
			map->mBoss1->GetBound());
		if (r2.IsCollided)
		{
			mPlayer->OnCollisionWithEnemy();

		}
		for (size_t i = 0; i < mPlayer->mListPlayerBullet.size(); i++)
		{
			if (mPlayer->mListPlayerBullet.at(i))
			{
				Entity::CollisionReturn r = GameCollision::RecteAndRect(map->mBoss1->GetBound(),
					mPlayer->mListPlayerBullet.at(i)->GetBound());
				if (r.IsCollided)
				{
					mPlayer->mListPlayerBullet.at(i)->OnCollision();
					map->mBoss1->OnCollissionWithBullet(mPlayer->mListPlayerBullet.at(i)->damage);
				}
			}
		}
	}
#pragma endregion

#pragma region XU LY VA CHAM CUA BOSS 2
	if (map->mBoss2) {
		Entity::CollisionReturn r2 = GameCollision::RecteAndRect(mPlayer->GetBound(),
			map->mBoss2->GetBound());
		if (r2.IsCollided)
		{
			mPlayer->OnCollisionWithEnemy();

		}
		for (size_t i = 0; i < mPlayer->mListPlayerBullet.size(); i++)
		{
			if (mPlayer->mListPlayerBullet.at(i))
			{
				Entity::CollisionReturn r = GameCollision::RecteAndRect(map->mBoss2->GetBound(),
					mPlayer->mListPlayerBullet.at(i)->GetBound());
				if (r.IsCollided)
				{
					mPlayer->mListPlayerBullet.at(i)->OnCollision();
					map->mBoss2->OnCollissionWithBullet(mPlayer->mListPlayerBullet.at(i)->damage);
				}
			}
		}
	}
#pragma endregion

#pragma region XU LY VA CHAM CUA BOSS 3
	if (map->mBoss3) {
		Entity::CollisionReturn r2 = GameCollision::RecteAndRect(mPlayer->GetBound(),
			map->mBoss3->GetBound());
		if (r2.IsCollided)
		{
			mPlayer->OnCollisionWithEnemy();

		}
		for (size_t i = 0; i < mPlayer->mListPlayerBullet.size(); i++)
		{
			if (mPlayer->mListPlayerBullet.at(i))
			{
				Entity::CollisionReturn r = GameCollision::RecteAndRect(map->mBoss3->GetBound(),
					mPlayer->mListPlayerBullet.at(i)->GetBound());
				if (r.IsCollided)
				{
					mPlayer->mListPlayerBullet.at(i)->OnCollision();
					map->mBoss3->OnCollissionWithBullet(mPlayer->mListPlayerBullet.at(i)->damage);
				}
			}
		}
	}
#pragma endregion

#pragma region XU LY VA CHAM CUA BRICK
	for (size_t i = 0; i < map->mListBrick2.size(); i++)
	{

		Entity::CollisionReturn r1 = GameCollision::RecteAndRect(mPlayer->GetBound(),
			map->mListBrick2[i]->GetBound());
		if (r1.IsCollided)
		{
			Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(mPlayer, r1);
			mPlayer->OnCollision(map->mListBrick2[i]->getEntity(), r1, sidePlayer);
		}
		for (size_t j = 0; j < mPlayer->mListPlayerBullet.size(); j++)
		{
			Entity::CollisionReturn r2 = GameCollision::RecteAndRect(mPlayer->mListPlayerBullet[j]->GetBound(),
				map->mListBrick2[i]->GetBound());
			if (r2.IsCollided)
			{
				map->mListBrick2[i]->HP -= mPlayer->mListPlayerBullet[j]->damage;
				mPlayer->mListPlayerBullet[j]->OnCollision();
			}

			//BRICK
			if (map->brick)
			{
				Entity::CollisionReturn r3 = GameCollision::RecteAndRect(mPlayer->mListPlayerBullet[j]->GetBound(),
					map->brick->GetBound());
				if (r3.IsCollided)
				{
					mPlayer->mListPlayerBullet[j]->OnCollision();
					map->brick->HP -= mPlayer->mListPlayerBullet[j]->damage;
				}
			}
		}

	}
#pragma endregion



}
void DemoScene::DrawQuadtree(QuadTree *quadtree)
{
	if (quadtree->GetNodes())
	{
		for (size_t i = 0; i < 4; i++)
		{
			DrawQuadtree(quadtree->GetNodes()[i]);
		}
	}

	mDebugDraw->DrawRect(quadtree->Bound, camera);

	if (quadtree->GetNodes())
	{
		for (size_t i = 0; i < 4; i++)
		{
			mDebugDraw->DrawRect(quadtree->GetNodes()[i]->Bound, camera);
		}
	}
}
void DemoScene::DrawCollidable()
{
	for (auto child : mCollidable)
	{
		mDebugDraw->DrawRect(child->GetBound(), camera);
	}
}

void DemoScene::saveQuadTree(QuadTree *quadtree)
{
	std::vector<Entity*> mList;
	mList = quadtree->getmListEntity();
	if (quadtree-> mLevel > 10) return;
	outfile << quadtree->mId <<" "<<quadtree->mLevel<<" " <<quadtree->Bound.left << " " << quadtree->Bound.top << " " << quadtree->Bound.right << " " << quadtree->Bound.bottom<< " " ;
	outfile << mList.size() << " " ;
	for (int i = 0; i < mList.size(); i++)
	{
		outfile << mList.at(i)->id << " ";
		
	}
	/*if (quadtree->GetNodes()) outfile << "4 ";
	else outfile << "0 ";*/
	outfile << endl;
	
	if (quadtree->GetNodes())
	{
		for (size_t i = 0; i < 4; i++)
		{
			saveQuadTree(quadtree->GetNodes()[i]);
		}
	}
	if (quadtree->mId == 14444) { outfile.close();  }
	
	
}
/*void DemoScene::createQuadTree()
{
	outfile.open("quadtree.txt");
	int cId,cLevel; RECT cBound;
	while (!outfile.eof())
	{
		//GAMELOG("quad1: %d", temp); 
		outfile >> cId;
		outfile >> cLevel;
		outfile >> cBound.left;
		outfile >> cBound.top;
		outfile >> cBound.bottom;
		outfile >> cBound.right;
		

		
	}
	
}*/

