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

		//Do the colours.
		ResetGridColours();
	}

	~GameGrid();

	//Public functions.
	void ResetGridColours() {
		//Do the chess board pattern by tinting every odd tile grey.
		Colour white = Colour(255, 255, 255);
		Colour grey = Colour(150, 150, 150);
		for (int y = 0; y < m_environmentTilemap.GetHeight(); y++) {
			for (int x = 0; x < m_environmentTilemap.GetWidth(); x++) {
				m_environmentTilemap.GetTilemapList()[x + y * m_environmentTilemap.GetWidth()].tint = white;

				//When y is even, make the odd x tiles grey. When y is odd, make the even x tiles grey.
				if (y % 2 == 1) {
					if (x % 2 == 0) {
						m_environmentTilemap.GetTilemapList()[x + y * m_environmentTilemap.GetWidth()].tint = grey;
					}
				}
				else {
					if (x % 2 != 0) {
						m_environmentTilemap.GetTilemapList()[x + y * m_environmentTilemap.GetWidth()].tint = grey;
					}
				}
			}
		}
	}

	//Getters and setters.
	IsometricTilemap* GetEnvironmentTilemap() {
		return &m_environmentTilemap;
	}

private:
	//Private variables.
	IsometricTilemap m_environmentTilemap;

	//Private functions.
};