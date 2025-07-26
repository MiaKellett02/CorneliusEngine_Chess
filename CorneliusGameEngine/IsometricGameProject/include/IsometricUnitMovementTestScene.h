#pragma once

//Project includes.
#include "Scene.h"
#include "GameGrid.h"
#include "AStarPathfinding.h"

//Library includes.
#include <string>
#include <iostream>

//Forward Declarations.
class GameGrid;

//Class defintion.
class IsometricUnitMovementTestScene : public Scene {
public:
	//Constructor and destructor.
	IsometricUnitMovementTestScene(std::string a_sceneName, const std::string& a_defaultTileSprite) : Scene(a_sceneName) {
		
		int terrainWidth = 8;			// 8x8 grid for a chess board.
		int terrainHeight = 8;
		/*std::cout << "Enter terrain width - minimum = 1 (in tiles): ";
		while (!(std::cin >> terrainWidth) || terrainWidth < 1) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input for width. Please enter an integer and make sure the width is atleast 1 tile: ";
		}
		std::cout << "Enter terrain height - minimum = 1 (in tiles): ";
		while (!(std::cin >> terrainHeight) || terrainHeight < 1) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input for height. Please enter an integer and make sure the height is atleast 1 tile: ";
		}*/

		m_gameGrid = GameGrid(terrainWidth, terrainHeight, a_defaultTileSprite);
	}
	~IsometricUnitMovementTestScene();

	// Inherited via Scene
	void SetupScene() override;
	void UpdateScene(double a_deltaTime) override;
	void ShutdownScene() override;


private:
	GameGrid m_gameGrid;


	const Vector2Int UNSELECTED_VALUE = Vector2Int(-1, -1);

	Vector2Int selectedPosOne = UNSELECTED_VALUE;
	Vector2Int selectedPosTwo = UNSELECTED_VALUE;

	std::vector<Vector2Int> calculatedPath;

	std::vector<CorneliusEngine::AStarPosition> m_navmesh;
	std::vector<Vector2Int> m_checkedPositions;
	bool m_navmeshReset = false;

	//Private functions.
	void DoPathfinding();
	void ResetNavmesh();
	void SetupNavmesh();
};