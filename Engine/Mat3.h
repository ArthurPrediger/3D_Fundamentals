#pragma once

#include "Vec2.h"
#include "Vec3.h"

template<typename T>
class Mat3_
{
public:
	Vec2_<T> operator*(const Vec2_<T>& v) const
	{
		return Vec2_<T>(*this * Vec3_(v));
	}
	Vec3_<T> operator*(const Vec3_<T>& v) const
	{
		Vec3_<T> vout;
		vout.x = cells[0][0] * v.x + cells[0][1] * v.y + cells[0][2] * v.z;
		vout.y = cells[1][0] * v.x + cells[1][1] * v.y + cells[1][2] * v.z;
		vout.z = cells[2][0] * v.x + cells[2][1] * v.y + cells[2][2] * v.z;
		return vout;
	}
	Mat3_ operator*(const Mat3_& rhs) const
	{
		Mat3_ mout = {};
		for (int row_left = 0; row_left < 3; row_left++)
		{
			for (int col_right = 0; col_right < 3; col_right++)
			{
				mout.cells[row_left][col_right] = (T)0;
				for (int i = 0; i < 3; i++)
				{
					mout.cells[row_left][col_right] += cells[row_left][i] * rhs.cells[i][col_right];
				}
			}
		}
		return mout;
	}
	static Mat3_ Scale(T factor)
	{
		return { factor, (T)0,   (T)0,
			     (T)0,   factor, (T)0,
			     (T)0,   (T)0,   (T)1, };
	}	
	static Mat3_ ScaleIndependent(T x, T y)
	{
		return {  x,     (T)0,  (T)0,
			     (T)0,    y,    (T)0,
			     (T)0,   (T)0,  (T)1, };
	}
	static Mat3_ Identity()
	{
		return Scale((T)1);
	}
	static Mat3_ FlipY(T factor)
	{
		return { (T)1, (T)0,  (T)0,
				 (T)0, (T)-1, (T)0, 
				 (T)0, (T)0,  (T)1, };
	}
	static Mat3_ RotationZ(T theta)
	{
		const T cosTheta = (T)cos(theta);
		const T sinTheta = (T)sin(theta);
		return { cosTheta, -sinTheta, (T)0,
				 sinTheta, cosTheta,  (T)0,
				 (T)0,     (T)0,      (T)1 };
	}
	static Mat3_ RotationY(T theta)
	{
		const T cosTheta = (T)cos(theta);
		const T sinTheta = (T)sin(theta);
		return { cosTheta,  (T)0, sinTheta,
			     (T)0,      (T)1, (T)0,
				 -sinTheta, (T)0, cosTheta };
	}
	static Mat3_ RotationX(T theta)
	{
		const T cosTheta = (T)cos(theta);
		const T sinTheta = (T)sin(theta);
		return { (T)1, (T)0,      (T)0,
				 (T)0, cosTheta, -sinTheta,
				 (T)0, sinTheta, cosTheta };
	}
	static Mat3_ Translation(T x, T y)
	{
		return { (T)1, (T)0, (T)x,
				 (T)0, (T)1, (T)y,
				 (T)0, (T)0, (T)1, };
	}
public:
	// [row][col]
	T cells[3][3];
};

using Mat3 = Mat3_<float>;
using Mad3 = Mat3_<double>;
