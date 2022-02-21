/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Vec2.h																				  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include "ChiliMath.h"

template <typename T>
class Vec2_
{
public:
	Vec2_() {}
	Vec2_( T x,T y )
		:
	x( x ),
	y( y )
	{}
	Vec2_( const Vec2_& vect )
		:
	Vec2_( vect.x,vect.y )
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