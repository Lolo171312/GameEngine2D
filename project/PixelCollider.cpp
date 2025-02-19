#include "PixelCollider.h"
#include <litegfx.h>
#include <glfw3.h>

PixelCollider::PixelCollider(const Vec2& Pos, Vec2 Size, uint8_t* Pixels) : m_vPosition(Pos), m_vSize(Size), m_pPixels(Pixels)
{}

bool PixelCollider::collides(const Vec2& circlePos, float circleRadius) const
{
	return checkCirclePixels(circlePos, circleRadius, m_vPosition, m_vSize, m_pPixels);
}

bool PixelCollider::collides(const Vec2& rectPos, const Vec2& rectSize) const
{
	return checkPixelsRect(m_vPosition, m_vSize, m_pPixels, rectPos, rectSize);
}

bool PixelCollider::collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const
{
	return checkPixelsPixels(m_vPosition, m_vSize, m_pPixels, pixelsPos, pixelsSize, pixels);
}
