#pragma once

#include "Scene.h"
#include "Sphere.h"
#include "Mat.h"
#include "SpecularPhongPointEffect.h"
#include "SolidColorEffect.h"

class SpecularPhongPointScene : public Scene
{
public:
	typedef ::Pipeline<SpecularPhongPointEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
	typedef ::Pipeline<SolidColorEffect> LightIndicatorPipeline;
public:
	SpecularPhongPointScene(Graphics& gfx, IndexedTriangleList<Vertex> itl)
		:
		itlist(std::move(itl)),
		pZb(std::make_shared<ZBuffer>(gfx.ScreenWidth, gfx.ScreenHeight)),
		pipeline(gfx, pZb),
		liPipeline(gfx, pZb),
		Scene("Phong shader using point light scene")
	{
		itlist.AdjustToTrueCenter();
		offset_z = itlist.GetRadius() * 1.6f;

		for (auto& v : lightIndicator.vertices)
		{
			v.color = Colors::White;
		}
	}
	virtual void Update(Keyboard& kbd, Mouse& mouse, float dt) override
	{
		if (kbd.KeyIsPressed('W'))
		{
			theta_x = wrap_angle(theta_x + dTheta * dt);
		}
		if (kbd.KeyIsPressed('A'))
		{
			theta_y = wrap_angle(theta_y + dTheta * dt);
		}
		if (kbd.KeyIsPressed('Q'))
		{
			theta_z = wrap_angle(theta_z + dTheta * dt);
		}
		if (kbd.KeyIsPressed('S'))
		{
			theta_x = wrap_angle(theta_x - dTheta * dt);
		}
		if (kbd.KeyIsPressed('D'))
		{
			theta_y = wrap_angle(theta_y - dTheta * dt);
		}
		if (kbd.KeyIsPressed('E'))
		{
			theta_z = wrap_angle(theta_z - dTheta * dt);
		}
		if (kbd.KeyIsPressed('U'))
		{
			light_pos.z += 0.2f * dt;
		}
		if (kbd.KeyIsPressed('I'))
		{
			light_pos.y += 0.2f * dt;
		}
		if (kbd.KeyIsPressed('O'))
		{
			light_pos.z -= 0.2f * dt;
		}
		if (kbd.KeyIsPressed('J'))
		{
			light_pos.x -= 0.2f * dt;
		}
		if (kbd.KeyIsPressed('K'))
		{
			light_pos.y -= 0.2f * dt;
		}
		if (kbd.KeyIsPressed('L'))
		{
			light_pos.x += 0.2f * dt;
		}
		if (kbd.KeyIsPressed('R'))
		{
			offset_z += 2.0f * dt;
		}
		if (kbd.KeyIsPressed('F'))
		{
			offset_z -= 2.0f * dt;
		}
	}
	virtual void Draw() override
	{
		pipeline.BeginFrame();

		const float screenRatio = float(Graphics::ScreenWidth) / float(Graphics::ScreenHeight);
		//const auto proj = Mat4::Projection(2.0f * screenRatio, 2.0f, 1.0f, 10.0f);
		const auto proj = Mat4::ProjectionHFOV(120.0f, screenRatio, 1.0f, 10.0f);

		// set pipeline transform
		pipeline.effect.vs.BindWorld(
			Mat4::Translation(0.0f, 0.0f, offset_z) *
			Mat4::RotationX(theta_x) *
			Mat4::RotationY(theta_y) *
			Mat4::RotationZ(theta_z));
		pipeline.effect.vs.BindProjection(proj);

		pipeline.effect.ps.SetLightPosition(light_pos);
		// render triangles
		pipeline.Draw(itlist);

		liPipeline.effect.vs.BindWorld(Mat4::Translation(light_pos));
		liPipeline.effect.vs.BindProjection(proj);
		liPipeline.Draw(lightIndicator);
	}
private:
	std::shared_ptr<ZBuffer> pZb;
	IndexedTriangleList<Vertex> itlist;
	IndexedTriangleList<SolidColorEffect::Vertex> lightIndicator =
		Sphere::GetPlain<SolidColorEffect::Vertex>(0.05f);
	Pipeline pipeline;
	LightIndicatorPipeline liPipeline;
	static constexpr float dTheta = PI;
	float offset_z = 2.0f;
	float theta_x = 0.0f;
	float theta_y = 135.0f;
	float theta_z = 0.0f;
	Vec3 light_pos = { 0.0f, 0.0f, 0.6f };
};