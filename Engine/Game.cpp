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
#include "CubeSkinScene.h"
#include "CubeVertexColorScene.h"
#include "CubeSolidScene.h"
#include "DoubleCubeScene.h"
#include "VertexWaveScene.h"
#include "CubeVertexPositionColorScene.h"
#include "CubeSolidGeometryScene.h"
#include "CubeFlatShadingIndependentScene.h"
#include "GeometryFlatShadingScene.h"
#include "Sphere.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	scenes.push_back(std::make_unique<GeometryFlatShadingScene>(gfx,
		Sphere::GetPlain<GeometryFlatShadingScene::Vertex>()));
	scenes.push_back(std::make_unique<CubeFlatShadingIndependentScene>(gfx));
	scenes.push_back(std::make_unique<GeometryFlatShadingScene>(gfx,
		IndexedTriangleList<GeometryFlatShadingScene::Vertex>::Load("Models//bunny.obj")));
	scenes.push_back(std::make_unique<CubeSkinScene>(gfx, L"Images//dice_skin.png"));
	scenes.push_back(std::make_unique<CubeVertexColorScene>(gfx));
	scenes.push_back(std::make_unique<CubeSolidScene>(gfx));
	scenes.push_back(std::make_unique<DoubleCubeScene>(gfx));
	scenes.push_back(std::make_unique<VertexWaveScene>(gfx));
	scenes.push_back(std::make_unique<CubeVertexPositionColorScene>(gfx));
	scenes.push_back(std::make_unique<CubeSolidGeometryScene>(gfx));
	curScene = scenes.begin();
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

	while (!wnd.kbd.KeyIsEmpty())
	{
		const auto e = wnd.kbd.ReadKey();
		if (e.GetCode() == VK_TAB && e.IsPress())
		{
			CycleScenes();
		}
	}

	(*curScene)->Update(wnd.kbd, wnd.mouse, dt);
}

void Game::CycleScenes()
{
	if (++curScene == scenes.end())
	{
		curScene = scenes.begin();
	}
}

void Game::ComposeFrame()
{
	(*curScene)->Draw();
}