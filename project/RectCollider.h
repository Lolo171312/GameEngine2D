#pragma once

#include "Collider.h"

class RectCollider : public Collider
{
public:
	RectCollider(const Vec2& Pos, const Vec2& Rec);
	virtual bool collides(const Vec2& circlePos, float circleRadius) const override;
	virtual bool collides(const Vec2& rectPos, const Vec2& rectSize) const override;
	virtual bool collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const override;

	inline Vec2 GetRect() const { return m_vRect; }
	inline void SetRect(Vec2 rect) { m_vRect = rect; }

private:
	Vec2 m_vRect;
	const Vec2& m_vPosition;
};

