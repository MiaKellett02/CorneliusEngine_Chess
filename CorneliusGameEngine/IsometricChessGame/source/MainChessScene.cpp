#include "MainChessScene.h"
#include "Logging.h"
#include "InputHandler.h"
#include "Timer.h"
#include "ChessBoard.h"

MainChessScene::~MainChessScene()
{
}

void MainChessScene::SetupScene()
{
	// Perform any setup for the scene here. This will be called when the scene is loaded, but before the first update.

	// WE ARE DOING THE SCENE SETUP IN THE FIRST UPDATE INSTEAD OF HERE BECAUSE WE NEED TO WAIT FOR THE SCENE TO BE LOADED BEFORE WE CAN CREATE THE CHESS BOARD, WHICH IS AN ENTITY THAT NEEDS TO BE ADDED TO THE SCENE. IF WE TRIED TO CREATE THE CHESS BOARD HERE, IT WOULD NOT BE ADDED TO THE SCENE AND WOULD NOT BE UPDATED OR RENDERED.
}

void MainChessScene::UpdateScene(double a_deltaTime)
{
	if (m_firstFrame) {
		m_firstFrame = false; // Set the flag to false after the first update.
		//Perform any setup that needs to happen after the scene is loaded, but before the first update.
	//Initialize the chess board, which will add the pieces to the scene as entities.
		m_chessBoard = new ChessBoard(m_defaultTileSprite);

		//Add the gamegrid tilemaps to the scene list to render.
		m_sceneTilemaps.push_back(m_chessBoard->GetGameGrid().GetEnvironmentTilemap());
	}

	Scene::UpdateScene(a_deltaTime); // Call the base scene update to ensure all entities in the scene are updated.

	m_chessBoard->UpdateChessBoard(a_deltaTime); // Update the chess board, which will handle any logic related to the chess pieces and their interactions.
}

void MainChessScene::ShutdownScene()
{
	m_firstFrame = true; // Reset the first frame flag for when this scene is loaded again in the future.

	// Perform any cleanup for memory management. For example, if the chess board was allocated on the heap, it should be deleted here.
	delete m_chessBoard;
}