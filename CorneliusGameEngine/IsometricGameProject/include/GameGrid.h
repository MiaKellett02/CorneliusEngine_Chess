#pragma once

//Project includes.
#include "IsometricTilemap.h"
#include "Application.h"

//Library includes.

//Class definition.
class GameGrid {
public:
	//Constructor and destructor.
	GameGrid() = default;

	GameGrid(int a_gridWidth, int a_gridHeight, const std::string& a_defaultTileSprite) {
		m_environmentTilemap = IsometricTilemap("_environment", a_gridWidth, a_gridHeight);

		m_environmentTilemap.AddNewTileType("_default", a_defaultTileSprite, Application::Instance()->GetRenderer());

		//On each odd x and y tile, tint it bright red.
		Colour white = Colour(255, 255, 255);
		white.Darken(0.1f);
		for (int y = 0; y < m_environmentTilemap.GetHeight(); y++) {
			for (int x = 0; x < m_environmentTilemap.GetWidth(); x++) {
				m_environmentTilemap.GetTilemapList()[x + y * m_environmentTilemap.GetWidth()].tint = white;
			}
		}
	}

	~GameGrid();

	//Getters and setters.
	IsometricTilemap* GetEnvironmentTilemap() {
		return &m_environmentTilemap;
	}

private:
	IsometricTilemap m_environmentTilemap;
};