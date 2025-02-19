#include "RectCollider.h"

RectCollider::RectCollider(const Vec2& Pos, const Vec2& Rec) : m_vPosition(Pos), m_vRect(Rec)
{}

bool RectCollider::collides(const Vec2& circlePos, float circleRadius) const
{
	return checkCircleRect(circlePos, circleRadius, m_vPosition, m_vRect);
}

bool RectCollider::collides(const Vec2& rectPos, const Vec2& rectSize) const
{
	return checkRectRect(m_vPosition, m_vRect, rectPos, rectSize);
}

bool RectCollider::collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const
{
	return checkPixelsRect(pixelsPos, pixelsSize, pixels, m_vPosition, m_vRect);
}