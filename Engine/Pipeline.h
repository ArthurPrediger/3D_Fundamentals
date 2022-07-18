#pragma once

#include "ChiliWin.h"
#include "Graphics.h"
#include "Triangle.h"
#include "IndexedTriangleList.h"
#include "NDCScreenTransformer.h"
#include "Mat.h"
#include "ZBuffer.h"
#include <algorithm>
#include <memory>

template<class Effect>
class Pipeline
{
public:
	typedef typename Effect::Vertex Vertex;
	typedef typename Effect::VertexShader::Output VSOut;
	typedef typename Effect::GeometryShader::Output GSOut;
public:
	Pipeline(Graphics& gfx)
		:
		gfx(gfx),
		pZb(std::make_shared<ZBuffer>(gfx.ScreenWidth, gfx.ScreenHeight))
	{}
	Pipeline(Graphics& gfx, std::shared_ptr<ZBuffer> pZb)
		:
		gfx(gfx),
		pZb(pZb)
	{
		assert(pZb->GetWidth() == gfx.ScreenWidth && pZb->GetHeight() == gfx.ScreenHeight);
	}
	void Draw(IndexedTriangleList<Vertex>& triList)
	{
		ProcessVertices(triList.vertices, triList.indices);
	}
	void BeginFrame()
	{
		pZb->Clear();
	}
private:
	void ProcessVertices(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		std::vector<VSOut> verticesOut(vertices.size());

		std::transform( vertices.begin(), vertices.end(), verticesOut.begin(), effect.vs );

		AssembleTriangles(verticesOut, indices);
	}
	void AssembleTriangles(const std::vector<VSOut>& vertices, const std::vector<size_t>& indices)
	{
		const auto eyePos = effect.vs.GetProj() * Vec4{ 0.0f, 0.0f, 0.0f, 1.0f };

		for (size_t i = 0, end = indices.size() / 3; i < end; i++)
		{
			const auto& v0 = vertices[indices[i * 3]];
			const auto& v1 = vertices[indices[i * 3 + 1]];
			const auto& v2 = vertices[indices[i * 3 + 2]];

			//cull backfacing triangles with cross product (%) shenanigans
			if ((v1.pos - v0.pos) % (v2.pos - v0.pos) * Vec3(v0.pos - eyePos) <= 0.0f)
			{
				ProcessTriangle(v0, v1, v2, i);
			}
		}
	}
	void ProcessTriangle(const VSOut& v0, const VSOut& v1, const VSOut& v2, size_t triangle_index)
	{
		ClipCullTriangle(effect.gs( v0, v1, v2, triangle_index ));
	}
	void ClipCullTriangle(Triangle<GSOut>& t)
	{
		if (t.v0.pos.x > t.v0.pos.w &&
			t.v1.pos.x > t.v1.pos.w &&
			t.v2.pos.x > t.v2.pos.w)
		{
			return;
		}
		if (t.v0.pos.x < -t.v0.pos.w &&
			t.v1.pos.x < -t.v1.pos.w &&
			t.v2.pos.x < -t.v2.pos.w)
		{
			return;
		}
		if (t.v0.pos.y > t.v0.pos.w &&
			t.v1.pos.y > t.v1.pos.w &&
			t.v2.pos.y > t.v2.pos.w)
		{			 
			return;	 
		}			 
		if (t.v0.pos.y < -t.v0.pos.w &&
			t.v1.pos.y < -t.v1.pos.w &&
			t.v2.pos.y < -t.v2.pos.w)
		{
			return;
		}
		if (t.v0.pos.z > t.v0.pos.w &&
			t.v1.pos.z > t.v1.pos.w &&
			t.v2.pos.z > t.v2.pos.w)
		{			 
			return;	 
		}			 
		if (t.v0.pos.z < 0.0f &&
			t.v1.pos.z < 0.0f &&
			t.v2.pos.z < 0.0f)
		{
			return;
		}

		const auto Clip1 = [this](GSOut& v0, GSOut& v1, GSOut& v2)
		{
			const float alphaA = (-v0.pos.z) / (v1.pos.z - v0.pos.z);
			const float alphaB = (-v0.pos.z) / (v2.pos.z - v0.pos.z);

			const auto v0a = interpolate(v0, v1, alphaA);
			const auto v0b = interpolate(v0, v2, alphaB);

			PostProcessTrianglesVertices(Triangle<GSOut>{ v0a, v1, v2 });
			PostProcessTrianglesVertices(Triangle<GSOut>{ v0b, v0a, v2 });
		};

		const auto Clip2 = [this](GSOut& v0, GSOut& v1, GSOut& v2)
		{
			const float alpha0 = (-v0.pos.z) / (v2.pos.z - v0.pos.z);
			const float alpha1 = (-v1.pos.z) / (v2.pos.z - v1.pos.z);

			v0 = interpolate(v0, v2, alpha0);
			v1 = interpolate(v1, v2, alpha1);

			PostProcessTrianglesVertices(Triangle<GSOut>{ v0, v1, v2 });
		};

		if (t.v0.pos.z < 0.0f)
		{
			if (t.v1.pos.z < 0.0f)
			{
				Clip2(t.v0, t.v1, t.v2);
			}
			else if (t.v2.pos.z < 0.0f)
			{
				Clip2(t.v0, t.v2, t.v1);
			}
			else
			{
				Clip1(t.v0, t.v1, t.v2);
			}
		}
		else if (t.v1.pos.z < 0.0f)
		{
			if (t.v2.pos.z < 0.0f)
			{
				Clip2(t.v1, t.v2, t.v0);
			}
			else
			{
				Clip1(t.v1, t.v0, t.v2);
			}
		}
		else if (t.v2.pos.z < 0.0f)
		{
			Clip1(t.v2, t.v0, t.v1);
		}
		else
		{
			PostProcessTrianglesVertices(t);
		}
	}
	void PostProcessTrianglesVertices(Triangle<GSOut>& triangle)
	{
		pst.Transform(triangle.v0);
		pst.Transform(triangle.v1);
		pst.Transform(triangle.v2);

		DrawTriangle(triangle);
	}
	void DrawTriangle(const Triangle<GSOut>& triangle)
	{
		const GSOut* pv0 = &triangle.v0;
		const GSOut* pv1 = &triangle.v1;
		const GSOut* pv2 = &triangle.v2;

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

			const GSOut vi = *pv0 + (*pv2 - *pv0) * alphaSplit;

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
	void DrawFlatTopTriangle(const GSOut& it0, const GSOut& it1, const GSOut& it2)
	{
		const float delta_y = it2.pos.y - it0.pos.y;
		const auto dit0 = (it2 - it0) / delta_y;
		const auto dit1 = (it2 - it1) / delta_y;

		GSOut itEdge1 = it1;

		DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
	}
	void DrawFlatBottomTriangle(const GSOut& it0, const GSOut& it1, const GSOut& it2)
	{
		const float delta_y = it2.pos.y - it0.pos.y;
		const auto dit0 = (it1 - it0) / delta_y;
		const auto dit1 = (it2 - it0) / delta_y;

		GSOut itEdge1 = it0;

		DrawFlatTriangle(it0, it1, it2, dit0, dit1, itEdge1);
	}
	void DrawFlatTriangle(const GSOut& it0, const GSOut& it1, const GSOut& it2,
		const GSOut& dit0, const GSOut& dit1, GSOut itEdge1)
	{
		GSOut itEdge0 = it0;

		const int yStart = std::max((int)ceil(it0.pos.y - 0.5f), 0);
		const int yEnd = std::min((int)ceil(it2.pos.y - 0.5f), int(Graphics::ScreenHeight));

		itEdge0 += dit0 * (float(yStart) + 0.5f - it0.pos.y);
		itEdge1 += dit1 * (float(yStart) + 0.5f - it0.pos.y);

		for (int y = yStart; y < yEnd; y++,
			itEdge0 += dit0, itEdge1 += dit1)
		{
			const int xStart = std::max((int)ceil(itEdge0.pos.x - 0.5f), 0);
			const int xEnd = std::min((int)ceil(itEdge1.pos.x - 0.5f), int(Graphics::ScreenWidth));

			GSOut iLine = itEdge0;

			const float dx = itEdge1.pos.x - itEdge0.pos.x;
			const auto diLine = (itEdge1 - iLine) / dx;

			iLine += diLine * (float(xStart) + 0.5f - itEdge0.pos.x);

			for (int x = xStart; x < xEnd; x++, iLine += diLine)
			{
				const float w = 1.0f / iLine.pos.w;

				if (pZb->TestAndSet(x, y, iLine.pos.z))
				{
					const auto attr = iLine * w;
					gfx.PutPixel(x, y, effect.ps(attr));
				}
			}
		}
	}
public:
	Effect effect;
private:
	Graphics& gfx;
	NDCScreenTransformer pst;
	std::shared_ptr<ZBuffer> pZb;
};