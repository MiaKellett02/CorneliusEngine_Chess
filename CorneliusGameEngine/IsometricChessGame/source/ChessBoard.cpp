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

ChessBoard ChessBoard::GetGameBoardState(const ChessBoard& a_mainBoard)
{
	return ChessBoard(a_mainBoard);
}

bool ChessBoard::MakeGameMove(Vector2Int a_startPos, Vector2Int a_endPos)
{
	//Ensure the positions are in bounds of the board.
	if (a_startPos.x < 0 || a_startPos.x > 7 || a_startPos.y < 0 || a_startPos.y > 7 ||
		a_endPos.x < 0 || a_endPos.x > 7 || a_endPos.y < 0 || a_endPos.y > 7) {
		CorneliusEngine::Log("Invalid move: Start or end position is out of bounds.");
		return false;
	}

	//Get the piece at the start position and the piece at the end position.
	ChessPiece& pieceToMove = GetPieceAtPosition(a_startPos.x, a_startPos.y);
	ChessPiece& targetPiece = GetPieceAtPosition(a_endPos.x, a_endPos.y);

	//Check if there is a piece at the start position, and that the piece belongs to the current player.
	if (pieceToMove.GetPieceType() == ChessPiece::PieceType::NO_TYPE) {
		CorneliusEngine::Log("Invalid move: No piece at the start position.");
		return false;
	}

	ChessPiece::PieceColour currentPlayerColour = m_isWhitePlayersTurn ? ChessPiece::PieceColour::WHITE : ChessPiece::PieceColour::BLACK;
	if (pieceToMove.GetPieceColour() != currentPlayerColour) {
		CorneliusEngine::Log("Invalid move: The piece at the start position does not belong to the current player.");
		return false;
	}

	//Check if the move is valid for the piece at the start position.
	std::vector<Vector2Int> validMoves = pieceToMove.GetAllValidPieceMoves();
	bool isValidMove = false;
	for (const Vector2Int& validMove : validMoves) {
		if (validMove == a_endPos) {
			isValidMove = true;
			break;
		}
	}
	if (!isValidMove) {
		CorneliusEngine::Log("Invalid move: The piece at the start position cannot move to the end position.");
		return false;
	}

	//Move the piece to the end position by updating the piece at the end position to be the same as the piece at the start position, and then updating the piece at the start position to be an empty tile.
	targetPiece.UpdatePiece(pieceToMove.GetPieceColour(), pieceToMove.GetPieceType());
	pieceToMove.UpdatePiece(ChessPiece::PieceColour::NO_COLOUR, ChessPiece::PieceType::NO_TYPE);

	//After making the move, switch the turn to the other player.
	m_isWhitePlayersTurn = !m_isWhitePlayersTurn;
	return true; // Move was successful.
}

void ChessBoard::UpdateChessBoard(double a_deltaTime)
{
	if (currentUpdateTime < UPDATE_INTERVAL) {
		currentUpdateTime += static_cast<float>(a_deltaTime);
		//Ensure players are setup.
		m_whitePlayer.SetIsBlackPlayer(false);
		m_whitePlayer.SetIsHumanPlayer(true); // Default to white as human player for testing.

		m_blackPlayer.SetIsBlackPlayer(true);
		m_blackPlayer.SetIsHumanPlayer(true); // have black player be Human for testing before AI logic is implemented.

		return; // Not enough time has passed since the last update, so return early.
	}
	currentUpdateTime = 0.0f; // Reset the timer for the next update.

	//Update the players, which will handle input and any AI logic for making moves.
	if (m_isWhitePlayersTurn) {
		m_whitePlayer.Update(UPDATE_INTERVAL, this);
	}
	else {
		m_blackPlayer.Update(UPDATE_INTERVAL, this);
	}
}