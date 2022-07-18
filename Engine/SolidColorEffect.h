#pragma once

#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"
#include "Pipeline.h"

class SolidColorEffect
{
public:
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec4& pos)
			:
			pos(pos)
		{}
		Vertex(const Vec4& pos, const Vertex& src)
			:
			color(src.color),
			pos(pos)
		{}
		Vertex(const Vec4& pos, const Vec3& color)
			:
			color(color),
			pos(pos)
		{}
		Vertex& operator +=(const Vertex& rhs)
		{
			pos += rhs.pos;
			return *this;
		}
		Vertex operator +(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex& operator -=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			return *this;
		}
		Vertex operator -(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex& operator *=(float rhs)
		{
			pos *= rhs;
			return *this;
		}
		Vertex operator *(float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator /=(float rhs)
		{
			pos /= rhs;
			return *this;
		}
		Vertex operator /(float rhs) const
		{
			return Vertex(*this) /= rhs;
		}
	public:
		Vec4 pos;
		Color color;
	};

	typedef DefaultVertexShader<Vertex> VertexShader;

	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

	class PixelShader
	{
	public:
		template<class Input>
		Color operator()(const Input& in) const
		{
			return in.color;
		}
	};
public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};
