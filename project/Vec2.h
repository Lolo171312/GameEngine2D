#pragma once
class Vec2
{
public:
	//Constructors
	Vec2(double x = 0, double y = 0);
	Vec2(Vec2 const& other);

	//Operator Overloads
	Vec2 operator+(Vec2 const& other);
	Vec2& operator+=(Vec2 const& other);
	Vec2 operator-(Vec2 const& other);
	Vec2 operator-(const Vec2& other) const;
	Vec2& operator-=(Vec2 const& other);
	Vec2 operator*(float value);
	Vec2 operator*(double value);
	Vec2& operator*=(float value);
	Vec2 operator/(float value);
	Vec2& operator/=(float value);
	Vec2& operator=(const Vec2& other);
	bool operator==(const Vec2& other);

	//Methods
	Vec2& Abs();
	Vec2 SafeAbs() const; //Same as Abs but does NOT modify the original Vec2
	float Size() const;
	float Dot(Vec2 const& other) const;
	Vec2& Normalize();
	Vec2 SafeNormalize() const; //Same as Normalize but does NOT modify the original Vec2

	float Angle(Vec2 const& other) const;
	float Distance(Vec2 const& other) const;

	//Static Methods
	static float DOT(Vec2 const& thisVector, Vec2 const& other);
	static float ANGLE(Vec2 const& thisVector, Vec2 const& other);
	static float DIST(Vec2 const& thisVector, Vec2 const& other);
	inline static Vec2 ZeroVec() { return Vec2(0, 0); }

	double _x;
	double _y;

private:
	const float PI = 3.14159265f;
};