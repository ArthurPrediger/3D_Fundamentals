#pragma once

#include "Pipeline.h"
#include "DefaultGeometryShader.h"

class SpecularPhongPointEffect
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
			n(src.n),
			pos(pos)
		{}
		Vertex(const Vec3& pos, const Vec3& n)
			:
			n(n),
			pos(pos)
		{}
	public:
		Vec3 pos;
		Vec3 n;
	};

	class VertexShader
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
				n(src.n),
				worldPos(src.worldPos),
				pos(pos)
			{}
			Output(const Vec3& pos, const Vec3& n, const Vec3& worldPos)
				:
				n(n),
				worldPos(worldPos),
				pos(pos)
			{}
			Output& operator +=(const Output& rhs)
			{
				pos += rhs.pos;
				n += rhs.n;
				worldPos += rhs.worldPos;
				return *this;
			}
			Output operator +(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator -=(const Output& rhs)
			{
				pos -= rhs.pos;
				n -= rhs.n;
				worldPos -= rhs.worldPos;
				return *this;
			}
			Output operator -(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator *=(float rhs)
			{
				pos *= rhs;
				n *= rhs;
				worldPos *= rhs;
				return *this;
			}
			Output operator *(float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator /=(float rhs)
			{
				pos /= rhs;
				n /= rhs;
				worldPos /= rhs;
				return *this;
			}
			Output operator /(float rhs) const
			{
				return Output(*this) /= rhs;
			}
		public:
			Vec3 pos;
			Vec3 n;
			Vec3 worldPos;
		};
	public:
		void BindRotation(const Mat3& rotation_in)
		{
			rotation = rotation_in;
		}
		void BindTranslation(const Vec3& translation_in)
		{
			translation = translation_in;
		}
		Output operator()(const Vertex& v) const
		{
			const auto pos = rotation * v.pos + translation;

			return { pos, rotation * v.n, pos };
		}
	private:
		Mat3 rotation;
		Vec3 translation;
	};

	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

	class PixelShader
	{
	public:
		template<class Input>
		Color operator()(const Input& in) const
		{
			const auto surf_norm = in.n.GetNormalized();

			const auto vec_to_l = light_pos - in.worldPos;
			const auto dist = vec_to_l.Len();
			const auto dir = vec_to_l / dist;

			const auto attenuation = 1.0f /
				(quadratic_attenuation * sq(dist) + linear_attenuation * dist + constant_attenuation);

			const auto d = light_diffuse * attenuation * std::max(0.0f, surf_norm * dir);

			const auto w = surf_norm * (vec_to_l * surf_norm);
			const auto r = w * 2.0f - vec_to_l;

			const auto s = light_diffuse * specular_intensity * 
				std::powf(std::max(0.0f, -r.GetNormalized() * in.worldPos.GetNormalized()), specular_power);

			return Color(material_color.GetHadamard(d + light_ambient + s).GetSaturated() * 255.0f);
		}
		void SetDiffuseLight(const Vec3& c)
		{
			light_diffuse = c;
		}
		void SetAmbientLight(const Vec3& c)
		{
			light_ambient = c;
		}
		void SetLightPosition(const Vec3& lp)
		{
			light_pos = lp;
		}
		void SetMaterialColor(Color c)
		{
			material_color = Vec3(c);
		}
	private:
		Mat3 rotation;
		Vec3 translation;
		Vec3 light_pos = { 0.0f, 0.0f, 0.5f };
		Vec3 light_diffuse = { 1.0f, 1.0f, 1.0f };
		Vec3 light_ambient = { 0.1f, 0.1f, 0.1f };
		Vec3 material_color = { 0.8f, 0.85f, 1.0f };
		float quadratic_attenuation = 2.69f;
		float linear_attenuation = 1.05f;
		float constant_attenuation = 0.369f;
		float specular_intensity = 0.6f;
		float specular_power = 30.0f;
	};
public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};