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
	//Add the gamegrid tilemaps to the scene list to render.
	m_sceneTilemaps.push_back(m_gameGrid.GetEnvironmentTilemap());

	//Initialize the chess board, which will add the pieces to the scene as entities.
}

void MainChessScene::UpdateScene(double a_deltaTime)
{
	if (m_firstFrame) {
		m_firstFrame = false; // Set the flag to false after the first update.
		//Perform any setup that needs to happen after the scene is loaded, but before the first update.
		m_chessBoard = new ChessBoard();
	}

	Scene::UpdateScene(a_deltaTime); // Call the base scene update to ensure all entities in the scene are updated.

	//Get the position of the mouse and use that to get the isometric coordinate it is hovering over..
	Vector2Int mousePos = InputHandler::GetMousePosition();
	Vector2Int mouseIsoPos = Application::Instance()->GetRenderer().GetIsometricGridPosFromScreenCoords(mousePos, true);
	/*CorneliusEngine::Log(("IsoPos(" + std::to_string(mouseIsoPos.x) + ", " + std::to_string(mouseIsoPos.y) + ")") +
						 (" MousePos(" + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y) + ")") +
						 (" CamOffset(" + std::to_string(Application::Instance()->GetRenderer().GetCameraOffset().x) + ", " + std::to_string(Application::Instance()->GetRenderer().GetCameraOffset().y) + ")"));*/


	//Check mouse button states.
	bool leftButtonPressed = InputHandler::LeftMouseButtonPressed();
	bool rightButtonPressed = InputHandler::RightMouseButtonPressed();
	bool isValidPosition = m_gameGrid.GetEnvironmentTilemap()->IsValidPosition(mouseIsoPos);

	//Find which position is being selected by the player. If the left mouse button is pressed, select the position. If the right mouse button is pressed, clear the selections.
	if (leftButtonPressed && isValidPosition) {
		if (selectedPosOne == UNSELECTED_VALUE) {
			selectedPosOne = mouseIsoPos;
		}
		else if (selectedPosTwo == UNSELECTED_VALUE && !(selectedPosTwo == selectedPosOne)) {
			selectedPosTwo = mouseIsoPos;
		}
	}
	else if (rightButtonPressed) {
		//Clear the selections
		selectedPosOne = UNSELECTED_VALUE;
		selectedPosTwo = UNSELECTED_VALUE;
	}

	//Loop over all the chess pieces and reset the hover state.
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			ChessPiece& piece = m_chessBoard->GetPieceAtPosition(x, y);
			if (piece.GetPieceType() != ChessPiece::PieceType::NO_TYPE) {
				piece.SetIsHovered(false);
			}
		}
	}

	//Tint the position the mouse is hovering over a dark grey if no position is currently selected.
	if (selectedPosOne == UNSELECTED_VALUE && selectedPosTwo == UNSELECTED_VALUE) {
		//Reset the colours of the grid.
		m_gameGrid.ResetGridColours();

		//Tint the hovered tile a darker grey.
		if (isValidPosition) {
			int tileIndex = mouseIsoPos.x + mouseIsoPos.y * m_gameGrid.GetEnvironmentTilemap()->GetWidth();
			Colour originalColour = m_gameGrid.GetEnvironmentTilemap()->GetTilemapList()[tileIndex].tint;
			Colour hoverColour = Colour(originalColour.r / 3, originalColour.g / 3, originalColour.b / 3);
			m_gameGrid.GetEnvironmentTilemap()->GetTilemapList()[tileIndex].tint = hoverColour;

			//Make the chess piece at this position (if there is one) also be tinted to indicate it is being hovered over.
			ChessPiece& pieceAtHoverPos = m_chessBoard->GetPieceAtPosition(mouseIsoPos.x, mouseIsoPos.y);
			if (pieceAtHoverPos.GetPieceType() != ChessPiece::PieceType::NO_TYPE) {
				pieceAtHoverPos.SetIsHovered(true);
			}
		}
	}
}

void MainChessScene::ShutdownScene()
{
}