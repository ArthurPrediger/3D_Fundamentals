#pragma once

#include "ChiliWin.h"
#include "Graphics.h"
#include "Triangle.h"
#include "IndexedTriangleList.h"
#include "PC3ScreenTransformer.h"
#include "Mat3.h"
#include <algorithm>

class Pipeline
{
public:
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec3& pos)
			:
			pos(pos)
		{}
		Vertex(const Vec3& pos, const Vertex& src)
			:
			t(src.t),
			pos(pos)
		{}
		Vertex(const Vec3& pos, const Vec2& t)
			:
			t(t),
			pos(pos)
		{}
		Vertex& operator +=(const Vertex& rhs)
		{
			pos += rhs.pos;
			t += rhs.t;
			return *this;
		}
		Vertex operator +(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex& operator -=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			t -= rhs.t;
			return *this;
		}
		Vertex operator -(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex& operator *=(float rhs)
		{
			pos *= rhs;
			t *= rhs;
			return *this;
		}
		Vertex operator *(float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator /=(float rhs)
		{
			pos /= rhs;
			t /= rhs;
			return *this;
		}
		Vertex operator /(float rhs) const
		{
			return Vertex(*this) /= rhs;
		}
	public:
		Vec3 pos;
		Vec2 t;
	};
public:
	Pipeline(Graphics& gfx)
		:
		gfx(gfx)
	{}
	void Draw(IndexedTriangleList<Vertex>& triList)
	{
		ProcessVertices(triList.vertices, triList.indices);
	}
	void BindRotation(const Mat3& rotation_in)
	{
		rotation = rotation_in;
	}
	void BindTranslation(const Vec3& translation_in)
	{
		translation = translation_in;
	}
	void BindTexture(const std::wstring& filename)
	{
		pTex = std::make_unique<Surface>(Surface::FromFile(filename));
	}
private:
	void ProcessVertices(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		std::vector<Vertex> verticesOut;

		for (const auto& v : vertices)
		{
			verticesOut.emplace_back((rotation * v.pos) + translation, v.t);
		}

		AssembleTriangles(verticesOut, indices);
	}
	void AssembleTriangles(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		for (size_t i = 0, end = indices.size() / 3; i < end; i++)
		{
			const auto& v0 = vertices[indices[i * 3]];
			const auto& v1 = vertices[indices[i * 3 + 1]];
			const auto& v2 = vertices[indices[i * 3 + 2]];

			//cull backfacion triangles with cross product (%) shenanigans
			if ((v1.pos - v0.pos) % (v2.pos - v0.pos) * v0.pos <= 0.0f)
			{
				ProcessTriangle(v0, v1, v2);
			}
		}
	}
	void ProcessTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		PostProcessTrianglesVertices(Triangle<Vertex>{ v0, v1, v2 });
	}
	void PostProcessTrianglesVertices(Triangle<Vertex>& triangle)
	{
		pst.Transform(triangle.v0.pos);
		pst.Transform(triangle.v1.pos);
		pst.Transform(triangle.v2.pos);

		DrawTriangle(triangle);
	}
	void DrawTriangle(const Triangle<Vertex>& triangle)
	{
		const Vertex* pv0 = &triangle.v0;
		const Vertex* pv1 = &triangle.v1;
		const Vertex* pv2 = &triangle.v2;

		if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);
		if (pv2->pos.y < pv1->pos.y) std::swap(pv1, pv2);
		if (pv1->pos.y < pv0->pos.y) std::swap(pv1, pv0);

		if (pv0->pos.y == pv1->pos.y)
		{
			if (pv1->pos.x < pv0->pos.x) std::swap(pv0, pv1);
			DrawFlatTopTriangle(*pv0, *pv1, *pv2);
		}
		else if (pv1->pos.y == pv2->pos.y)
		{
			if (pv2->pos.x < pv1->pos.x) std::swap(pv1, pv2);
			DrawFlatBottomTriangle(*pv0, *pv1, *pv2);
		}
		else
		{
			const float alphaSplit = (pv1->pos.y - pv0->pos.y) / (pv2->pos.y - pv0->pos.y);

			const Vertex vi = *pv0 + (*pv2 - *pv0) * alphaSplit;

			if (pv1->pos.x < vi.pos.x)
			{
				DrawFlatBottomTriangle(*pv0, *pv1, vi);
				DrawFlatTopTriangle(*pv1, vi, *pv2);
			}
			else
			{
				DrawFlatBottomTriangle(*pv0, vi, *pv1);
				DrawFlatTopTriangle(vi, *pv1, *pv2);
			}
		}
	}
	void DrawFlatTopTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2)
	{
		const float delta_y = it2.pos.y - it0.pos.y;
		const auto dit0 = (it2 - it0) / delta_y;
		const auto dit1 = (it2 - it1) / delta_y;

		Vertex itEdge1 = it1;

		DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
	}
	void DrawFlatBottomTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2)
	{
		const float delta_y = it2.pos.y - it0.pos.y;
		const auto dit0 = (it1 - it0) / delta_y;
		const auto dit1 = (it2 - it0) / delta_y;

		Vertex itEdge1 = it0;

		DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
	}
	void DrawFlatTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2,
		const Vertex& dit0, const Vertex& dit1, Vertex itEdge1)
	{
		Vertex itEdge0 = it0;

		const int yStart = (int)ceil(it0.pos.y - 0.5f);
		const int yEnd = (int)ceil(it2.pos.y - 0.5f);

		itEdge0 += dit0 * (float(yStart) + 0.5f - it0.pos.y);
		itEdge1 += dit1 * (float(yStart) + 0.5f - it0.pos.y);

		const float tex_width = float(pTex->GetWidth());
		const float tex_height = float(pTex->GetHeight());
		const float tex_clamp_x = tex_width - 1.0f;
		const float tex_clamp_y = tex_height - 1.0f;

		for (int y = yStart; y < yEnd; y++,
			itEdge0 += dit0, itEdge1 += dit1)
		{
			const int xStart = (int)ceil(itEdge0.pos.x - 0.5f);
			const int xEnd = (int)ceil(itEdge1.pos.x - 0.5f);

			Vertex iLine = itEdge0;

			const float dx = itEdge1.pos.x - itEdge0.pos.x;
			const auto diLine = (itEdge1 - iLine) / dx;

			iLine += diLine * (float(xStart) + 0.5f - itEdge0.pos.x);

			for (int x = xStart; x < xEnd; x++, iLine += diLine)
			{
				gfx.PutPixel(x, y, pTex->GetPixel(
					(unsigned int)std::min(iLine.t.x * tex_width, tex_clamp_x),
					(unsigned int)std::min(iLine.t.y * tex_height, tex_clamp_y)));
			}
		}
	}
private:
	Graphics& gfx;
	PC3ScreenTransformer pst;
	Mat3 rotation;
	Vec3 translation;
	std::unique_ptr<Surface> pTex;
};