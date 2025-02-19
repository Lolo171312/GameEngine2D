#include "Sprite.h"
#include "stb_image.h"
#include <litegfx.h>
#include <glfw3.h>
#include "Collider.h"
#include <iostream>
#include "CircleCollider.h"
#include "RectCollider.h"
#include "PixelCollider.h"

Sprite::Sprite(const char* fileName, int hFrames, int vFrames, int fps) : m_pTexture(nullptr), m_blendMode(lblend_t::BLEND_ALPHA), m_red(1), m_green(1), m_blue(1), m_alpha(1), m_position(Vec2(0, 0)),
m_angle(0), m_scale(Vec2(1, 1)), m_pivot(Vec2(0, 0)), m_hFrames(hFrames), m_vFrames(vFrames), m_fps(fps), m_currentFps(0), m_hCurrentFrame(0), m_vCurrentFrame(0), m_CollisionType(COLLISION_NONE), m_pCollider(nullptr), m_bShouldFreeTexture(true)
{
	m_pPixels = stbi_load(fileName, &m_width, &m_height, nullptr, 4);
	if (m_pPixels)
	{
		m_pTexture = ltex_alloc(m_width, m_height, 0);
		ltex_setpixels(m_pTexture, m_pPixels);
		stbi_image_free(m_pPixels);
	}
	else
	{
		printf("Error. fileName no existe");
	}
}

Sprite::Sprite(ltex_t* texture, float xPos, float yPos, float spriteWidth, float spriteHeight, float U0, float U1, float V0, float V1) 
	: m_pTexture(texture), m_position(Vec2(xPos, yPos)), m_width(spriteWidth), m_height(spriteHeight), m_U0(U0), m_U1(U1), m_V0(V0), m_V1(V1), m_scale(Vec2(1, 1)), m_pivot(Vec2::ZeroVec()), m_bShouldFreeTexture(false), m_hFrames(1), m_vFrames(1)
{}

Sprite::~Sprite()
{
	if(m_CollisionType == COLLISION_PIXELS)
	{
		stbi_image_free(m_pPixels);
	}
	
	if(m_bShouldFreeTexture)
	{
		ltex_free(m_pTexture);
	}
}

void Sprite::SetTexture(const char* fileName, int hFrames, int vFrames)
{
	m_pPixels = stbi_load(fileName, &m_width, &m_height, nullptr, 4);
	if (m_pPixels)
	{
		ltex_free(m_pTexture);
		m_pTexture = ltex_alloc(m_width, m_height, 0);
		ltex_setpixels(m_pTexture, m_pPixels);
		
		if(GetCollisionType() == COLLISION_PIXELS)
		{
			PixelCollider* pPixelColl = static_cast<PixelCollider*>(GetCollider());
			if (pPixelColl)
			{
				pPixelColl->SetPixels(m_pPixels);
			}
		}
		else
		{
			stbi_image_free(m_pPixels);
		}

		m_hFrames = hFrames;
		m_vFrames = vFrames;
		SetScale(m_scale);
	}
	else
	{
		printf("Error. fileName no existe");
	}
}

void Sprite::SetColor(float r, float g, float b, float a)
{
	m_red = r;
	m_green = g;
	m_blue = b;
	m_alpha = a;
}

void Sprite::SetPosition(const Vec2& pos)
{
	m_position = pos;
}

void Sprite::SetPosition(float x, float y)
{
	m_position._x = x;
	m_position._y = y;
}

void Sprite::SetScale(const Vec2& scale)
{
	m_scale = scale;

	if (GetCollisionType() == COLLISION_CIRCLE)
	{
		CircleCollider* pCircleColl = static_cast<CircleCollider*>(GetCollider());
		if (pCircleColl)
		{
			pCircleColl->SetRadus(std::fmax((GetSize()._x * scale._x / 2.f), (GetSize()._y * scale._y / 2.f)));
		}
	}

	if (GetCollisionType() == COLLISION_RECT)
	{
		RectCollider* pRectColl = static_cast<RectCollider*>(GetCollider());
		if (pRectColl)
		{
			pRectColl->SetRect(Vec2(GetSize()._x * scale._x, GetSize()._y * scale._y));
		}
	}

	if (GetCollisionType() == COLLISION_PIXELS)
	{
		PixelCollider* pPixelColl = static_cast<PixelCollider*>(GetCollider());
		if (pPixelColl)
		{
			pPixelColl->SetSize(Vec2(GetSize()._x * m_scale._x, GetSize()._y * m_scale._y));
		}
	}
}

const Vec2& Sprite::GetSize() const
{
	const float xSize = static_cast<float>(m_width) / m_hFrames;
	const float ySize = static_cast<float>(m_height) / m_vFrames;
	return Vec2(xSize, ySize);
}

void Sprite::Update(float deltaTime)
{
	if (m_fps > 0) 
	{
		++m_currentFps;
		if (m_currentFps > m_fps)
		{
			m_currentFps = 0;
			++m_hCurrentFrame;
			if (m_hCurrentFrame >= m_hFrames)
			{
				m_hCurrentFrame = 0;
				++m_vCurrentFrame;
				if(m_vCurrentFrame >= m_vFrames)
				{
					m_vCurrentFrame = 0;
				}
			}
		}
	}
	else
	{
		--m_currentFps;
		if (m_currentFps < m_fps)
		{
			m_currentFps = 0;
			--m_hCurrentFrame;
			if (m_hCurrentFrame < 0)
			{
				m_hCurrentFrame = m_hFrames - 1;
				--m_vCurrentFrame;
				if(m_vCurrentFrame<0)
				{
					m_vCurrentFrame = m_vFrames;
				}
			}
		}
	}

	Draw();
}

void Sprite::Draw() const
{
	const Vec2 vSize = GetSize();
	float u0 = (vSize._x / static_cast<float>(m_width)) * m_hCurrentFrame;
	float v0 = (vSize._y / static_cast<float>(m_height)) * m_vCurrentFrame;
	float u1 = (vSize._x / static_cast<float>(m_width)) * (m_hCurrentFrame + 1);
	float v1 = (vSize._y / static_cast<float>(m_height)) * (m_vCurrentFrame + 1);

	lgfx_setblend(m_blendMode);
	lgfx_setcolor(m_red, m_green, m_blue, m_alpha);
	ltex_drawrotsized(m_pTexture, m_position._x, m_position._y, m_angle, m_pivot._x, m_pivot._y, vSize._x * m_scale._x, vSize._y * m_scale._y, u0, v0, u1, v1);
}

void Sprite::SimpleDraw() const
{
	ltex_drawrotsized(m_pTexture, m_position._x, m_position._y, m_angle, m_pivot._x, m_pivot._y, m_width, m_height, m_U0, m_V0, m_U1, m_V1);
}

void Sprite::SetCollisionType(CollisionType type)
{
	if(m_pCollider != nullptr)
	{
		delete m_pCollider;
	}

	if(m_CollisionType == COLLISION_PIXELS)
	{
		stbi_image_free(m_pPixels);
	}

	m_CollisionType = type;
	switch (m_CollisionType)
	{
	case COLLISION_NONE:
		//Quitar collision
		break;
	case COLLISION_CIRCLE:
		//Poner collision de Circulo
		m_pCollider = new CircleCollider(m_position, std::fmax((GetSize()._x * m_scale._x / 2.f), (GetSize()._y * m_scale._y / 2.f)));
		break;
	case COLLISION_RECT:
		//Poner collision de Rectangulo
		m_pCollider = new RectCollider(m_position, Vec2(GetSize()._x * m_scale._x, GetSize()._y * m_scale._y));
		break;
	case COLLISION_PIXELS:
		//Poner collision de Pixeles
		m_pPixels = new unsigned char[m_width * m_height * 4];
		ltex_getpixels(m_pTexture, m_pPixels);
		Vec2 Size = Vec2(GetSize()._x * m_scale._x, GetSize()._y * m_scale._y);
		m_pCollider = new PixelCollider(m_position, Size, m_pPixels);
		break;
	}
}

bool Sprite::Collides(const Sprite& other) const
{
	if (other.GetCollider() == nullptr || other.GetCollisionType() == COLLISION_NONE) return false;

	if(other.GetCollisionType() == COLLISION_CIRCLE)
	{
		CircleCollider* pCircleColl = static_cast<CircleCollider*>(other.GetCollider());
		if(pCircleColl)
		{
			return m_pCollider->collides(other.GetPosition(), pCircleColl->GetRadius());
		}

		return false;
	}

	if (other.GetCollisionType() == COLLISION_RECT)
	{
		RectCollider* pRectColl = static_cast<RectCollider*>(other.GetCollider());
		if (pRectColl)
		{
			return m_pCollider->collides(other.GetPosition(), pRectColl->GetRect());
		}

		return false;
	}

	if (other.GetCollisionType() == COLLISION_PIXELS)
	{
		PixelCollider* pPixelColl = static_cast<PixelCollider*>(other.GetCollider());
		if (pPixelColl)
		{
			return m_pCollider->collides(other.GetPosition(), pPixelColl->GetSize(), pPixelColl->GetPixels());
		}

		return false;
	}
	
	return false;
}

bool Sprite::operator==(const Sprite& other)
{
	return (this->m_pTexture == other.m_pTexture) && (this->m_CollisionType == other.m_CollisionType) && (this->m_position == other.m_position);
}
