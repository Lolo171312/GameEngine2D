#pragma once

#include "Vec2.h"

struct Color
{
public:
	Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a)
	{}

	float r;
	float g;
	float b;
	float a;
};

typedef unsigned char uint8_t;
class Sprite;

class Collider
{
public:
	virtual bool collides(const Vec2& circlePos, float circleRadius) const = 0;
	virtual bool collides(const Vec2& rectPos, const Vec2& rectSize) const = 0;
	virtual bool collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const = 0;

	//ShowDebug
	static void DrawDebugCollider(const Sprite& _Sprite, const Color& _Color);

protected:
	bool checkCircleCircle(const Vec2& pos1, float radius1, const Vec2& pos2, float radius2) const;
	bool checkCircleRect(const Vec2& circlePos, float circleRadius, const Vec2& rectPos, const Vec2& rectSize) const;
	bool checkRectRect(const Vec2& rectPos1, const Vec2& rectSize1, const Vec2& rectPos2, const Vec2& rectSize2) const;
	bool checkCirclePixels(const Vec2& circlePos, float circleRadius, const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const;
	bool checkPixelsPixels(const Vec2& pixelsPos1, const Vec2& pixelsSize1, const uint8_t* pixels1, const Vec2& pixelsPos2, const Vec2& pixelsSize2, const uint8_t* pixels2) const;
	bool checkPixelsRect(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels, const Vec2& rectPos, const Vec2& rectSize) const;
};

