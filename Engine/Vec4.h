#pragma once

#include "Vec3.h"  

template<typename T>
class Vec4_ : public Vec3_<T>
{
public:
	Vec4_() = default;
	Vec4_(T x, T y, T z, T w)
		:
		Vec3_(x, y, z),
		w(w)
	{}
	Vec4_(const Vec3_& v3, T w = (T)1.0)
		:
		Vec3_(v3),
		w(w)
	{}
	template <typename T2>
	explicit operator Vec4_<T2>() const
	{
		return { (T2)x,(T2)y, (T2)z, (T2)w };
	}
	Vec4_ operator-() const
	{
		return Vec4_(-x, -y, -z, -w);
	}
	Vec4_& operator=(const Vec4_& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;

		return *this;
	}
	Vec4_& operator+=(const Vec4_& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;

		return *this;
	}
	Vec4_& operator-=(const Vec4_& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;

		return *this;
	}
	Vec4_ operator+(const Vec4_& rhs) const
	{
		return Vec4_(*this) += rhs;
	}
	Vec4_ operator-(const Vec4_& rhs) const
	{
		return Vec4_(*this) -= rhs;
	}
	Vec4_& operator*=(const T& rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;

		return *this;
	}
	Vec4_ operator*(const T& rhs) const
	{
		return Vec4_(*this) *= rhs;
	}
	Vec4_& operator/=(const T& rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;

		return *this;
	}
	Vec4_ operator/(const T& rhs) const
	{
		return Vec4_(*this) /= rhs;
	}
	bool operator==(const Vec4_& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == z.rhs && w == rhs.w;
	}
	bool operator!=(const Vec4_& rhs) const
	{
		return !(*this == rhs);
	}
	Vec4_& Saturate()
	{
		x = std::min(1.0f, std::max(0.0f, x));
		y = std::min(1.0f, std::max(0.0f, y));
		z = std::min(1.0f, std::max(0.0f, z));
		w = std::min(1.0f, std::max(0.0f, w));

		return *this;
	}
	Vec4_ GetSaturated() const
	{
		Vec4_ temp(*this);
		return temp.Saturate();
	}
	Vec4_& Hadamard(const Vec4_& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;

		return *this;
	}
	Vec4_ GetHadamard(const Vec4_& rhs) const
	{
		Vec4_ temp(*this);
		return temp.Hadamard(rhs);
	}
public:
	T w;
};

typedef Vec4_<float> Vec4;
typedef Vec4_<double> Ved4;
typedef Vec4_<int> Vei4;