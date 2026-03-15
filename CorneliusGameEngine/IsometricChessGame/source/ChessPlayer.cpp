//Project includes.
#include "ChessPlayer.h"
#include "Colour.h"
#include "InputHandler.h"
#include "Application.h"
#include "ChessPiece.h"
#include "Logging.h"
#include "ChessBoard.h"
#include "Vector2.h"
#include "Scene.h"
#include "Entity.h"
#include "ChessPieceRendererComponent.h"

//Library includes.
#include <vector>

//Function definitions.

ChessPlayer::ChessPlayer()
{
}

ChessPlayer::~ChessPlayer()
{
}

void ChessPlayer::Update(double a_deltaTime, ChessBoard* a_mainChessBoard)
{
	if (a_mainChessBoard->IsWhitePlayersTurn() && m_isBlackPlayer) {
		return; // If it is the white players turn and we are the black player, return;
	}
	else if (!a_mainChessBoard->IsWhitePlayersTurn() && !m_isBlackPlayer) {
		return; // If not the black players turn and we are the white player, return;
	}


	if (m_isHumanPlayer) {
		//Do human player input handling and logic here.
		HandleHumanPlayerInput(a_deltaTime, a_mainChessBoard);
	}
	else {
		//Do AI player logic here.
	}
}

void ChessPlayer::HandleHumanPlayerInput(double a_deltaTime, ChessBoard* a_mainChessBoard)
{
	//Get the position of the mouse and use that to get the isometric coordinate it is hovering over..
	Vector2Int mousePos = InputHandler::GetMousePosition();
	Vector2Int mouseIsoPos = Application::Instance()->GetRenderer().GetIsometricGridPosFromScreenCoords(mousePos, true);
	/*CorneliusEngine::Log(("IsoPos(" + std::to_string(mouseIsoPos.x) + ", " + std::to_string(mouseIsoPos.y) + ")") +
						 (" MousePos(" + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y) + ")") +
						 (" CamOffset(" + std::to_string(Application::Instance()->GetRenderer().GetCameraOffset().x) + ", " + std::to_string(Application::Instance()->GetRenderer().GetCameraOffset().y) + ")"));*/


						 //Check mouse button states.
	bool leftButtonPressed = InputHandler::LeftMouseButtonPressed();
	bool rightButtonPressed = InputHandler::RightMouseButtonPressed();
	ChessPiece& pieceAtHoverPos = a_mainChessBoard->GetPieceAtPosition(mouseIsoPos.x, mouseIsoPos.y);
	bool isValidPosition = a_mainChessBoard->GetGameGrid().GetEnvironmentTilemap()->IsValidPosition(mouseIsoPos);

	//Find which position is being selected by the player. If the left mouse button is pressed, select the position. If the right mouse button is pressed, clear the selections.
	if (currentCooldownTime > 0) {
		// Decrease the cooldown timer by the delta time.
		currentCooldownTime -= static_cast<float>(a_deltaTime);
	}
	else {
		currentCooldownTime = 0; // Ensure the cooldown time does not go below 0.
		if (leftButtonPressed && isValidPosition) {
			if (selectedPosOne == UNSELECTED_VALUE && (pieceAtHoverPos.GetPieceType() != ChessPiece::PieceType::NO_TYPE)) {
				CorneliusEngine::Log("Selected Piece on position: (" + std::to_string(mouseIsoPos.x) + ", " + std::to_string(mouseIsoPos.y) + ")");
				selectedPosOne = mouseIsoPos;
				currentCooldownTime = buttonPressCooldown; // Reset the cooldown timer after a button press is registered.
			}
			else if (selectedPosTwo == UNSELECTED_VALUE && !(selectedPosTwo == selectedPosOne)) {
				CorneliusEngine::Log("Selected Second Piece on position: (" + std::to_string(mouseIsoPos.x) + ", " + std::to_string(mouseIsoPos.y) + ")");
				selectedPosTwo = mouseIsoPos;
				currentCooldownTime = buttonPressCooldown; // Reset the cooldown timer after a button press is registered.
			}
		}
		else if (rightButtonPressed) {
			//Clear the selections
			selectedPosOne = UNSELECTED_VALUE;
			selectedPosTwo = UNSELECTED_VALUE;
			//CorneliusEngine::Log("Cleared selected positions.");
			currentCooldownTime = buttonPressCooldown; // Reset the cooldown timer after a button press is registered.
		}
	}

	//Loop over all the chess pieces and reset the hover state.
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			ChessPiece& piece = a_mainChessBoard->GetPieceAtPosition(x, y);
			if (piece.GetPieceType() != ChessPiece::PieceType::NO_TYPE) {
				piece.SetIsHovered(false);
			}
		}
	}

	//Tint the position the mouse is hovering over a dark grey if no position is currently selected.
	if (selectedPosOne == UNSELECTED_VALUE && selectedPosTwo == UNSELECTED_VALUE) {
		//Reset the colours of the grid.
		a_mainChessBoard->GetGameGrid().ResetGridColours();
	}
	else if (selectedPosOne != UNSELECTED_VALUE && selectedPosTwo == UNSELECTED_VALUE) {
		a_mainChessBoard->GetGameGrid().ResetGridColours();
		// If the first position is selected, and the second one is not,
		// tint the piece on the selected tile green, and then tint any tiles that are valid moves green as well to indicate the valid moves for this piece.
		// Then, tint any tiles that are invalid moves red to indicate they are invalid moves for this piece.

		//Get the piece at the selected position & all it's valid moves.
		ChessPiece& selectedPiece = a_mainChessBoard->GetPieceAtPosition(selectedPosOne.x, selectedPosOne.y);

		//Check if the piece at the selected position is a valid piece (not an empty tile & is the current colour).
		ChessPiece::PieceColour currentPlayerColour = m_isBlackPlayer ? ChessPiece::PieceColour::BLACK : ChessPiece::PieceColour::WHITE;
		if (selectedPiece.GetPieceColour() != currentPlayerColour) {
			selectedPosOne = UNSELECTED_VALUE; // Clear the selected position, as it is not a valid piece for the current player.
			return; // Exit the function if the selected piece is not a valid piece for the current player.
		}

		// Get all the valid moves for this piece.
		std::vector<Vector2Int>& validMoves = selectedPiece.GetAllValidPieceMoves();

		//Loop over all the tiles on the board and tint them based on whether they are a valid move for this piece or not.
		Colour redTint = Colour(255, 0, 0);
		Colour greenTint = Colour(0, 255, 0);

		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				Vector2Int currentPos = Vector2Int(x, y);
				int tileIndex = x + y * a_mainChessBoard->GetGameGrid().GetEnvironmentTilemap()->GetWidth();
				//Check if this position is in the valid moves vector for the selected piece.
				bool isValidMove = false;
				for (int i = 0; i < static_cast<int>(validMoves.size()); i++) {
					if (validMoves[i] == currentPos) {
						isValidMove = true;
						break;
					}
				}
				Colour originalColour = a_mainChessBoard->GetGameGrid().GetEnvironmentTilemap()->GetTilemapList()[tileIndex].tint;
				Colour newColour;
				if (isValidMove) {
					//Valid move, tint green.
					newColour = Colour((originalColour.r + greenTint.r) / 2, (originalColour.g + greenTint.g) / 2, (originalColour.b + greenTint.b) / 2);
				}
				else {
					//Not a valid move, tint red.
					newColour = Colour((originalColour.r + redTint.r) / 2, (originalColour.g + redTint.g) / 2, (originalColour.b + redTint.b) / 2);
				}

				//Apply the new colour to the tile.
				a_mainChessBoard->GetGameGrid().GetEnvironmentTilemap()->GetTilemapList()[tileIndex].tint = newColour;
			}
		}
	}
	else if (selectedPosOne != UNSELECTED_VALUE && selectedPosTwo != UNSELECTED_VALUE) {
		a_mainChessBoard->GetGameGrid().ResetGridColours();
		//Make the move from selectedPosOne to selectedPosTwo, and then clear the selected positions.
		bool moveValid = a_mainChessBoard->MakeGameMove(selectedPosOne, selectedPosTwo);
		if (!moveValid) {
			//Potentially show UI feedback to indicate the move was invalid, such as flashing the selected tiles red or something similar.
		}

		selectedPosOne = UNSELECTED_VALUE;
		selectedPosTwo = UNSELECTED_VALUE;
	}

	//Tint the hovered tile a darker colour.
	//Get the tile index of the position the mouse is hovering over, so we can tint it if needed.
	mouseIsoPos = Application::Instance()->GetRenderer().GetIsometricGridPosFromScreenCoords(mousePos, true);
	int hoverTileIndex = mouseIsoPos.x + mouseIsoPos.y * a_mainChessBoard->GetGameGrid().GetEnvironmentTilemap()->GetWidth();

	isValidPosition = a_mainChessBoard->GetGameGrid().GetEnvironmentTilemap()->IsValidPosition(mouseIsoPos);
	if (isValidPosition) {
		Colour originalColour = a_mainChessBoard->GetGameGrid().GetEnvironmentTilemap()->GetTilemapList()[hoverTileIndex].tint;
		Colour hoverColour = Colour(originalColour.r / 3, originalColour.g / 3, originalColour.b / 3);
		a_mainChessBoard->GetGameGrid().GetEnvironmentTilemap()->GetTilemapList()[hoverTileIndex].tint = hoverColour;

		//Make the chess piece at this position (if there is one) also be tinted to indicate it is being hovered over.
		if (pieceAtHoverPos.GetPieceType() != ChessPiece::PieceType::NO_TYPE) {
			pieceAtHoverPos.SetIsHovered(true);
		}
	}
}
