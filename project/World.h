#pragma once

#include "Vec2.h"
#include "vector"
#include "pugixml/pugixml.hpp"

struct ltex_t;
class Sprite;
class Background;

enum class ECollisionFlag
{
	None, Up, Down, Right, Left
};

struct BackgroundData
{
public:
	BackgroundData() : BackgroundTexture(nullptr), Scroll(0.f), AutomaticSpeed(Vec2::ZeroVec())
	{}

	BackgroundData(ltex_t* _BackgroundTexture, float _Scroll, Vec2 _AutoSpeed = Vec2::ZeroVec()) : BackgroundTexture(_BackgroundTexture), Scroll(_Scroll), AutomaticSpeed(_AutoSpeed)
	{}

	inline ltex_t* GetBackgroundTexture() const { return BackgroundTexture; }
	inline float GetScroll() const { return Scroll; }
	inline const Vec2& GetAutomaticSpeed() const { return AutomaticSpeed; }

	inline void SetBackgroundTexture(ltex_t* _BackgroundTexture) { BackgroundTexture = _BackgroundTexture; }

private:
	ltex_t* BackgroundTexture = nullptr;
	float Scroll = 0.f;
	Vec2 AutomaticSpeed;
};

class World
{
public:
	World(float screenWidth, float screenHeight, BackgroundData back1, BackgroundData back2, BackgroundData back3, BackgroundData back4, float clearRed = 0.15f, float clearGreen = 0.15f, float clearBlue = 0.15f);
	~World();

	inline float GetClearRed() const { return m_clearRed; }
	inline float GetClearGreen() const { return m_clearGreen; }
	inline float GetClearBlue() const { return m_clearBlue; }

	const ltex_t* GetBackground(size_t layer) const;
	inline const Vec2& GetMapSize() const { return m_mapSize; }

	float GetScrollRatio(size_t layer) const;
	void SetScrollRatio(size_t layer, float ratio);

	const Vec2& GetScrollSpeed(size_t layer) const;
	void SetScrollSpeed(size_t layer, const Vec2& speed);

	inline const Vec2& GetCameraPosition() const { return m_cameraPosition; }
	void SetCameraPosition(const Vec2& pos);

	void AddSprite(Sprite* sprite);
	void RemoveSprite(Sprite* sprite);

	void Update(float deltaTime);
	void Draw(const Vec2& screenSize);
	void DrawMap();

	bool LoadMap(const char* filename, uint16_t firstColId);
	void GetTileSprites();
	ECollisionFlag MoveSprite(Sprite& MovingSprite, const Vec2& Amount);

private:
	void CreateBackground(const BackgroundData& BackgroundStruct, std::vector<Background*>& BackgroundVector);
	void GetTileUVs(ltex_t* Texture, int tilePosition, float tWidth, float tHeight, float& U0, float& V0, float& U1, float& V1);

	float m_clearRed;
	float m_clearGreen;
	float m_clearBlue;

	BackgroundData m_backgroundDatas[4];
	float m_backgroundScrollRatios[4];
	Vec2 m_backgroundScrollSpeeds[4];

	Vec2 m_cameraPosition;

	std::vector<Background*> m_backgrounds1;
	std::vector<Background*> m_backgrounds2;
	std::vector<Background*> m_backgrounds3;
	std::vector<Background*> m_backgrounds4;
	std::vector<Sprite*> m_sprites;
	std::vector<Sprite*> m_mapSprites;

	Vec2 m_screenSize;
	Vec2 m_mapSize;

	pugi::xml_document m_mapDoc;
	pugi::xml_node m_mapNode;
	ltex_t* m_mapTexture;
};

