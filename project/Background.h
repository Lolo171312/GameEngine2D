#pragma once

#include "Vec2.h"

struct ltex_t;
class Sprite;

class Background
{
public:
	Background(ltex_t* _Texture, float _Scroll = 0.0f, Vec2 _AutoSpeed = Vec2::ZeroVec(), Vec2 _InitialPosition = Vec2::ZeroVec());

	void Update(float Delta, const Vec2& CameraPosition, const Vec2& ScreenSize);
	void Draw();

	void SetNumberOfHorizontalNeighbours(unsigned int NewNumOfNeighbours) { NumberOfHorizontalNeighbours = NewNumOfNeighbours; }
	void SetNumberOfVerticalNeighbours(unsigned int NewNumOfNeighbours) { NumberOfVerticalNeighbours = NewNumOfNeighbours; }

private:
	ltex_t* Texture;
	Vec2 AutomaticSpeed;
	float Scroll;

	Vec2 Position;
	Vec2 InitialPosition;

	unsigned int NumberOfHorizontalNeighbours;
	unsigned int NumberOfVerticalNeighbours;
};

