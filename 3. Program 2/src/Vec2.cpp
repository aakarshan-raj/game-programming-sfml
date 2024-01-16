#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
{
}

Vec2::Vec2(float xin, float yin)
	: x(xin),
	  y(yin)
{
}

bool Vec2::operator==(Vec2 &rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator!=(Vec2 &rhs) const
{
	return (x != rhs.x || y != rhs.y);
}

Vec2 Vec2::operator+(Vec2 &rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(Vec2 &rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(Vec2 &rhs) const
{
	return Vec2(x * rhs.x, y * rhs.y);
}

Vec2 Vec2::operator/(Vec2 &rhs) const
{
	return Vec2(x / rhs.x, y / rhs.y);
}

void Vec2::operator+=(const Vec2 &rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator-=(const Vec2 &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator*=(const float rhs)
{
	x *= rhs;
	y *= rhs;
}

void Vec2::operator/=(const float rhs)
{
	x /= rhs;
	y /= rhs;
}

void Vec2::normalize()
{
	float deno = sqrt(x * x + y * y);
	x = x / deno;
	y = y / deno;
}

float Vec2::length() const
{
	return sqrt(x * x + y * y);
}

float Vec2::dist(const Vec2 &rhs) const
{
	return sqrt(((x - rhs.x) * (x - rhs.x)) + ((y - rhs.y) * (y - rhs.y)));
}