#pragma once

#include "Collider.h"

struct ltex_t;

class PixelCollider : public Collider
{
public:
	PixelCollider(const Vec2& Pos, Vec2 Size, uint8_t* Pixels);
	virtual bool collides(const Vec2& circlePos, float circleRadius) const override;
	virtual bool collides(const Vec2& rectPos, const Vec2& rectSize) const override;
	virtual bool collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const override;

	inline const Vec2& GetSize() const { return m_vSize; }
	inline void SetSize(Vec2 Size) { m_vSize = Size; }
	inline const uint8_t* GetPixels() const { return m_pPixels; }
	inline void SetPixels(uint8_t* Pixels) { m_pPixels = Pixels; }

private:
	const Vec2& m_vPosition;
	Vec2 m_vSize;
	uint8_t* m_pPixels;
};

