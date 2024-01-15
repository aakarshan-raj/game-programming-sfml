#pragma once

class Vec2
{
public:
	float x = 0;
	float y = 0;

	Vec2();
	Vec2(float xin, float yin);

	bool operator==(Vec2 &lhs) const;
	bool operator!=(Vec2 &lhs) const;

	Vec2 operator+(Vec2 &lhs) const;
	Vec2 operator-(Vec2 &lhs) const;
	Vec2 operator*(Vec2 &lhs) const;
	Vec2 operator/(Vec2 &lhs) const;

	void operator+=(const Vec2 &lhs);
	void operator-=(const Vec2 &lhs);
	void operator*=(const float lhs);
	void operator/=(const float lhs);

	void normalize();
	float dist(const Vec2 &rhs) const;

	float length() const;
};
