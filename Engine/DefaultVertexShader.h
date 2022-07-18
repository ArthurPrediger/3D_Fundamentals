#pragma once

template <class Vertex>
class DefaultVertexShader
{
public:
	typedef Vertex Output;
public:
	void BindWorld(const Mat4& transformation_in)
	{
		world = transformation_in;
	}
	void BindProjection(const Mat4& transformation_in)
	{
		proj = transformation_in;
		worldProj = proj * world;
	}
	Output operator()(const Vertex& v) const
	{
		const auto pos = Vec4(v.pos);

		return { worldProj * pos, v };
	}
	const Mat4& GetProj() const
	{
		return proj;
	}
private:
	Mat4 world = Mat4::Identity();
	Mat4 proj = Mat4::Identity();
	Mat4 worldProj = Mat4::Identity();
};