#pragma once

class Vec2
{
public:
	float x = 0;
	float y = 0;

	Vec2();
	Vec2(float xin, float yin);

	bool operator==(Vec2 &rhs) const;
	bool operator!=(Vec2 &rhs) const;

	Vec2 operator+(Vec2 &rhs) const;
	Vec2 operator-(Vec2 &rhs) const;
	Vec2 operator*(Vec2 &rhs) const;
	Vec2 operator/(Vec2 &rhs) const;

	void operator+=(const Vec2 &rhs);
	void operator-=(const Vec2 &rhs);
	void operator*=(const float rhs);
	void operator/=(const float rhs);

	void normalize();
	float dist(const Vec2 &rhs) const;

	float length() const;
};
