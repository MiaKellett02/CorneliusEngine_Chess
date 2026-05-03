#include "MainMenuScene.h"
#include "Logging.h"
#include "InputHandler.h"

MainMenuScene::~MainMenuScene()
{
}

void MainMenuScene::SetupScene()
{
	// Perform any setup for the scene here. This will be called when the scene is loaded, but before the first update.

	// WE ARE DOING THE SCENE SETUP IN THE FIRST UPDATE INSTEAD OF HERE BECAUSE WE NEED TO WAIT FOR THE SCENE TO BE LOADED BEFORE WE CAN MANIPULATE SCENE STATE.
}

void MainMenuScene::UpdateScene(double a_deltaTime)
{
	if (m_firstFrame) {
		m_firstFrame = false; // Set the flag to false after the first update.

		//Build the background tilemap with a checkerboard pattern.
		m_backgroundTilemap = IsometricTilemap("_mainMenuBackground", 8, 8);
		m_backgroundTilemap.AddNewTileType("_default", m_defaultTileSprite, Application::Instance()->GetRenderer());

		Colour white = Colour(255, 255, 255);
		Colour dark = Colour(80, 80, 120);
		for (int y = 0; y < m_backgroundTilemap.GetHeight(); y++) {
			for (int x = 0; x < m_backgroundTilemap.GetWidth(); x++) {
				bool isDark = (x + y) % 2 == 1;
				m_backgroundTilemap.GetTilemapList()[x + y * m_backgroundTilemap.GetWidth()].tint = isDark ? dark : white;
			}
		}

		//Add the background tilemap to the scene's render list.
		m_sceneTilemaps.push_back(&m_backgroundTilemap);

		//Print main menu instructions to the console.
		std::cout << "=============================" << std::endl;
		std::cout << "   ISOMETRIC CHESS GAME      " << std::endl;
		std::cout << "=============================" << std::endl;
		std::cout << " Left click to start game   " << std::endl;
		std::cout << " Press Escape to quit       " << std::endl;
		std::cout << "=============================" << std::endl;
	}

	Scene::UpdateScene(a_deltaTime); // Call the base scene update to ensure all entities in the scene are updated.

	//Check for a fresh left mouse click to transition to the chess scene.
	bool mousePressed = InputHandler::LeftMouseButtonPressed();
	if (mousePressed && !m_prevMousePressed) {
		Application::Instance()->LoadSceneByName("ChessScene");
	}
	m_prevMousePressed = mousePressed;
}

void MainMenuScene::ShutdownScene()
{
	m_firstFrame = true; // Reset the first frame flag for when this scene is loaded again in the future.
	m_prevMousePressed = false; // Reset the mouse state.
	m_sceneTilemaps.clear(); // Clear the tilemap render list so it is rebuilt cleanly on next load.
}
