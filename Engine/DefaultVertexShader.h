#pragma once

template <class Vertex>
class DefaultVertexShader
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
		return { (rotation * in.pos) + translation, in };
	}
private:
	Mat3 rotation;
	Vec3 translation;
};