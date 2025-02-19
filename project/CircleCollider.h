#pragma once

#include "Collider.h"

class CircleCollider : public Collider
{
public:
	CircleCollider(const Vec2& Pos, float Radius);
	virtual bool collides(const Vec2& circlePos, float circleRadius) const override;
	virtual bool collides(const Vec2& rectPos, const Vec2& rectSize) const override;
	virtual bool collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const override;

	inline float GetRadius() const { return m_fRadius; }
	inline void SetRadus(float radius) { m_fRadius = radius; }

private:
	float m_fRadius;
	const Vec2& m_vPosition;
};

