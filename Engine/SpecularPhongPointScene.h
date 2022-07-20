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
		mod_pos.z = itlist.GetRadius() * 1.6f;

		for (auto& v : lightIndicator.vertices)
		{
			v.color = Colors::White;
		}
	}
	virtual void Update(Keyboard& kbd, Mouse& mouse, float dt) override
	{
		if (kbd.KeyIsPressed('W'))
		{
			cam_pos.z += cam_speed * dt;
		}
		if (kbd.KeyIsPressed('A'))
		{
			cam_pos.x -= cam_speed * dt;
		}
		if (kbd.KeyIsPressed('S'))
		{
			cam_pos.z -= cam_speed * dt;
		}
		if (kbd.KeyIsPressed('D'))
		{
			cam_pos.x += cam_speed * dt;
		}
	}
	virtual void Draw() override
	{
		pipeline.BeginFrame();


		const auto proj = Mat4::ProjectionHFOV( hfov, aspect_ratio, 0.01f, 10.0f);
		const auto view = Mat4::Translation(-cam_pos);

		// set pipeline transform
		pipeline.effect.vs.BindWorld(
			Mat4::Translation(mod_pos) *
			Mat4::RotationX(theta_x) *
			Mat4::RotationY(theta_y) *
			Mat4::RotationZ(theta_z));
		pipeline.effect.vs.BindView(view);
		pipeline.effect.vs.BindProjection(proj);

		pipeline.effect.ps.SetLightPosition(light_pos);
		// render triangles
		pipeline.Draw(itlist);

		liPipeline.effect.vs.BindWorld(Mat4::Translation(light_pos));
		liPipeline.effect.vs.BindView(view);
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

	static constexpr float aspect_ratio = float(Graphics::ScreenWidth) / float(Graphics::ScreenHeight);
	static constexpr float hfov = 120.0f;
	static constexpr float vfov = hfov / aspect_ratio;

	static constexpr float htrack = hfov / float(Graphics::ScreenWidth);
	static constexpr float vtrack = vfov / float(Graphics::ScreenHeight);
	static constexpr float cam_speed = 1.0f;
	Vec3 cam_pos = { 0.0f, 0.0f, 0.0f };

	Vec3 mod_pos = { 0.0f, 0.0f, 2.0f };
	float theta_x = 0.0f;
	float theta_y = 135.0f;
	float theta_z = 0.0f;

	Vec3 light_pos = { 0.0f, 0.0f, 0.6f };
};