#pragma once

#include "Pipeline.h"

class WaveVertexTextureEffect
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

	class VertexShader
	{
	public:
		typedef Vertex Output;
	public:
		void BindRotation(const Mat3& rotation_in)
		{
			rotation = rotation_in;
		}
		void BindTranslation(const Vec3& translation_in)
		{
			translation = translation_in;
		}
		Output operator()(const Vertex& in) const
		{
			Vec3 pos = rotation * in.pos + translation;
			pos.y += amplitude * sinf(time * scrollFreq + pos.x * waveFreq);
			return { pos, in.t };
		}
		void SetTime(float t)
		{
			time = t;
		}
	private:
		Mat3 rotation;
		Vec3 translation;
		float time = 0.0f;
		float waveFreq = 10.0f;
		float scrollFreq = 5.0f;
		float amplitude = 0.05f;
	};

	class GeometryShader
	{
	public:
		class Output
		{
		public:
			Output() = default;
			Output(const Vec3& pos)
				:
				pos(pos)
			{}
			Output(const Vec3& pos, const Output& src)
				:
				t(src.t),
				l(src.l),
				pos(pos)
			{}
			Output(const Vec3& pos, const Vec2& t, float l)
				:
				t(t),
				l(l),
				pos(pos)
			{}
			Output& operator +=(const Output& rhs)
			{
				pos += rhs.pos;
				t += rhs.t;
				return *this;
			}
			Output operator +(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator -=(const Output& rhs)
			{
				pos -= rhs.pos;
				t -= rhs.t;
				return *this;
			}
			Output operator -(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator *=(float rhs)
			{
				pos *= rhs;
				t *= rhs;
				return *this;
			}
			Output operator *(float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator /=(float rhs)
			{
				pos /= rhs;
				t /= rhs;
				return *this;
			}
			Output operator /(float rhs) const
			{
				return Output(*this) /= rhs;
			}
		public:
			Vec3 pos;
			Vec2 t;
			float l;
		};
	public:
		Triangle<Output> operator()(const VertexShader::Output& v0, const VertexShader::Output& v1,
			const VertexShader::Output& v2, size_t triangle_index) const
		{
			const auto n = ((v1.pos - v0.pos) % (v2.pos - v0.pos)).GetNormalized();

			const auto l = std::min(1.0f, diffuse * std::max(0.0f, -n * dir) + ambient);

			return { { v0.pos, v0.t, l }, { v1.pos, v1.t, l }, { v2.pos, v2.t, l } };
		}
		void SetDiffuseLight(float d)
		{
			ambient = d;
		}
		void SetAmbientLight(float a)
		{
			ambient = a;
		}
		void SetLightDirection(const Vec3& dl)
		{
			assert(dl.LenSq() >= 0.001f);
			dir = dl.GetNormalized();
		}
	private:
		Vec3 dir = { 0.0f, 0.0f, 1.0f };
		float diffuse = 1.0f;
		float ambient = 0.1f;
	};

	class PixelShader
	{
	public:
		template<class Input>
		Color operator()(const Input& in) const
		{
			const Vec3 color = Vec3(pTex->GetPixel(
				(unsigned int)std::min(in.t.x * tex_width, tex_clamp_x),
				(unsigned int)std::min(in.t.y * tex_height, tex_clamp_y)));

			return Color(color * in.l);
		}
		void BindTexture(const std::wstring& filename)
		{
			pTex = std::make_unique<Surface>(Surface::FromFile(filename));
			tex_width = float(pTex->GetWidth());
			tex_height = float(pTex->GetHeight());
			tex_clamp_x = tex_width - 1.0f;
			tex_clamp_y = tex_height - 1.0f;
		}
	private:
		std::unique_ptr<Surface> pTex;
		float tex_width;
		float tex_height;
		float tex_clamp_x;
		float tex_clamp_y;
	};
public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};