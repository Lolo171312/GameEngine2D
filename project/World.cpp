#include "World.h"
#include <litegfx.h>
#include <glfw3.h>
#include <stb_image.h>
#include "Sprite.h"
#include "Background.h"
#include <iostream>
#include "Utils.h"
#include "Collider.h"

World::World(float screenWidth, float screenHeight, BackgroundData back1, BackgroundData back2, BackgroundData back3, BackgroundData back4, float clearRed, float clearGreen, float clearBlue) : 
	m_clearRed(clearRed), m_clearGreen(clearGreen), m_clearBlue(clearBlue)
{
	m_backgroundDatas[0] = back1;
	m_backgroundDatas[1] = back2;
	m_backgroundDatas[2] = back3;
	m_backgroundDatas[3] = back4;

	m_screenSize = Vec2(screenWidth, screenHeight);

	CreateBackground(back1, m_backgrounds1);
	CreateBackground(back2, m_backgrounds2);
	CreateBackground(back3, m_backgrounds3);
	CreateBackground(back4, m_backgrounds4);
}

World::~World()
{
	for (size_t i = 0; i < 4; ++i)
	{
		ltex_free(m_backgroundDatas[i].GetBackgroundTexture());
	}

	if (m_mapTexture) 
	{
		ltex_free(m_mapTexture);
	}

	for (size_t i = 0; i < m_mapSprites.size(); ++i)
	{
		delete m_mapSprites[i];
		m_mapSprites[i] = nullptr;
	}
}

bool World::LoadMap(const char* filename, uint16_t firstColId)
{
	pugi::xml_parse_result result = m_mapDoc.load_file(filename);
	if (result)
	{
		//Obtener mapNode
		m_mapNode = m_mapDoc.child("map");
		//Obtener direccion del png de tiles
		std::string extractedPath = Utils::extractPath(filename);
		extractedPath = extractedPath.append(m_mapNode.child("tileset").child("image").attribute("source").as_string());
		//Cargar la textura del mapa
		int tilesWidht;
		int tilesHeight;
		uint8_t* Pixels = stbi_load(extractedPath.c_str(), &tilesWidht, &tilesHeight, nullptr, 4);
		if (Pixels)
		{
			m_mapTexture = ltex_alloc(tilesWidht, tilesHeight, 0);
			ltex_setpixels(m_mapTexture, Pixels);
			stbi_image_free(Pixels);
			m_mapSize = Vec2(m_mapNode.attribute("width").as_int() * m_mapNode.attribute("tilewidth").as_int(), m_mapNode.attribute("height").as_int() * m_mapNode.attribute("tileheight").as_int());
			GetTileSprites();
		}
		return true;
	}
	else
	{
		std::cout << result.description() << std::endl;
		return false;
	}
}

void World::GetTileSprites()
{
	if (m_mapTexture)
	{
		//Obtener numero total de tiles
		int TotalWidthTiles = m_mapNode.attribute("tilewidth").as_int();
		int TotalHeightTiles = m_mapNode.attribute("tileheight").as_int();

		float TileWidth = m_mapNode.attribute("width").as_int();
		float TileHeight = m_mapNode.attribute("height").as_int();

		//Obtener primer tile a dibujar
		pugi::xml_node tileNode = m_mapNode.child("layer").child("data").child("tile");

		for (size_t HeightTiles = 0; HeightTiles < TotalHeightTiles; ++HeightTiles)
		{
			for (size_t WidthTiles = 0; WidthTiles < TotalWidthTiles; ++WidthTiles)
			{
				//Comprobar que el tile sea distinto a 0
				int TileInt = tileNode.attribute("gid").as_int();
				if (TileInt != 0)
				{
					float TileU0 = 0.f;
					float TileU1 = 0.f;
					float TileV0 = 0.f;
					float TileV1 = 0.f;
					GetTileUVs(m_mapTexture, TileInt, TileWidth, TileHeight, TileU0, TileV0, TileU1, TileV1);
					//Obtener el tile actual
					Sprite* TileSprite = new Sprite(m_mapTexture, WidthTiles * TileWidth, HeightTiles * TileHeight, TileWidth, TileHeight, TileU0, TileU1, TileV0, TileV1);
					m_mapSprites.push_back(TileSprite);
					TileSprite->SetCollisionType(COLLISION_RECT);
				}

				//Pasar al tile vecino
				tileNode = tileNode.next_sibling("tile");
			}
			//Salto de línea
		}
	}
}

ECollisionFlag World::MoveSprite(Sprite& MovingSprite, const Vec2& Amount)
{
	MovingSprite.AddPosition(Amount);

	for (size_t i = 0; i < m_mapSprites.size(); ++i)
	{
		if(m_mapSprites[i]->Collides(MovingSprite))
		{
			float deltaX = (MovingSprite.GetPosition()._x + MovingSprite.GetSize()._x / 2) - (m_mapSprites[i]->GetPosition()._x + m_mapSprites[i]->GetSize()._x / 2);
			float deltaY = (MovingSprite.GetPosition()._y + MovingSprite.GetSize()._y / 2) - (m_mapSprites[i]->GetPosition()._y + m_mapSprites[i]->GetSize()._y / 2);

			float intersectX = abs(deltaX) - (MovingSprite.GetSize()._x / 2 + m_mapSprites[i]->GetSize()._x / 2);
			float intersectY = abs(deltaY) - (MovingSprite.GetSize()._y / 2 + m_mapSprites[i]->GetSize()._y / 2);

			if (intersectX > intersectY) 
			{
				if (deltaX > 0)
				{
					// Colisión por la derecha
					MovingSprite.SetPosition(m_mapSprites[i]->GetPosition()._x + m_mapSprites[i]->GetSize()._x + 3.f, MovingSprite.GetPosition()._y);
					return ECollisionFlag::Left;
				}
				else
				{
					// Colisión por la izquierda
					MovingSprite.SetPosition(m_mapSprites[i]->GetPosition()._x - m_mapSprites[i]->GetSize()._x - 3.f, MovingSprite.GetPosition()._y);
					return ECollisionFlag::Right;
				}
			}
			else 
			{
				//Deshacer movimiento vertical
				if (deltaY > 0)
				{
					// Colisión por abajo
					MovingSprite.SetPosition(MovingSprite.GetPosition()._x, m_mapSprites[i]->GetPosition()._y + m_mapSprites[i]->GetSize()._y + 32.f);
					return ECollisionFlag::Up;
				}
				else
				{
					// Colisión por arriba
					MovingSprite.SetPosition(MovingSprite.GetPosition()._x, m_mapSprites[i]->GetPosition()._y - m_mapSprites[i]->GetSize()._y - 32.f);
					return ECollisionFlag::Down;
				}
			}

			break;
		}
	}

	return ECollisionFlag::None;
}

const ltex_t* World::GetBackground(size_t layer) const
{
	if(layer > -1 && layer < 4)
	{
		return m_backgroundDatas[layer].GetBackgroundTexture();
	}

	return nullptr;
}

float World::GetScrollRatio(size_t layer) const
{
	if (layer > -1 && layer < 4)
	{
		return m_backgroundScrollRatios[layer];
	}

	return 0.0f;
}

void World::SetScrollRatio(size_t layer, float ratio)
{
	if (layer > -1 && layer < 4)
	{
		m_backgroundScrollRatios[layer] = ratio;
	}
}

const Vec2& World::GetScrollSpeed(size_t layer) const
{
	if (layer > -1 && layer < 4)
	{
		return m_backgroundScrollSpeeds[layer];
	}

	return Vec2(0, 0);
}

void World::SetScrollSpeed(size_t layer, const Vec2& speed)
{
	if (layer > -1 && layer < 4)
	{
		m_backgroundScrollSpeeds[layer];
	}
}

void World::SetCameraPosition(const Vec2& pos)
{
	if (pos._x > 0 && pos._x < GetMapSize()._x - m_screenSize._x)
	{
		m_cameraPosition._x = pos._x;
	}
	else
	{
		if(pos._x < 0)
		{
			m_cameraPosition._x = 0.f;
		}
		else 
		{
			m_cameraPosition._x = GetMapSize()._x - m_screenSize._x;
		}
	}

	if (pos._y > 0 && pos._y < GetMapSize()._y - m_screenSize._y)
	{
		m_cameraPosition._y = pos._y;
	}
	else
	{
		if (pos._y < 0)
		{
			m_cameraPosition._y = 0.f;
		}
		else
		{
			m_cameraPosition._y = GetMapSize()._y - m_screenSize._y;
		}
	}
}

void World::AddSprite(Sprite* sprite)
{
	if (sprite == nullptr) return;

	std::vector<Sprite*>::iterator Itr = std::find(m_sprites.begin(), m_sprites.end(), sprite);
	if (Itr == m_sprites.end()) 
	{
		m_sprites.push_back(sprite);
	}
}

void World::RemoveSprite(Sprite* sprite)
{
	if (sprite == nullptr) return;

	std::vector<Sprite*>::iterator Itr = std::find(m_sprites.begin(), m_sprites.end(), sprite);
	if (Itr != m_sprites.end())
	{
		m_sprites.erase(Itr);
	}
}

void World::Update(float deltaTime)
{
	lgfx_setorigin(m_cameraPosition._x, m_cameraPosition._y);

	//Clear Scene and Draw Backgrounds
	Draw(m_screenSize);

	//Draw sprites
	for (size_t i = 0; i < m_sprites.size(); ++i)
	{
		if (m_sprites[i])
		{
			m_sprites[i]->Update(deltaTime);
		}
	}

	for (size_t i = 0; i < m_backgrounds4.size(); ++i)
	{
		m_backgrounds4[i]->Update(deltaTime, m_cameraPosition, m_screenSize);
	}
	
	for (size_t i = 0; i < m_backgrounds3.size(); ++i)
	{
		m_backgrounds3[i]->Update(deltaTime, m_cameraPosition, m_screenSize);
	}
	
	for (size_t i = 0; i < m_backgrounds2.size(); ++i)
	{
		m_backgrounds2[i]->Update(deltaTime, m_cameraPosition, m_screenSize);
	}
	
	for (size_t i = 0; i < m_backgrounds1.size(); ++i)
	{
		m_backgrounds1[i]->Update(deltaTime, m_cameraPosition, m_screenSize);
	}
}

void World::Draw(const Vec2& screenSize)
{
	//Clear color Buffer
	lgfx_clearcolorbuffer(m_clearRed, m_clearGreen, m_clearBlue);

	//Draw background
	for (size_t i = 0; i < m_backgrounds4.size(); ++i)
	{
		m_backgrounds4[i]->Draw();
	}
	
	for (size_t i = 0; i < m_backgrounds3.size(); ++i)
	{
		m_backgrounds3[i]->Draw();
	}
	
	for (size_t i = 0; i < m_backgrounds2.size(); ++i)
	{
		m_backgrounds2[i]->Draw();
	}
	
	for (size_t i = 0; i < m_backgrounds1.size(); ++i)
	{
		m_backgrounds1[i]->Draw();
	}

	DrawMap();
}

void World::DrawMap()
{
	//Ajustar colores antes de dibujar el mapa
	lgfx_setblend(BLEND_ALPHA);
	lgfx_setcolor(1.f, 1.f, 1.f, 1.f);

	for (size_t i = 0; i < m_mapSprites.size(); ++i)
	{
		m_mapSprites[i]->SimpleDraw();
	}
}

void World::CreateBackground(const BackgroundData& BackgroundStruct, std::vector<Background*>& BackgroundVector)
{
	if (BackgroundStruct.GetBackgroundTexture() == nullptr) return;

	unsigned int HorizontalBackgrounds = 0;
	unsigned int VerticalBackgrounds = 0;

	for (float backgroundHeight = -BackgroundStruct.GetBackgroundTexture()->height; backgroundHeight < m_screenSize._y + BackgroundStruct.GetBackgroundTexture()->height; )
	{
		HorizontalBackgrounds = 0;
		for (float backgroundWidth = -BackgroundStruct.GetBackgroundTexture()->width; backgroundWidth < m_screenSize._x + BackgroundStruct.GetBackgroundTexture()->width; )
		{
			BackgroundVector.push_back(new Background(BackgroundStruct.GetBackgroundTexture(), BackgroundStruct.GetScroll(), BackgroundStruct.GetAutomaticSpeed(), Vec2(backgroundWidth, backgroundHeight)));
			backgroundWidth += BackgroundStruct.GetBackgroundTexture()->width;
			++HorizontalBackgrounds;
		}

		backgroundHeight += BackgroundStruct.GetBackgroundTexture()->height;
		++VerticalBackgrounds;
	}

	for (size_t i = 0; i < BackgroundVector.size(); ++i)
	{
		BackgroundVector[i]->SetNumberOfHorizontalNeighbours(HorizontalBackgrounds);
		BackgroundVector[i]->SetNumberOfVerticalNeighbours(VerticalBackgrounds);
	}
}

void World::GetTileUVs(ltex_t* Texture, int tilePosition, float tWidth, float tHeight, float& U0, float& V0, float& U1, float& V1)
{
	int Tile = tilePosition - 1;
	float HorizontalVal = std::fmod(Tile * tWidth, Texture->width);
	U0 = HorizontalVal / Texture->width;
	U1 = (HorizontalVal + tWidth) / Texture->width;

	int TilesInRow = Texture->width / tWidth;
	float VerticalVal = std::floor(Tile / TilesInRow) * tHeight;
	V0 = VerticalVal / Texture->height;
	V1 = (VerticalVal + tHeight) / Texture->height;
}