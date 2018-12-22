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

	map = new Map("Resources/map.tmx",mPlayer);
	mPlayer->SetPosition(GameGlobal::GetWidth() / 2, GameGlobal::GetHeight()+500);
	camera = new Camera(GameGlobal::GetWidth(), GameGlobal::GetHeight());
	camera->SetPosition(GameGlobal::GetWidth()/2, GameGlobal::GetHeight()/2);

	map->SetCamera(camera);

	//mEnemy1 = new Enemy1();
	//mEnemy1->SetPosition(1167.33 ,1569.33);
	
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
}

void DemoScene::Draw()
{
	
	map->Draw();
    mPlayer->Draw();
	
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
	//Neu megaman dung ngoai mep thi luc nay cho mario rot xuong duoi dat    
	if (widthBottom < Define::PLAYER_BOTTOM_RANGE_FALLING && widthBottom != 0)
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
	vector<Entity*> listEnemy;
	listEnemy = map->mListBodyEnemy1;
	for (size_t i = 0; i < listEnemy.size(); i++)
	{
		Entity::CollisionReturn r = GameCollision::RecteAndRect(mPlayer->GetBound(),
			listEnemy.at(i)->GetBound()); //xac dinh Rect va cham

		if (r.IsCollided)
		{
			//lay phia va cham cua Entity so voi Player
			Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(mPlayer, r);

			//lay phia va cham cua Player so voi Entity
			//Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollision.at(i), r);

			//goi den ham xu ly collision cua Player va Entity
			mPlayer->OnCollisionWithEnemy(listEnemy.at(i), r, sidePlayer);
			//listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);

			
		}
	}

	//XU LY VA CHAM CUA ENEMY VS MAP
	for (size_t j = 0; j < listEnemy.size(); j++)
	{
		vector<Entity*> listCollisionEnemyvsMap;

		map->GetQuadTree()->getEntitiesCollideAble(listCollisionEnemyvsMap, listEnemy[j]);

		

		for (size_t i = 0;i <listCollisionEnemyvsMap.size(); i++)
		{
			Entity::CollisionReturn r = GameCollision::RecteAndRect(listEnemy[j]->GetBound(),
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
		//XU LY VA CHAM CUA ENEMY VS BULLET
		for (size_t i = 0; i < mPlayer->mListPlayerBullet.size(); i++)
		{
			if (mPlayer->mListPlayerBullet.at(i))
			{
				Entity::CollisionReturn r = GameCollision::RecteAndRect(listEnemy[j]->GetBound(),
					mPlayer->mListPlayerBullet.at(i)->GetBound());
				if (r.IsCollided)
				{
					mPlayer->mListPlayerBullet.at(i)->OnCollision();
					
				}
			}
		}
	}

	
#pragma endregion

#pragma region XU LY VA CHAM ENEMY2
	vector<Entity*> listEnemy2;
	listEnemy2 = map->mListBodyEnemy2;
	for (size_t i = 0; i < listEnemy2.size(); i++)
	{
		Entity::CollisionReturn r = GameCollision::RecteAndRect(mPlayer->GetBound(),
			listEnemy2.at(i)->GetBound()); //xac dinh Rect va cham

		if (r.IsCollided)
		{
			//lay phia va cham cua Entity so voi Player
			Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(mPlayer, r);

			//lay phia va cham cua Player so voi Entity
			//Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollision.at(i), r);

			//goi den ham xu ly collision cua Player va Entity
			mPlayer->OnCollisionWithEnemy(listEnemy2.at(i), r, sidePlayer);
			//listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);


		}
	}

	//XU LY VA CHAM CUA ENEMY VS MAP
	for (size_t j = 0; j < listEnemy2.size() - 1; j++)
	{
		vector<Entity*> listCollisionEnemyvsMap;

		map->GetQuadTree()->getEntitiesCollideAble(listCollisionEnemyvsMap, listEnemy2[j]);



		for (size_t i = 0; i < listCollisionEnemyvsMap.size(); i++)
		{
			Entity::CollisionReturn r = GameCollision::RecteAndRect(listEnemy2[j]->GetBound(),
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
	}
#pragma endregion

#pragma region XU LY VA CHAM CUA ENEMY3
	//xu ly va cham voi enemy, duyet listenemy
	vector<Entity*> listEnemy3;
	listEnemy3 = map->mListBodyEnemy3;
	for (size_t i = 0; i < listEnemy3.size(); i++)
	{
		Entity::CollisionReturn r = GameCollision::RecteAndRect(mPlayer->GetBound(),
			listEnemy3.at(i)->GetBound()); //xac dinh Rect va cham

		if (r.IsCollided)
		{
			//lay phia va cham cua Entity so voi Player
			Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(mPlayer, r);

			//lay phia va cham cua Player so voi Entity
			//Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollision.at(i), r);

			//goi den ham xu ly collision cua Player va Entity
			mPlayer->OnCollisionWithEnemy(listEnemy3.at(i), r, sidePlayer);
			//listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);


		}
	}

	//XU LY VA CHAM CUA ENEMY VS MAP
	for (size_t j = 0; j < listEnemy3.size(); j++)
	{
		vector<Entity*> listCollisionEnemyvsMap;

		map->GetQuadTree()->getEntitiesCollideAble(listCollisionEnemyvsMap, listEnemy3[j]);



		for (size_t i = 0; i < listCollisionEnemyvsMap.size(); i++)
		{
			Entity::CollisionReturn r = GameCollision::RecteAndRect(listEnemy3[j]->GetBound(),
				listCollisionEnemyvsMap.at(i)->GetBound()); //xac dinh Rect va cham

			if (r.IsCollided)
			{
				//lay phia va cham cua Entity so voi Player
				Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(listEnemy3[j], r);

				//lay phia va cham cua Player so voi Entity
				//Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollision.at(i), r);

				//goi den ham xu ly collision cua Player va Entity
				map->mListEnemy3[j]->OnCollision(listCollisionEnemyvsMap.at(i), r, sidePlayer);
				//listCollision.at(i)->OnCollision(mPlayer, r, sideImpactor);

				//kiem tra neu va cham voi phia duoi cua Player 

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

