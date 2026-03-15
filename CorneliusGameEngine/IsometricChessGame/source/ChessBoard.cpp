//Project includes.
#include "ChessBoard.h"
#include "Scene.h"
#include "Application.h"
#include "Entity.h"
#include "ChessPieceRendererComponent.h"
#include "InputHandler.h"
#include "Logging.h"

void ChessBoard::Initialize(const std::string& a_defaultTileSprite) {
	m_gameGrid = GameGrid(8, 8, a_defaultTileSprite); // Initialize the game grid with the same size as the chess board, and a default tile sprite for rendering the tiles on the board.

	// Initialize the chess board with the starting positions of the pieces.
	int boardSizeX = 8;
	int boardSizeY = 8;
	m_boardState.clear(); // Create the 2D vector to represent the chess board.
	m_boardState.reserve(boardSizeX * boardSizeY);
	for (int y = 0; y < boardSizeY; y++) {
		for (int x = 0; x < boardSizeX; x++) {
			//Default to no piece.
			m_boardState.push_back(ChessPiece(ChessPiece::PieceColour::NO_COLOUR, ChessPiece::PieceType::NO_TYPE, this, Vector2Int(x, y)));
			if (x == 1 && y == 6) {
				int a = 0;
			}
		}
	}

	//Create entities that map to the pieces on the board and add them to the scene for rendering.
	for (int y = 0; y < boardSizeY; y++) {
		for (int x = 0; x < boardSizeX; x++) {
			//Spawn the pawns.
			if (y == 1) {
				//Spawn a black pawn for the second row.
				m_boardState.at(x + (y * boardSizeX)).UpdatePiece(ChessPiece::PieceColour::BLACK, ChessPiece::PieceType::PAWN);
			}
			else if (y == boardSizeY - 2) {
				//Spawn a white pawn on the 7th row.
				m_boardState.at(x + (y * boardSizeX)).UpdatePiece(ChessPiece::PieceColour::WHITE, ChessPiece::PieceType::PAWN);
			}

			if (x == 1 && y == 6) {
				int a = 0;
			}

			//Spawn an entity for this piece and add it to the scene.
			// Example: Adding a new entity to the active scene
			Scene* scene = Application::Instance()->GetActiveScene();

			//Size is set to (-1, -1) as the renderer will use the isometric tile size for rendering, so the entity's size does not need to be set.
			Entity* newEntity = new Entity(nullptr, Vector2(x, y), Vector2(-1, -1), "ChessPieceRenderer", "Player", "resources/WhitePawn.png");
			newEntity->DisableRendering(); // Disable rendering for this entity by default, as it will be enabled by the ChessPieceRendererComponent when it detects there is a piece to render.
			newEntity->EnableIsometricRendering(); // Enable isometric rendering for this entity, as it will be rendered in an isometric perspective.

			//Add the chess piece renderer component to the entity, which will handle rendering the piece based on the piece type and colour at this position on the board.
			ChessPieceRendererComponent* newComponent = new ChessPieceRendererComponent(newEntity, this, 0.016f); // Update 60 times a second.
			newEntity->AddComponent(newComponent);

			//Add the entity to the scene.
			std::vector<Entity*>& sceneEntities = scene->GetAllEntities();
			sceneEntities.push_back(newEntity);
		}
	}

	int end = 0;
}

ChessBoard::~ChessBoard()
{

}

ChessPiece& ChessBoard::GetPieceAtPosition(int x, int y) {
	// Check if m_boardState is valid and board size is 8x8
	constexpr int boardSizeX = 8;
	constexpr int boardSizeY = 8;
	if (x < 0 || x > 7 || y < 0 || y > 7) {
		// Out of bounds access, return a reference to a static empty piece.
		static ChessPiece emptyPiece(ChessPiece::PieceColour::NO_COLOUR, ChessPiece::PieceType::NO_TYPE, this, Vector2Int(-1, -1));
		return emptyPiece;
	}
	return m_boardState.at(x + (y * boardSizeX));
}

void ChessBoard::UpdateChessBoard(double a_deltaTime)
{
	if (currentUpdateTime < UPDATE_INTERVAL) {
		currentUpdateTime += static_cast<float>(a_deltaTime);
		return; // Not enough time has passed since the last update, so return early.
	}
	currentUpdateTime = 0.0f; // Reset the timer for the next update.

	//Get the position of the mouse and use that to get the isometric coordinate it is hovering over..
	Vector2Int mousePos = InputHandler::GetMousePosition();
	Vector2Int mouseIsoPos = Application::Instance()->GetRenderer().GetIsometricGridPosFromScreenCoords(mousePos, true);
	/*CorneliusEngine::Log(("IsoPos(" + std::to_string(mouseIsoPos.x) + ", " + std::to_string(mouseIsoPos.y) + ")") +
						 (" MousePos(" + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y) + ")") +
						 (" CamOffset(" + std::to_string(Application::Instance()->GetRenderer().GetCameraOffset().x) + ", " + std::to_string(Application::Instance()->GetRenderer().GetCameraOffset().y) + ")"));*/


						 //Check mouse button states.
	bool leftButtonPressed = InputHandler::LeftMouseButtonPressed();
	bool rightButtonPressed = InputHandler::RightMouseButtonPressed();
	ChessPiece& pieceAtHoverPos = GetPieceAtPosition(mouseIsoPos.x, mouseIsoPos.y);
	bool isValidPosition = m_gameGrid.GetEnvironmentTilemap()->IsValidPosition(mouseIsoPos);

	//Find which position is being selected by the player. If the left mouse button is pressed, select the position. If the right mouse button is pressed, clear the selections.
	if (leftButtonPressed && isValidPosition) {
		if (selectedPosOne == UNSELECTED_VALUE && (pieceAtHoverPos.GetPieceType() != ChessPiece::PieceType::NO_TYPE)) {
			CorneliusEngine::Log("Selected Piece on position: (" + std::to_string(mouseIsoPos.x) + ", " + std::to_string(mouseIsoPos.y) + ")");
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
			ChessPiece& piece = GetPieceAtPosition(x, y);
			if (piece.GetPieceType() != ChessPiece::PieceType::NO_TYPE) {
				piece.SetIsHovered(false);
			}
		}
	}

	//Tint the position the mouse is hovering over a dark grey if no position is currently selected.
	if (selectedPosOne == UNSELECTED_VALUE && selectedPosTwo == UNSELECTED_VALUE) {
		//Reset the colours of the grid.
		m_gameGrid.ResetGridColours();
	}
	else if (selectedPosOne != UNSELECTED_VALUE) {
		m_gameGrid.ResetGridColours();
		// If the first position is selected, and the second one is not,
		// tint the piece on the selected tile green, and then tint any tiles that are valid moves green as well to indicate the valid moves for this piece.
		// Then, tint any tiles that are invalid moves red to indicate they are invalid moves for this piece.

		//Get the piece at the selected position & all it's valid moves.
		ChessPiece& selectedPiece = GetPieceAtPosition(selectedPosOne.x, selectedPosOne.y);
		const std::vector<Vector2Int>& validMoves = selectedPiece.GetAllValidPieceMoves();

		//Loop over all the tiles on the board and tint them based on whether they are a valid move for this piece or not.
		Colour redTint = Colour(255, 0, 0);
		Colour greenTint = Colour(0, 255, 0);

		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				Vector2Int currentPos = Vector2Int(x, y);
				int tileIndex = x + y * m_gameGrid.GetEnvironmentTilemap()->GetWidth();
				//Check if this position is in the valid moves vector for the selected piece.
				bool isValidMove = false;
				for (int i = 0; i < validMoves.size(); i++) {
					if (validMoves[i] == currentPos) {
						isValidMove = true;
						break;
					}
				}
				Colour originalColour = m_gameGrid.GetEnvironmentTilemap()->GetTilemapList()[tileIndex].tint;
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
				m_gameGrid.GetEnvironmentTilemap()->GetTilemapList()[tileIndex].tint = newColour;
			}
		}
	}

	//Tint the hovered tile a darker colour.
	//Get the tile index of the position the mouse is hovering over, so we can tint it if needed.
	mouseIsoPos = Application::Instance()->GetRenderer().GetIsometricGridPosFromScreenCoords(mousePos, true);
	int hoverTileIndex = mouseIsoPos.x + mouseIsoPos.y * m_gameGrid.GetEnvironmentTilemap()->GetWidth();

	isValidPosition = m_gameGrid.GetEnvironmentTilemap()->IsValidPosition(mouseIsoPos);
	if (isValidPosition) {
		Colour originalColour = m_gameGrid.GetEnvironmentTilemap()->GetTilemapList()[hoverTileIndex].tint;
		Colour hoverColour = Colour(originalColour.r / 3, originalColour.g / 3, originalColour.b / 3);
		m_gameGrid.GetEnvironmentTilemap()->GetTilemapList()[hoverTileIndex].tint = hoverColour;

		//Make the chess piece at this position (if there is one) also be tinted to indicate it is being hovered over.
		if (pieceAtHoverPos.GetPieceType() != ChessPiece::PieceType::NO_TYPE) {
			pieceAtHoverPos.SetIsHovered(true);
		}
	}
}