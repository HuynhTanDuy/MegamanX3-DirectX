#include "Map.h"
#include "GameComponents/GameLog.h"
Map::Map(char* filePath,Player *mPlayer)
{
	LoadMap(filePath);
	this->mPlayer = mPlayer;
}

Map::~Map()
{
	delete mMap;
	for (size_t i = 0; i < mListTileset.size(); i++)
	{
		if (mListTileset[i])
			delete mListTileset[i];
	}
	mListTileset.clear();

	delete mQuadTree;
}

void Map::LoadMap(char* filePath)
{
	mMap = new Tmx::Map();
	mMap->ParseFile(filePath);

	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = this->GetWidth();
	r.bottom = this->GetHeight();

	mQuadTree = new QuadTree(1,1, r); //khoi tao quadtree

	for (size_t i = 0; i < mMap->GetNumTilesets(); i++)
	{
		const Tmx::Tileset *tileset = mMap->GetTileset(i);

		Sprite *sprite = new Sprite(tileset->GetImage()->GetSource().c_str());

		mListTileset.insert(std::pair<int, Sprite*>(i, sprite));
	}

	//Khoi tao body cho cac doi tuong tinh
#pragma region -OBJECTGROUP, STATIC OBJECT-

	for (size_t i = 0; i < mMap->GetNumObjectGroups(); i++)
	{
		const Tmx::ObjectGroup *objectGroup = mMap->GetObjectGroup(i);
	
			for (size_t j = 0; j < objectGroup->GetNumObjects(); j++)
			{
				//lay object group chu khong phai layer
				//object group se chua nhung body
				Tmx::Object *object = objectGroup->GetObjects().at(j);

				Entity *entity = new Entity();

				entity->SetPosition(object->GetX() + object->GetWidth() / 2,
					object->GetY() + object->GetHeight() / 2);
				entity->SetWidth(object->GetWidth());
				entity->SetHeight(object->GetHeight());
				entity->Tag = Entity::EntityTypes::Static;
				entity->id = object->GetId();
				mQuadTree->insertEntity(entity);
				mListEntity[entity->id] = entity;
				mList.push_back(entity);
			}
		
	}
#pragma endregion
	for (size_t i = 0; i < mMap->GetNumObjectGroups(); i++)
	{

		const Tmx::ObjectGroup *objectGroup = mMap->GetObjectGroup(i);
		if (objectGroup->GetName() == "creep 1")
		{
			for (size_t j = 0; j < objectGroup->GetNumObjects(); j++)
			{

				Tmx::Object *object = objectGroup->GetObjects().at(j);
				D3DXVECTOR3 position(object->GetX(), object->GetY(), 0);
				//Enemy1 *temp = new Enemy1();
				Enemy1 *temp = new Enemy1();
				temp->SetPosition(position);
				mListEnemy1.push_back(temp);
				Entity *entity = new Entity();
				entity->SetPosition(position);
				entity->SetWidth(temp->GetWidth());
				entity->SetHeight(temp->GetHeight());
				mListBodyEnemy1.push_back(entity);
			}
		}
		if (objectGroup->GetName() == "creep 2")
		{
			for (size_t j = 0; j < objectGroup->GetNumObjects(); j++)
			{

				Tmx::Object *object = objectGroup->GetObjects().at(j);
				D3DXVECTOR3 position(object->GetX(), object->GetY(), 0);
				//Enemy1 *temp = new Enemy1();
				Enemy2 *temp = new Enemy2();
				temp->SetPosition(position);
				mListEnemy2.push_back(temp);
				Entity *entity = new Entity();
				entity->SetPosition(position);
				entity->SetWidth(temp->GetWidth());
				entity->SetHeight(temp->GetHeight());
				mListBodyEnemy2.push_back(entity);
			}
		}
		if (objectGroup->GetName() == "creep 3")
		{
			for (size_t j = 0; j < objectGroup->GetNumObjects(); j++)
			{

				Tmx::Object *object = objectGroup->GetObjects().at(j);
				D3DXVECTOR3 position(object->GetX(), object->GetY(), 0);
				//Enemy1 *temp = new Enemy1();
				Enemy3 *temp = new Enemy3();
				temp->SetPosition(position);
				mListEnemy3.push_back(temp);
				Entity *entity = new Entity();
				entity->SetPosition(position);
				entity->SetWidth(temp->GetWidth());
				entity->SetHeight(temp->GetHeight());
				mListBodyEnemy3.push_back(entity);
			}
		}
	}
	//GAMELOG("ene: %d", mListEnemy1);
	//createQuadTree();
}

Tmx::Map* Map::GetMap()
{
	return mMap;
}

int Map::GetWidth()
{
	return mMap->GetWidth()* mMap->GetTileWidth();
}

int Map::GetHeight()
{
	return mMap->GetHeight() * mMap->GetTileHeight();
}
int Map::GetTileWidth()
{
	return mMap->GetTileWidth();
}

int Map::GetTileHeight()
{
	return mMap->GetTileHeight();
}



/*int Map::GetWidth()
{
	return mMap->GetWidth();
}

int Map::GetHeight()
{
	return mMap->GetHeight();
}
*/

void Map::Draw()
{
	D3DXVECTOR2 trans = D3DXVECTOR2(GameGlobal::GetWidth() / 2 - mCamera->GetPosition().x,
		GameGlobal::GetHeight() / 2 - mCamera->GetPosition().y);
	//background->Draw(D3DXVECTOR3(), RECT(), D3DXVECTOR2(), trans);*/
#pragma region DRAW TILESET

	for (size_t i = 0; i < mMap->GetNumTileLayers(); i++)
	{
		const Tmx::TileLayer *layer = mMap->GetTileLayer(i);

		if (!layer->IsVisible())
		{
			continue;
		}
		RECT sourceRECT;

		int tileWidth = mMap->GetTileWidth();
		int tileHeight = mMap->GetTileHeight();
		for (size_t m = 0; m < layer->GetHeight(); m++)
		{
			for (size_t n = 0; n < layer->GetWidth(); n++)
			{


				int tilesetIndex = layer->GetTileTilesetIndex(n, m);

				if (tilesetIndex != -1)
				{
					const Tmx::Tileset *tileSet = mMap->GetTileset(tilesetIndex);

					int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
					int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;

					Sprite* sprite = mListTileset[layer->GetTileTilesetIndex(n, m)];

					//tile index
					int tileID = layer->GetTileId(n, m);

					int y = tileID / tileSetWidth;
					int x = tileID - y * tileSetWidth;

					sourceRECT.top = y * tileHeight;
					sourceRECT.bottom = sourceRECT.top + tileHeight;
					sourceRECT.left = x * tileWidth;
					sourceRECT.right = sourceRECT.left + tileWidth;
					//tru tilewidth/2 va tileheight/2 vi Sprite ve o vi tri giua hinh anh cho nen doi hinh de cho
					//dung toa do (0,0) cua the gioi thuc la (0,0) neu khong thi se la (-tilewidth/2, -tileheigth/2);
					D3DXVECTOR3 position(n * tileWidth + tileWidth / 2, m * tileHeight + tileHeight / 2, 0);

					sprite->SetWidth(tileWidth);
					sprite->SetHeight(tileHeight);

					sprite->Draw(position, sourceRECT, D3DXVECTOR2(), trans);


					//Sprite* sprite = mListTileset[j];
					if (mCamera != NULL)
					{
						RECT objRECT;
						objRECT.left = position.x - tileWidth / 2;
						objRECT.top = position.y - tileHeight / 2;
						objRECT.right = objRECT.left + tileWidth;
						objRECT.bottom = objRECT.top + tileHeight;

						if (!GameCollision::RecteAndRect(mCamera->GetBound(), objRECT).IsCollided)
							continue;
					}

				}
			}

		}
	}
#pragma endregion

#pragma region DRAW ENEMIES
	for (size_t i = 0; i < mListEnemy1.size(); i++)
	{
		mListEnemy1[i]->Draw(mListEnemy1[i]->GetPosition(), RECT(), D3DXVECTOR2(), trans);
	}
	for (size_t i = 0; i < mListEnemy2.size(); i++)
	{
		mListEnemy2[i]->Draw(mListEnemy2[i]->GetPosition(), RECT(), D3DXVECTOR2(), trans);
	}
	for (size_t i = 0; i < mListEnemy3.size(); i++)
	{
		mListEnemy3[i]->Draw(mListEnemy3[i]->GetPosition(), RECT(), D3DXVECTOR2(), trans);
	}
#pragma endregion 

	for (size_t i = 0; i < mPlayer->mListPlayerBullet.size(); i++)
	{
		mPlayer->mListPlayerBullet.at(i)->Draw(mPlayer->mListPlayerBullet.at(i)->GetPosition(), RECT(), D3DXVECTOR2(), trans);
	}
}

void Map::SetCamera(Camera * camera)
{
	this->mCamera = camera;
}
RECT Map::GetWorldMapBound()
{
	RECT bound;
	bound.left = bound.top = 0;
	bound.right = mMap->GetWidth() * mMap->GetTileWidth();
	bound.bottom = mMap->GetHeight() * mMap->GetTileHeight();

	return bound;
}
std::map<int, Sprite*> Map::getListTileSet()
{
	return mListTileset;
}



QuadTree * Map::GetQuadTree()
{
	return mQuadTree;
}
bool Map::IsBoundLeft()
{
	return (mCamera->GetBound().left == 0);
}

bool Map::IsBoundRight()
{
	return (mCamera->GetBound().right == this->GetWidth());
}

bool Map::IsBoundTop()
{
	return (mCamera->GetBound().top == 0);
}

bool Map::IsBoundBottom()
{
	return (mCamera->GetBound().bottom == this->GetHeight());
}

void Map::Update(float dt)
{
	/*for (size_t i = 0; i < mListBricks.size(); i++)
	{
		mListBricks[i]->Update(dt);
	}*/
	for (size_t i = 0; i < mListEnemy1.size(); i++)
	{
		if (mPlayer->GetPosition().x > mListEnemy1[i]->GetPosition().x - 30) mListEnemy1[i]->SetReverse(true);
		else mListEnemy1[i]->SetReverse(false);
		mListEnemy1[i]->Update(dt);
	}
	for (size_t i = 0; i < mListBodyEnemy1.size(); i++)
	{
		Entity *entity = new Entity();
		entity->SetPosition(mListEnemy1[i]->GetPosition());
		entity->SetWidth(mListEnemy1[i]->GetWidth());
		entity->SetHeight(mListEnemy1[i]->GetHeight());
		mListBodyEnemy1[i] = entity;
	}
	for (size_t i = 0; i < mListEnemy2.size(); i++)
	{
		if (mPlayer->GetPosition().x > mListEnemy2[i]->GetPosition().x - 30) mListEnemy2[i]->SetReverse(true);
		else mListEnemy2[i]->SetReverse(false);
		mListEnemy2[i]->Update(dt);
	}
	for (size_t i = 0; i < mListEnemy3.size(); i++)
	{
		if (mPlayer->GetPosition().x > mListEnemy3[i]->GetPosition().x - 30) mListEnemy3[i]->SetReverse(true);
		else mListEnemy3[i]->SetReverse(false);
		mListEnemy3[i]->Update(dt);
	}
	for (size_t i = 0; i < mListBodyEnemy3.size(); i++)
	{
		Entity *entity = new Entity();
		entity->SetPosition(mListEnemy3[i]->GetPosition());
		entity->SetWidth(mListEnemy3[i]->GetWidth());
		entity->SetHeight(mListEnemy3[i]->GetHeight());
		mListBodyEnemy3[i] = entity;
	}
	//mListEnemy1[0]->Update(dt);
}
void Map::createQuadTree()
{
	this->outfile.open("quadtree.txt");
	int cId, cLevel, cNumberOfEntity, j, cNumberOfNodes,dem,temp; RECT cBound;
	while (!outfile.eof())
	{
		//GAMELOG("quad1: %d", temp); 
		outfile >> cId; temp = cId;
		outfile >> cLevel;
		outfile >> cBound.left;
		outfile >> cBound.top;
		outfile >> cBound.right;
		outfile >> cBound.bottom;
		//GAMELOG("id: %d", cId);
		mapQuadtree[cId] = new QuadTree(cId, cLevel, cBound);
		outfile >> cNumberOfEntity;
		for (int i = 0; i < cNumberOfEntity; i++)
		{
			//mapQuadtree[cId]->getmListEntity().push_back(mListEntity[outfile >> j]);
			outfile >> j;
			mapQuadtree[temp]->mListEntity.push_back(mListEntity[j]);
			
			//GAMELOG("temp: %d", temp);
			//GAMELOG("j: %d", j);
			//GAMELOG("list: %d", mapQuadtree[1]->getmListEntity().size());
		}
	}
	
	this->outfile.close();
	
		
	std::map<int, QuadTree*>::iterator it;
	for (it = mapQuadtree.begin(); it!=mapQuadtree.end(); it++)
	{
		if (it->first == 1) mQuadTree = it->second;
		else
		{
			int p = it->first / 10;
			int child = it->first % 10;
			if (mapQuadtree[p] != NULL)
			{
				QuadTree *pQuadTree = mapQuadtree[p];
				//if  (pQuadTree->Nodes==NULL) pQuadTree->Nodes= new QuadTree *[4];
				if (pQuadTree->Nodes == NULL)   pQuadTree->Nodes = new QuadTree *[4];
				switch (child)
				{
				case 1:
					pQuadTree->Nodes[0] = it->second;
					break;
				case 2:
					pQuadTree->Nodes[1] = it->second;
					break;
				case 3:
					pQuadTree->Nodes[2] = it->second;
					break;
				case 4:
					pQuadTree->Nodes[3] = it->second;
					break;
				default:
					break;
				}
			}
		}
	}
	//GAMELOG("quad %d:", mQuadTree1);
	/*this->outfile1.open("quadtree1.txt");
	
	//outfile1 << mapQuadtree[1]->mId << mapQuadtree[1]->Bound.left;
	//saveQuadTree(mQuadTree1);
	outfile1 << mapQuadtree[1]->mId << " " << mapQuadtree[1]->mLevel << " " << mapQuadtree[1]->Bound.left << " " << mapQuadtree[1]->Bound.top << " " << mapQuadtree[1]->Bound.right << " " << mapQuadtree[1]->Bound.bottom << " " <<mapQuadtree[1]->getmListEntity().size()  <<std::endl;
	outfile1 << mapQuadtree[12]->mId << " " << mapQuadtree[12]->mLevel << " " << mapQuadtree[12]->Bound.left << " " << mapQuadtree[12]->Bound.top << " " << mapQuadtree[12]->Bound.right << " " << mapQuadtree[12]->Bound.bottom << " " << mapQuadtree[12]->getmListEntity().size()<< std::endl;
	outfile1.close();
	*/
	
	
}

