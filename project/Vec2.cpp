#include "Vec2.h"
#include <cmath>

Vec2::Vec2(double x, double y):
	_x(x), _y(y)
{}

Vec2::Vec2(Vec2 const& other):
	_x(other._x), _y(other._y)
{}

Vec2 Vec2::operator+(Vec2 const& other)
{
	double x = this->_x + other._x;
	double y = this->_y + other._y;

	return Vec2(x, y);
}

Vec2& Vec2::operator+=(Vec2 const& other)
{
	this->_x += other._x;
	this->_y += other._y;

	return *this;
}

Vec2 Vec2::operator-(Vec2 const& other)
{
	double x = this->_x - other._x;
	double y = this->_y - other._y;

	return Vec2(x, y);
}

Vec2 Vec2::operator-(const Vec2& other) const
{
	return Vec2(this->_x - other._x, this->_y - other._y);
}

Vec2& Vec2::operator-=(Vec2 const& other)
{
	this->_x -= other._x;
	this->_y -= other._y;

	return *this;
}

Vec2 Vec2::operator*(float value)
{
	double x = this->_x * value;
	double y = this->_y * value;

	return Vec2(x, y);
}

Vec2 Vec2::operator*(double value)
{
	double x = this->_x * value;
	double y = this->_y * value;

	return Vec2(x, y);
}

Vec2& Vec2::operator*=(float value)
{
	this->_x *= value;
	this->_y *= value;

	return *this;
}

Vec2 Vec2::operator/(float value)
{
	double x = this->_x / value;
	double y = this->_y / value;

	return Vec2(x, y);
}

Vec2& Vec2::operator/=(float value)
{
	this->_x /= value;
	this->_y /= value;

	return *this;
}

Vec2& Vec2::operator=(const Vec2& other)
{
	this->_x = other._x;
	this->_y = other._y;

	return *this;
}

bool Vec2::operator==(const Vec2& other)
{
	return (this->_x == other._x) && (this->_y == other._y);
}

Vec2& Vec2::Abs()
{
	if (this->_x < 0)
	{
		this->_x *= -1;
	}

	if (this->_y < 0)
	{
		this->_y *= -1;
	}

	return *this;
}

Vec2 Vec2::SafeAbs() const
{
	float x = this->_x;
	float y = this->_y;
	if(x < 0)
	{
		x *= -1;
	}

	if(y < 0)
	{
		y *= -1;
	}

	return Vec2(x, y);
}

float Vec2::Size() const
{
	return sqrt((this->_x * this->_x) + (this->_y * this->_y));
}

float Vec2::Dot(Vec2 const& other) const
{
	return (this->_x * other._x) + (this->_y * other._y);
}

Vec2& Vec2::Normalize()
{
	float magnitude = this->Size();
	this->_x /= magnitude;
	this->_y /= magnitude;

	return *this;
}

Vec2 Vec2::SafeNormalize() const
{
	float magnitude = this->Size();
	double x = this->_x / magnitude;
	double y = this->_y / magnitude;

	return Vec2(x, y);
}

float Vec2::Angle(Vec2 const& other) const
{
	//Using DOT to calculate Angle
	//cos-1 * |Vec1| * |Vec2] = Dot(Vec1, Vec2)
	float acosVal = this->Dot(other) / (this->Size() * other.Size());

	//acos returns the angle in radians. To turn it into degrees -> rad * (180 / PI)
	float angle = acos(acosVal) * (180.f / PI);

	return angle;
}

float Vec2::Distance(Vec2 const& other) const
{
	float xDiff = this->_x - other._x;
	float yDiff = this->_y - other._y;
	
	return sqrt((xDiff * xDiff) + (yDiff * yDiff));
}

float Vec2::DOT(Vec2 const& thisVector, Vec2 const& other)
{
	return (thisVector._x * other._x) + (thisVector._y * other._y);
}

float Vec2::ANGLE(Vec2 const& thisVector, Vec2 const& other)
{
	float acosVal = thisVector.Dot(other) / (thisVector.Size() * other.Size());
	float angle = acos(acosVal) * (180.f / 3.14159265f);

	return angle;
}

float Vec2::DIST(Vec2 const& thisVector, Vec2 const& other)
{
	float xDiff = thisVector._x - other._x;
	float yDiff = thisVector._y - other._y;

	return sqrt((xDiff * xDiff) + (yDiff * yDiff));
}