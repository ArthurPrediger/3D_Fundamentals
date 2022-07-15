#pragma once

#include "ChiliMath.h"

template <typename T>
class Vec2_
{
public:
	Vec2_() = default;
	Vec2_( T x,T y )
		:
	x( x ),
	y( y )
	{}
	template <typename T2>
	explicit operator Vec2_<T2>() const
	{
		return { (T2)x,(T2)y };
	}
	T		LenSq() const
	{
		return sq( *this );
	}
	T		Len() const
	{
		return sqrt( LenSq() );
	}
	Vec2_&	Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		return *this;
	}
	Vec2_	GetNormalized() const
	{
		Vec2_ norm = *this;
		norm.Normalize();
		return norm;
	}
	Vec2_	operator-() const
	{
		return Vec2_( -x,-y );
	}
	Vec2_&	operator=( const Vec2_ &rhs )
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	Vec2_&	operator+=( const Vec2_ &rhs )
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vec2_&	operator-=( const Vec2_ &rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	T		operator*( const Vec2_ &rhs ) const
	{
		return x * rhs.x + y * rhs.y;
	}	
	Vec2_	operator+( const Vec2_ &rhs ) const
	{
		return Vec2_( *this ) += rhs;
	}
	Vec2_	operator-( const Vec2_ &rhs ) const
	{
		return Vec2_( *this ) -= rhs;
	}
	Vec2_&	operator*=( const T &rhs )
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	Vec2_	operator*( const T &rhs ) const
	{
		return Vec2_( *this ) *= rhs;
	}
	Vec2_&	operator/=( const T &rhs )
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	Vec2_	operator/( const T &rhs ) const
	{
		return Vec2_( *this ) /= rhs;
	}
	bool	operator==( const Vec2_ &rhs ) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool	operator!=(const Vec2_ &rhs) const
	{
		return !(*this == rhs);
	}
public:
	T x;
	T y;
};

typedef Vec2_<float> Vec2;
typedef Vec2_<double> Ved2;
typedef Vec2_<int> Vei2;