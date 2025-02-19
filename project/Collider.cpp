#include "Collider.h"
#include <litegfx.h>
#include <glfw3.h>
#include <iostream>
#include "Sprite.h"
#include "CircleCollider.h"
#include "RectCollider.h"
#include "stb_image.h"

void Collider::DrawDebugCollider(const Sprite& _Sprite, const Color& _Color)
{
    Collider* SpriteColl = _Sprite.GetCollider();
    CollisionType SpriteCollType = _Sprite.GetCollisionType();
    if (SpriteCollType == COLLISION_CIRCLE)
    {
        CircleCollider* CircleColl = static_cast<CircleCollider*>(SpriteColl);
        if (CircleColl)
        {
            lgfx_setcolor(_Color.r, _Color.g, _Color.b, _Color.a);
            lgfx_drawoval(_Sprite.GetPosition()._x, _Sprite.GetPosition()._y, CircleColl->GetRadius() * 2, CircleColl->GetRadius() * 2);
        }
    }
    else if(SpriteCollType == COLLISION_RECT)
    {
        RectCollider* RectColl = static_cast<RectCollider*>(SpriteColl);
        if(RectColl)
        {
            lgfx_setcolor(_Color.r, _Color.g, _Color.b, _Color.a);
            lgfx_drawrect(_Sprite.GetPosition()._x, _Sprite.GetPosition()._y, RectColl->GetRect()._x, RectColl->GetRect()._y);
        }
    }
}

bool Collider::checkCircleCircle(const Vec2& pos1, float radius1, const Vec2& pos2, float radius2) const
{
    Vec2 Circle1Point = Vec2(radius1, radius1) + pos1;
    Vec2 Circle2Point = Vec2(radius2, radius2) + pos2;
    float Distance = Vec2::DIST(Circle1Point, Circle2Point);
    return Distance < (radius1 + radius2);
}

bool Collider::checkCircleRect(const Vec2& circlePos, float circleRadius, const Vec2& rectPos, const Vec2& rectSize) const
{
    double rectLeft = rectPos._x;
    double rectRight = rectPos._x + rectSize._x;
    double rectTop = rectPos._y;
    double rectBottom = rectPos._y + rectSize._y;
    
    float closestX = std::max(rectLeft, std::min(circlePos._x + circleRadius, rectRight));
    float closestY = std::max(rectTop, std::min(circlePos._y + circleRadius, rectBottom));

    float distanceX = circlePos._x + circleRadius - closestX;
    float distanceY = circlePos._y + circleRadius - closestY;
    float distanceSquared = distanceX * distanceX + distanceY * distanceY;

    return distanceSquared < (circleRadius * circleRadius);
}

bool Collider::checkRectRect(const Vec2& rectPos1, const Vec2& rectSize1, const Vec2& rectPos2, const Vec2& rectSize2) const
{
    float rect1Left = rectPos1._x;
    float rect1Right = rectPos1._x + rectSize1._x;
    float rect1Top = rectPos1._y;
    float rect1Bottom = rectPos1._y + rectSize1._y;

    float rect2Left = rectPos2._x;
    float rect2Right = rectPos2._x + rectSize2._x;
    float rect2Top = rectPos2._y;
    float rect2Bottom = rectPos2._y + rectSize2._y;

    bool overlapX = rect1Left < rect2Right && rect1Right > rect2Left;

    bool overlapY = rect1Top < rect2Bottom && rect1Bottom > rect2Top;

    return overlapX && overlapY;
}

bool Collider::checkCirclePixels(const Vec2& circlePos, float circleRadius, const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const
{

    // Calcular las esquinas de los rectángulos de colisión
    float rect1Left = pixelsPos._x;
    float rect1Right = pixelsPos._x + pixelsSize._x;
    float rect1Top = pixelsPos._y;
    float rect1Bottom = pixelsPos._y + pixelsSize._y;

    float rect2Left = circlePos._x;
    float rect2Right = circlePos._x + circleRadius * 2;
    float rect2Top = circlePos._y;
    float rect2Bottom = circlePos._y + circleRadius * 2;

    // Calcular el área de superposición
    float overlapLeft = std::max(rect1Left, rect2Left);
    float overlapRight = std::min(rect1Right, rect2Right);
    float overlapTop = std::max(rect1Top, rect2Top);
    float overlapBottom = std::min(rect1Bottom, rect2Bottom);

    if (overlapLeft >= overlapRight || overlapTop >= overlapBottom) {
        return false; // No hay superposición
    }

    // Verificar la colisión de píxeles en la región de superposición
    for (int y = overlapTop; y < overlapBottom; y++)
    {
        for (int x = overlapLeft; x < overlapRight; x++)
        {
            float Dist = Vec2::DIST((Vec2(circleRadius, circleRadius) + circlePos), Vec2(x, y));

            //Check if pìxel is inside circle´s radius
            if(Dist > circleRadius)
            {
                continue;
            }

            // Coordenadas locales en la textura 1
            int localX1 = x - rect1Left;
            int localY1 = y - rect1Top;
            // Obtener píxeles (RGBA)
            const uint8_t* pixel1 = &pixels[(localY1 * static_cast<int>(pixelsSize._x) + localX1) * 4];

            // Comprobar si ambos píxeles no son transparentes
            if (pixel1[3] > 0)
            {
                return true; // Hay colisión de píxeles
            }
        }
    }

    return false; // No hay colisión

	return false;
}

bool Collider::checkPixelsPixels(const Vec2& pixelsPos1, const Vec2& pixelsSize1, const uint8_t* pixels1, const Vec2& pixelsPos2, const Vec2& pixelsSize2, const uint8_t* pixels2) const
{
    // Calcular las esquinas de los rectángulos de colisión
    float rect1Left = pixelsPos1._x;
    float rect1Right = pixelsPos1._x + pixelsSize1._x;
    float rect1Top = pixelsPos1._y;
    float rect1Bottom = pixelsPos1._y + pixelsSize1._y;

    float rect2Left = pixelsPos2._x;
    float rect2Right = pixelsPos2._x + pixelsSize2._x;
    float rect2Top = pixelsPos2._y;
    float rect2Bottom = pixelsPos2._y + pixelsSize2._y;

    // Calcular el área de superposición
    float overlapLeft = std::max(rect1Left, rect2Left);
    float overlapRight = std::min(rect1Right, rect2Right);
    float overlapTop = std::max(rect1Top, rect2Top);
    float overlapBottom = std::min(rect1Bottom, rect2Bottom);

    if (overlapLeft >= overlapRight || overlapTop >= overlapBottom) {
        return false; // No hay superposición
    }

    // Verificar la colisión de píxeles en la región de superposición
    for (int y = overlapTop; y < overlapBottom; y++) 
    {
        for (int x = overlapLeft; x < overlapRight; x++)
        {
            // Coordenadas locales en la textura 1
            int localX1 = x - rect1Left;
            int localY1 = y - rect1Top;

            // Coordenadas locales en la textura 2
            int localX2 = x - rect2Left;
            int localY2 = y - rect2Top;

            // Obtener píxeles (RGBA)
       
            const uint8_t* pixel1 = &pixels1[(localY1 * static_cast<int>(pixelsSize1._x) + localX1) * 4];
            const uint8_t* pixel2 = &pixels2[(localY2 * static_cast<int>(pixelsSize2._x) + localX2) * 4];

            // Comprobar si ambos píxeles no son transparentes
            if (pixel1[3] > 0 && pixel2[3] > 0)
            {
                return true; // Hay colisión de píxeles
            }
        }
    }

    return false; // No hay colisión de píxeles
}

bool Collider::checkPixelsRect(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels, const Vec2& rectPos, const Vec2& rectSize) const
{
    // Calcular las esquinas de los rectángulos de colisión
    float rect1Left = pixelsPos._x;
    float rect1Right = pixelsPos._x + pixelsSize._x;
    float rect1Top = pixelsPos._y;
    float rect1Bottom = pixelsPos._y + pixelsSize._y;

    float rect2Left = rectPos._x;
    float rect2Right = rectPos._x + rectSize._x;
    float rect2Top = rectPos._y;
    float rect2Bottom = rectPos._y + rectSize._y;

    // Calcular el área de superposición
    float overlapLeft = std::max(rect1Left, rect2Left);
    float overlapRight = std::min(rect1Right, rect2Right);
    float overlapTop = std::max(rect1Top, rect2Top);
    float overlapBottom = std::min(rect1Bottom, rect2Bottom);

    if (overlapLeft >= overlapRight || overlapTop >= overlapBottom) {
        return false; // No hay superposición
    }

    // Verificar la colisión de píxeles en la región de superposición
    for (int y = overlapTop; y < overlapBottom; y++)
    {
        for (int x = overlapLeft; x < overlapRight; x++)
        {
            // Coordenadas locales en la textura 1
            int localX1 = x - rect1Left;
            int localY1 = y - rect1Top;
            // Obtener píxeles (RGBA)
            const uint8_t* pixel1 = &pixels[(localY1 * static_cast<int>(pixelsSize._x) + localX1) * 4];

            // Comprobar si ambos píxeles no son transparentes
            if (pixel1[3] > 0)
            {
                return true; // Hay colisión de píxeles
            }
        }
    }

    return false; // No hay colisión
}
