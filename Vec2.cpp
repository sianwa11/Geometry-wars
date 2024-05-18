#include "Vec2.h";
#include <math.h>
#include<cmath>

Vec2::Vec2()
{

}

Vec2::Vec2(float xin, float yin): x(xin), y(yin)
{
}

bool Vec2::operator==(const Vec2& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator!=(const Vec2& rhs) const 
{
	return (x != rhs.x || y != rhs.y);
}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator/(float val) const
{
	return Vec2(x / val, y / val);
}

Vec2 Vec2::operator*(float val) const
{
	return Vec2(x * val,y * val);
}

void Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator/=(float val)
{
	x /= val;
	y /= val;
}

void Vec2::operator*=(float val)
{
	x *= val;
	y *= val;

}

float Vec2::dist(const Vec2& rhs) const
{
	float dx = rhs.x - x;
	float dy = rhs.y - y;

	return std::sqrt(dx * dx + dy * dy);
}

void Vec2::normalize()
{
	double L = std::sqrt(x * x + y * y);
	x = x / L;
	y = y / L;
}

Vec2 Vec2::normalize(Vec2 vec)
{
	double L = std::sqrt(vec.x * vec.x + vec.y * vec.y);
	return Vec2(vec.x / L, vec.y / L);

}


