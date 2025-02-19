#include "CircleCollider.h"
#include "Vec2.h"

CircleCollider::CircleCollider(const Vec2& Pos, float Radius) : m_vPosition(Pos), m_fRadius(Radius)
{}

bool CircleCollider::collides(const Vec2& circlePos, float circleRadius) const
{
    return checkCircleCircle(m_vPosition, m_fRadius, circlePos, circleRadius);
}

bool CircleCollider::collides(const Vec2& rectPos, const Vec2& rectSize) const
{
    return checkCircleRect(m_vPosition, m_fRadius, rectPos, rectSize);
}

bool CircleCollider::collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const
{
    return checkCirclePixels(m_vPosition, m_fRadius, pixelsPos, pixelsSize, pixels);
}
