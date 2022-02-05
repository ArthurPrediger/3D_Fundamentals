/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = 1.0f / 60.0f;
	if (wnd.kbd.KeyIsPressed('Q'))
	{
		theta_x = wrap_angle(theta_x + dTheta * dt);
	}
	if (wnd.kbd.KeyIsPressed('W'))
	{
		theta_y = wrap_angle(theta_y + dTheta * dt);
	}
	if (wnd.kbd.KeyIsPressed('E'))
	{
		theta_z = wrap_angle(theta_z + dTheta * dt);
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		theta_x = wrap_angle(theta_x - dTheta * dt);
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		theta_y = wrap_angle(theta_y - dTheta * dt);
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		theta_z = wrap_angle(theta_z - dTheta * dt);
	}
	if (wnd.kbd.KeyIsPressed('F'))
	{
		offset_z += 2.0f * dt;
	}
	if (wnd.kbd.KeyIsPressed('R'))
	{
		offset_z -= 2.0f * dt;
	}
}

void Game::ComposeFrame()
{
	std::vector<Color> colors
	{
		Colors::Blue,
		Colors::Cyan,
		Colors::Gray,
		Colors::Green,
		Colors::Red,
		Colors::Yellow,
		Colors::Blue,
		Colors::Cyan,
		Colors::Gray,
		Colors::Green,
		Colors::Red,
		Colors::Yellow
	};

	auto triangles = cube.GetTriangles();
	const Mat3 rot = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);
	for (auto& v : triangles.vertices)
	{
		v = rot * v;
		//v = Mat3::ScaleIndependent(float(Graphics::ScreenHeight) / float(Graphics::ScreenWidth), 1.0f) * v;
		v += {0.0f, 0.0f, offset_z};
	}

	for (size_t i = 0, end = triangles.indices.size() / 3; i < end; i++)
	{
		const Vec3& v0 = triangles.vertices[triangles.indices[i * 3]];
		const Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]];
		const Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]];
		triangles.cullFlags[i] = (v1 - v0) % (v2 - v0) * v0 >= 0.0f;
	}

	for (auto& v : triangles.vertices)
	{
		pst.Transform(v);
	}

	for (size_t i = 0, end = triangles.indices.size() / 3; i < end; i++)
	{
		if (!triangles.cullFlags[i])
		{
			gfx.DrawTriangle(
				triangles.vertices[triangles.indices[i * 3]], 
				triangles.vertices[triangles.indices[i * 3 + 1]],
				triangles.vertices[triangles.indices[i * 3 + 2]], 
				colors[i]);
		}
	}
}