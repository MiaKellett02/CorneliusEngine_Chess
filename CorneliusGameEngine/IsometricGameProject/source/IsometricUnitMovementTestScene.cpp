#include "IsometricUnitMovementTestScene.h"
#include "Logging.h"
#include "InputHandler.h"
#include "AStarPathfinding.h"
#include "Timer.h"

IsometricUnitMovementTestScene::~IsometricUnitMovementTestScene()
{
}

void IsometricUnitMovementTestScene::SetupScene()
{
	//Add the gamegrid tilemaps to the scene list to render.
	m_sceneTilemaps.push_back(m_gameGrid.GetEnvironmentTilemap());

	SetupNavmesh(); //Create the navmesh instance upon scene loading.
}

void IsometricUnitMovementTestScene::UpdateScene(double a_deltaTime)
{
	//Get the position of the mouse and use that to get the isometric coordinate it is hovering over..
	Vector2Int mousePos = InputHandler::GetMousePosition();
	Vector2Int mouseIsoPos = Application::Instance()->GetRenderer().GetIsometricGridPosFromScreenCoords(mousePos, true);
	/*CorneliusEngine::Log(("IsoPos(" + std::to_string(mouseIsoPos.x) + ", " + std::to_string(mouseIsoPos.y) + ")") +
						 (" MousePos(" + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y) + ")") +
						 (" CamOffset(" + std::to_string(Application::Instance()->GetRenderer().GetCameraOffset().x) + ", " + std::to_string(Application::Instance()->GetRenderer().GetCameraOffset().y) + ")"));*/

						 //Get some colours.
	Colour white(255, 255, 255);
	Colour grey(100, 100, 100);
	Colour red(255, 0, 0);

	//Check mouse button states.
	bool leftButtonPressed = InputHandler::LeftMouseButtonPressed();
	bool rightButtonPressed = InputHandler::RightMouseButtonPressed();
	bool isValidPosition = m_gameGrid.GetEnvironmentTilemap()->IsValidPosition(mouseIsoPos);

	if (leftButtonPressed && isValidPosition) {
		if (selectedPosOne == UNSELECTED_VALUE) {
			selectedPosOne = mouseIsoPos;
			//m_gameGrid.GetEnvironmentTilemap()->GetTile(mouseIsoPos).tint = red;
		}
		else if (selectedPosTwo == UNSELECTED_VALUE && !(selectedPosTwo == selectedPosOne)) {
			selectedPosTwo = mouseIsoPos;
			//m_gameGrid.GetEnvironmentTilemap()->GetTile(mouseIsoPos).tint = red;
		}
	}
	else if (rightButtonPressed) {
		//Clear the selections
		selectedPosOne = UNSELECTED_VALUE;
		selectedPosTwo = UNSELECTED_VALUE;
		calculatedPath.clear();
		ResetNavmesh();
	}

	if (selectedPosOne == selectedPosTwo && selectedPosOne != UNSELECTED_VALUE) {
		selectedPosTwo = UNSELECTED_VALUE;
	}

	//If the positions have been selected.
	if (selectedPosOne != UNSELECTED_VALUE && selectedPosTwo != UNSELECTED_VALUE && !(selectedPosTwo == selectedPosOne) && calculatedPath.size() <= 0) {
		DoPathfinding();
	}


	//Brighten the highlighted tile and make the others dark.
	if (calculatedPath.size() <= 0) {
		std::vector<IsometricTilemap::Tile> isoTiles = m_gameGrid.GetEnvironmentTilemap()->GetTilemapList();
		for (int y = 0; y < m_gameGrid.GetEnvironmentTilemap()->GetHeight(); y++) {
			for (int x = 0; x < m_gameGrid.GetEnvironmentTilemap()->GetWidth(); x++) {
				Vector2Int currentPos(x, y);
				int tileAccessIndex = x + y * m_gameGrid.GetEnvironmentTilemap()->GetWidth();

				if (x == mouseIsoPos.x && y == mouseIsoPos.y && currentPos != selectedPosOne && currentPos != selectedPosTwo) {
					//Make this bright white.
					m_gameGrid.GetEnvironmentTilemap()->GetTilemapList()[tileAccessIndex].tint = white;
				}
				else if (currentPos != selectedPosOne && currentPos != selectedPosTwo) {
					//Make this grey.
					m_gameGrid.GetEnvironmentTilemap()->GetTilemapList()[tileAccessIndex].tint = grey;
				}
			}
		}
	}
}

void IsometricUnitMovementTestScene::ShutdownScene()
{
}

void IsometricUnitMovementTestScene::DoPathfinding()
{
	m_navmeshReset = false;
	Colour white(255, 255, 255);
	Colour grey(100, 100, 100);
	Colour red(255, 0, 0);
	Colour blue(0, 0, 255);

	//Calculate path.
	CorneliusEngine::AStarPosition* startPos = nullptr;
	CorneliusEngine::AStarPosition* goalPos = nullptr;
	for (int i = 0; i < m_navmesh.size(); i++) {
		if (m_navmesh[i].position == selectedPosOne) {
			startPos = &m_navmesh[i];
		}
		else if (m_navmesh[i].position == selectedPosTwo) {
			goalPos = &m_navmesh[i];
		}
	}
	calculatedPath = CorneliusEngine::AStarPathfinding::FindPath(startPos, goalPos, m_checkedPositions);
	{
		for (int i = 0; i < m_checkedPositions.size(); i++) {
			m_gameGrid.GetEnvironmentTilemap()->GetTile(m_checkedPositions[i]).tint = blue;
		}

		Timer("Tint path timer");
		for (int i = 0; i < calculatedPath.size(); i++) {
			m_gameGrid.GetEnvironmentTilemap()->GetTile(calculatedPath[i]).tint = red;
		}
	}
}

void IsometricUnitMovementTestScene::ResetNavmesh()
{
	if (!m_navmeshReset) {
		Timer("Reset Navmesh Timer");
		for (int i = 0; i < m_navmesh.size(); i++) {
			m_navmesh[i].totalCostToStart = m_navmesh[i].START_COST;
			m_navmesh[i].connectingNodeToStart = nullptr;
		}
		m_navmeshReset = true;
	}
}

void IsometricUnitMovementTestScene::SetupNavmesh()
{
	Timer timer("Setup Navmesh Timer");
	for (int y = 0; y < m_gameGrid.GetEnvironmentTilemap()->GetHeight(); y++) {
		for (int x = 0; x < m_gameGrid.GetEnvironmentTilemap()->GetWidth(); x++) {
			Vector2Int currentPos(x, y);
			CorneliusEngine::AStarPosition newPos;
			newPos.position = currentPos;
			m_navmesh.push_back(newPos);
			
		}
	}

	//Setup connections between all nodes.
	for (int y = 0; y < m_gameGrid.GetEnvironmentTilemap()->GetHeight(); y++) {
		Timer("Navmesh setup for row " + std::to_string(y));
		for (int x = 0; x < m_gameGrid.GetEnvironmentTilemap()->GetWidth(); x++) {
			Vector2Int currentPos(x, y);
			int currentPosIndex = -1;
			for (int i = 0; i < m_navmesh.size(); i++) {
				if (m_navmesh[i].position == currentPos) {
					currentPosIndex = i;
					break;
				}
			}

			//Left hand side.
			if (x > 0) {
				Vector2Int leftPos(x - 1, y);
				for (int i = 0; i < m_navmesh.size(); i++) {
					if (m_navmesh[i].position == leftPos) {
						m_navmesh[currentPosIndex].MakeConnection(&m_navmesh[i], 1);
						break;
					}
				}
			}
			if (x > 0 && y < m_gameGrid.GetEnvironmentTilemap()->GetHeight()) {
				Vector2Int leftUpPos(x - 1, y + 1);
				for (int i = 0; i < m_navmesh.size(); i++) {
					if (m_navmesh[i].position == leftUpPos) {
						m_navmesh[currentPosIndex].MakeConnection(&m_navmesh[i], 1);
						break;
					}
				}
			}
			if (x > 0 && y > 0) {
				Vector2Int leftDownPos(x - 1, y - 1);
				for (int i = 0; i < m_navmesh.size(); i++) {
					if (m_navmesh[i].position == leftDownPos) {
						m_navmesh[currentPosIndex].MakeConnection(&m_navmesh[i], 1);
						break;
					}
				}
			}

			//Right Hand Side.
			if (x < m_gameGrid.GetEnvironmentTilemap()->GetWidth()) {
				Vector2Int rightPos(x + 1, y);
				for (int i = 0; i < m_navmesh.size(); i++) {
					if (m_navmesh[i].position == rightPos) {
						m_navmesh[currentPosIndex].MakeConnection(&m_navmesh[i], 1);
						break;
					}
				}

				if (y < m_gameGrid.GetEnvironmentTilemap()->GetHeight()) {
					Vector2Int rightUpPos(x + 1, y + 1);
					for (int i = 0; i < m_navmesh.size(); i++) {
						if (m_navmesh[i].position == rightUpPos) {
							m_navmesh[currentPosIndex].MakeConnection(&m_navmesh[i], 1);
							break;
						}
					}
				}

				if (y > 0) {
					Vector2Int rightDownPos(x + 1, y - 1);
					for (int i = 0; i < m_navmesh.size(); i++) {
						if (m_navmesh[i].position == rightDownPos) {
							m_navmesh[currentPosIndex].MakeConnection(&m_navmesh[i], 1);
							break;
						}
					}
				}
			}

			//Middle side.
			if (y > 0) {
				Vector2Int downPos(x, y - 1);
				for (int i = 0; i < m_navmesh.size(); i++) {
					if (m_navmesh[i].position == downPos) {
						m_navmesh[currentPosIndex].MakeConnection(&m_navmesh[i], 1);
						break;
					}
				}
			}

			if (y < m_gameGrid.GetEnvironmentTilemap()->GetHeight()) {
				Vector2Int upPos(x, y + 1);
				for (int i = 0; i < m_navmesh.size(); i++) {
					if (m_navmesh[i].position == upPos) {
						m_navmesh[currentPosIndex].MakeConnection(&m_navmesh[i], 1);
						break;
					}
				}
			}
		}
	}
}