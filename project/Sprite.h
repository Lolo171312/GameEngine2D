#pragma once

#include "Vec2.h"

struct ltex_t;
enum lblend_t;
class Collider;
typedef unsigned char uint8_t;

enum CollisionType {
	COLLISION_NONE,
	COLLISION_CIRCLE,
	COLLISION_RECT,
	COLLISION_PIXELS
};

class Sprite
{
public:
	Sprite(const char* fileName, int hFrames, int vFrames, int fps);
	Sprite(ltex_t* texture, float xPos, float yPos, float spriteWidth, float spriteHeight, float U0, float U1, float V0, float V1);
	~Sprite();

	inline const ltex_t* GetTexture() const { return m_pTexture; }
	void SetTexture(const char* fileName, int hFrames = 1, int vFrames = 1);

	inline lblend_t GetBlend() const { return m_blendMode; }
	inline void SetBlend(lblend_t mode) { m_blendMode = mode; }

	inline float GetRed() const { return m_red; }
	inline float GetGreen() const { return m_green; }
	inline float GetBlue() const { return m_blue; }
	inline float GetAlpha() const { return m_alpha; }
	void SetColor(float r, float g, float b, float a);

	inline const Vec2& GetPosition() const { return m_position; }
	void SetPosition(const Vec2& pos);
	void SetPosition(float x, float y);
	inline void AddPosition(Vec2 addVector) { m_position += addVector; }

	inline float GetAngle() const { return m_angle; }
	inline void SetAngle(float angle) { m_angle = angle; }

	inline const Vec2& GetScale() const { return m_scale; }
	void SetScale(const Vec2& scale);

	const Vec2& GetSize() const;

	inline const Vec2& GetPivot() const { return m_pivot; }
	inline void SetPivot(const Vec2& pivot) { m_pivot = pivot; }

	inline int GetHFrames() const { return m_hFrames; }
	inline int GetVFrames() const { return m_vFrames; }

	inline int GetFps() const { return m_fps; }
	inline void SetFps(int fps) { m_fps = fps; }

	inline float GetCurrentHFrame() const { return m_hCurrentFrame; }
	inline void SetCurrentHFrame(int hFrame) { m_hCurrentFrame = hFrame; }
	inline float GetCurrentVFrame() const { return m_vCurrentFrame; }
	inline void SetCurrentVFrame(int vFrame) { m_vCurrentFrame = vFrame; }

	void Update(float deltaTime);
	void Draw() const;
	void SimpleDraw() const;

	void SetCollisionType(CollisionType type);
	inline CollisionType GetCollisionType() const { return m_CollisionType; }
	inline Collider* GetCollider() const { return m_pCollider; }
	bool Collides(const Sprite& other) const;

	bool operator==(const Sprite& other);

private:
	ltex_t* m_pTexture;
	int m_width;
	int m_height;

	float m_U0;
	float m_U1;
	float m_V0;
	float m_V1;

	lblend_t m_blendMode;

	float m_red;
	float m_green;
	float m_blue;
	float m_alpha;

	Vec2 m_position;
	float m_angle;
	Vec2 m_scale;

	Vec2 m_pivot;

	int m_hFrames;
	int m_vFrames;

	int m_fps;
	int m_currentFps;

	int m_hCurrentFrame;
	int m_vCurrentFrame;

	CollisionType m_CollisionType;
	Collider* m_pCollider;

	uint8_t* m_pPixels;

	bool m_bShouldFreeTexture = false;
};