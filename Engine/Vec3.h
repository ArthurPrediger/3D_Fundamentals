#pragma once

#include "Vec2.h"  

template<typename T>
class Vec3_ : public Vec2_<T>
{
public:
	Vec3_() {}
	Vec3_(T x, T y, T z)
		:
		Vec2_<T>(x, y),
		z(z)
	{}
	Vec3_(const Vec3_& vect)
		:
		Vec3_(vect.x, vect.y, vect.z)
	{}
	template <typename T2>
	explicit operator Vec3_<T2>() const
	{
		return { (T2)x,(T2)y, (T2)z};
	}
	T LenSq() const
	{
		return sq(*this);
	}
	T Len() const
	{
		return sqrt(LenSq());
	}
	Vec3_& Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		z /= length;
		return *this;
	}
	Vec3_ GetNormalized() const
	{
		Vec3_ norm = *this;
		norm.Normalize();
		return norm;
	}
	Vec3_ operator-() const
	{
		return Vec3_(-x, -y, -z);
	}
	Vec3_& operator=(const Vec3_& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	Vec3_& operator+=(const Vec3_& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	Vec3_& operator-=(const Vec3_& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	// Dot product
	T operator*(const Vec3_& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	// Cross product
	Vec3_ operator%(const Vec3_& rhs) const
	{
		return Vec3_(
				y * rhs.z - z * rhs.y,
				z * rhs.x - x * rhs.z,
				x * rhs.y - y * rhs.x);
	}
	Vec3_ operator+(const Vec3_& rhs) const
	{
		return Vec3_(*this) += rhs;
	}
	Vec3_ operator-(const Vec3_& rhs) const
	{
		return Vec3_(*this) -= rhs;
	}
	Vec3_& operator*=(const T& rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	Vec3_ operator*(const T& rhs) const
	{
		return Vec3_(*this) *= rhs;
	}
	Vec3_& operator/=(const T& rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}
	Vec3_ operator/(const T& rhs) const
	{
		return Vec3_(*this) /= rhs;
	}
	bool operator==(const Vec3_& rhs) const
	{
		return x == rhs.x && y == rhs.y && z = z.rhs;
	}
	bool operator!=(const Vec3_& rhs) const
	{
		return !(*this == rhs);
	}
public:
	T z;
};

typedef Vec3_<float> Vec3;
typedef Vec3_<double> Ved3;
typedef Vec3_<int> Vei3;