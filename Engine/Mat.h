#pragma once

#include "Vec4.h"

template<typename T, size_t S>
class Mat
{
public:
	Mat& operator=(const Mat& rhs)
	{
		memcpy(elements, rhs.elements, sizeof(elements));
		return *this;
	}
	Mat& operator *=(T rhs)
	{
		for (auto& row : elements)
		{
			for (T& element : row)
			{
				e *= rhs;
			}
		}
	}
	Mat operator *(T rhs)
	{
		Mat result = *this;
		return result *= rhs;
	}
	Mat& operator *= (const Mat* rhs)
	{
		return *this = *this * rhs;
	}
	Mat operator*(const Mat& rhs) const
	{
		Mat result = {};
		for (size_t row_left = 0; row_left < S; row_left++)
		{
			for (size_t col_right = 0; col_right < S; col_right++)
			{
				T sum = (T)0;
				for (int i = 0; i < S; i++)
				{
					sum += elements[row_left][i] * rhs.elements[i][col_right];
				}
				result.elements[row_left][col_right] = sum;
			}
		}
		return result;
	}
	Mat operator!() const
	{
		Mat transpose{};
		for (size_t i = 0; i < S; i++)
		{
			for (size_t j = 0; j < S; j++)
			{
				transpose.elements[i][j] = elements[j][i];
			}
		}
		return transpose;
	}
	constexpr static Mat Identity()
	{
		if constexpr (S == 3)
		{
			return {
				(T)1.0, (T)0.0, (T)0.0,
				(T)0.0, (T)1.0, (T)0.0,
				(T)0.0, (T)0.0, (T)1.0
			};
		}
		else if constexpr (S == 4)
		{
			return {
				(T)1.0, (T)0.0, (T)0.0, (T)0.0,
				(T)0.0, (T)1.0, (T)0.0, (T)0.0,
				(T)0.0, (T)0.0, (T)1.0, (T)0.0, 
				(T)0.0, (T)0.0, (T)0.0, (T)1.0
			};
		}
		else
		{
			static_assert(false, "Bad dimensionality");
		}
	}
	constexpr static Mat Scaling(T factor)
	{
		if constexpr (S == 3)
		{
			return {
				factor, (T)0.0, (T)0.0,
				(T)0.0, factor, (T)0.0,
				(T)0.0, (T)0.0, factor
			};
		}
		else if constexpr (S == 4)
		{
			return {
				factor, (T)0.0, (T)0.0, (T)0.0,
				(T)0.0, factor, (T)0.0, (T)0.0,
				(T)0.0, (T)0.0, factor, (T)0.0,
				(T)0.0, (T)0.0, (T)0.0, (T)1.0
			};
		}
		else
		{
			static_assert(false, "Bad dimensionality");
		}
	}
	constexpr static Mat RotationZ(T theta)
	{
		const T cosTheta = (T)cos(theta);
		const T sinTheta = (T)sin(theta);
		if constexpr (S == 3)
		{
			return {
				cosTheta, -sinTheta, (T)0.0,
				sinTheta,  cosTheta, (T)0.0,
				(T)0.0,    (T)0.0,   (T)1.0
			};
		}
		else if constexpr (S == 4)
		{
			return {
				cosTheta, -sinTheta, (T)0.0, (T)0.0,
				sinTheta,  cosTheta, (T)0.0, (T)0.0,
				(T)0.0,    (T)0.0,   (T)1.0, (T)0.0,
				(T)0.0,    (T)0.0,   (T)0.0, (T)1.0
			};
		}
		else
		{
			static_assert(false, "Bad dimensionality");
		}
	}
	constexpr static Mat RotationY(T theta)
	{
		const T cosTheta = (T)cos(theta);
		const T sinTheta = (T)sin(theta);
		if constexpr (S == 3)
		{
			return {
				cosTheta,  (T)0.0, sinTheta,
				(T)0.0,    (T)1.0, (T)0.0,
				-sinTheta, (T)0.0, cosTheta
			};
		}
		else if constexpr (S == 4)
		{
			return {
				cosTheta,  (T)0.0, sinTheta, (T)0.0,
				(T)0.0,    (T)1.0, (T)0.0,   (T)0.0,
				-sinTheta, (T)0.0, cosTheta, (T)0.0,
				(T)0.0,    (T)0.0, (T)0.0,   (T)1.0
			};
		}
		else
		{
			static_assert(false, "Bad dimensionality");
		}
	}
	constexpr static Mat RotationX(T theta)
	{
		const T cosTheta = (T)cos(theta);
		const T sinTheta = (T)sin(theta);
		if constexpr (S == 3)
		{
			return {
				(T)1.0, (T)0.0,   (T)0.0,
				(T)0.0, cosTheta, -sinTheta,
				(T)0.0, sinTheta, cosTheta
			};
		}
		else if constexpr (S == 4)
		{
			return {
				(T)1.0, (T)0.0,   (T)0.0,    (T)0.0,
				(T)0.0, cosTheta, -sinTheta, (T)0.0,
				(T)0.0, sinTheta, cosTheta,  (T)0.0,
				(T)0.0, (T)0.0,   (T)0.0,    (T)1.0
			};
		}
		else
		{
			static_assert(false, "Bad dimensionality");
		}
	}
	template <class V>
	constexpr static Mat Translation(const V& tl)
	{
		return Translation(tl.x, tl.y, tl.z);
	}
	constexpr static Mat Translation(T x, T y, T z)
	{
		if constexpr (S == 4)
		{
			return {
				(T)1.0, (T)0.0, (T)0.0, x,
				(T)0.0, (T)1.0, (T)0.0, y,
				(T)0.0, (T)0.0, (T)1.0, z,
				(T)0.0, (T)0.0, (T)0.0, (T)1.0
			};
		}
		else
		{
		static_assert(false, "Bad dimensionality");
		}
	}
	constexpr static Mat Projection(T w, T h, T n, T f)
	{
		if constexpr (S == 4)
		{
			return {
				(T)2 * n / w, (T)0.0,    (T)0.0,      (T)0.0,
				(T)0.0,    (T)2 * n / h, (T)0.0,      (T)0.0,
				(T)0.0,    (T)0.0,    f / (f - n), -n * f / (f - n),
				(T)0.0,    (T)0.0,    (T)1.0,      (T)0.0
			};
		}
		else
		{
			static_assert(false, "Bad dimensionality");
		}
	}
	constexpr static Mat ProjectionHFOV(T fov, T ar, T n, T f)
	{
		if constexpr (S == 4)
		{
			const auto fov_rad = fov * (T)PI / (T)180.0;
			const auto w = (T)1.0 / std::tan(fov_rad / (T)2.0);
			const auto h = w * ar;

			return {
				w,      (T)0.0, (T)0.0,      (T)0.0,
				(T)0.0, h,      (T)0.0,      (T)0.0,
				(T)0.0, (T)0.0, f / (f - n), -n * f / (f - n),
				(T)0.0, (T)0.0, (T)1.0,      (T)0.0
			};
		}
		else
		{
			static_assert(false, "Bad dimensionality");
		}
	}
public:
	// [row][col]
	T elements[S][S];
};

template <typename T>
Vec3_<T>& operator*=(const Vec3_<T>& lhs, const Mat<T, 3>& rhs)
{
	return lhs = rhs * lhs;
}

template <typename T>
Vec3_<T> operator*(const Mat<T, 3>& lhs, const Vec3_<T>& rhs)
{
	return {
		lhs.elements[0][0] * rhs.x + lhs.elements[0][1] * rhs.y + lhs.elements[0][2] * rhs.z,
		lhs.elements[1][0] * rhs.x + lhs.elements[1][1] * rhs.y + lhs.elements[1][2] * rhs.z,
		lhs.elements[2][0] * rhs.x + lhs.elements[2][1] * rhs.y + lhs.elements[2][2] * rhs.z
	};
}

template <typename T>
Vec4_<T>& operator*=(const Vec4_<T>& lhs, const Mat<T, 4>& rhs)
{
	return lhs = rhs * lhs;
}

template <typename T>
Vec4_<T> operator*(const Mat<T, 4>& lhs, const Vec4_<T>& rhs)
{
	return {
		lhs.elements[0][0] * rhs.x + lhs.elements[0][1] * rhs.y + lhs.elements[0][2] * rhs.z + lhs.elements[0][3] * rhs.w,
		lhs.elements[1][0] * rhs.x + lhs.elements[1][1] * rhs.y + lhs.elements[1][2] * rhs.z + lhs.elements[1][3] * rhs.w,
		lhs.elements[2][0] * rhs.x + lhs.elements[2][1] * rhs.y + lhs.elements[2][2] * rhs.z + lhs.elements[2][3] * rhs.w,
		lhs.elements[3][0] * rhs.x + lhs.elements[3][1] * rhs.y + lhs.elements[3][2] * rhs.z + lhs.elements[3][3] * rhs.w,
	};
}

using Mat3 = Mat<float, 3>;
using Mad3 = Mat<double, 3>;
using Mat4 = Mat<float, 4>;
using Mad4 = Mat<double, 4>;