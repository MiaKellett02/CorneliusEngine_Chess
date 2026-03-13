//Project includes.
#include "ChessBoard.h"
#include "Scene.h"
#include "Application.h"
#include "Entity.h"
#include "ChessPieceRendererComponent.h"

void ChessBoard::Initialize() {
	// Initialize the chess board with the starting positions of the pieces.
	int boardSizeX = 8;
	int boardSizeY = 8;
	m_boardState.clear(); // Create the 2D vector to represent the chess board.
	m_boardState.reserve(boardSizeX * boardSizeY);
	for (int x = 0; x < boardSizeX; x++) {
		for (int y = 0; y < boardSizeY; y++) {
			//Default to no piece.
			m_boardState.push_back(ChessPiece(ChessPiece::PieceColour::NO_COLOUR, ChessPiece::PieceType::NO_TYPE));
		}
	}

	//Create entities that map to the pieces on the board and add them to the scene for rendering.
	for (int y = 0; y < boardSizeY; y++) {
		for (int x = 0; x < boardSizeX; x++) {
			//For debugging, add a pawn in each standard position for the pieces on the board..
			if (y == 0 || y == 1) {
				//Spawn a black pawn for the first two rows.
				m_boardState.at(x + (y * boardSizeX)).UpdatePiece(ChessPiece::PieceColour::BLACK, ChessPiece::PieceType::PAWN);
			}
			else if (y == boardSizeY - 1 || y == boardSizeY - 2) {
				//Spawn a white pawn for the last two rows.
				m_boardState.at(x + (y * boardSizeX)).UpdatePiece(ChessPiece::PieceColour::WHITE, ChessPiece::PieceType::PAWN);
			}

			//Spawn an entity for this piece and add it to the scene.
			// Example: Adding a new entity to the active scene
			Scene* scene = Application::Instance()->GetActiveScene();

			//Size is set to (-1, -1) as the renderer will use the isometric tile size for rendering, so the entity's size does not need to be set.
			Entity* newEntity = new Entity(nullptr, Vector2(x, y), Vector2(-1,-1), "ChessPieceRenderer", "Player", "resources/WhitePawn.png");
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

ChessPiece& ChessBoard::GetPieceAtPosition(int x, int y) {
    // Check if m_boardState is valid and board size is 8x8
    constexpr int boardSizeX = 8;
    constexpr int boardSizeY = 8;
    if (x < 0 || x > 7 || y < 0 || y > 7) {
        // Out of bounds access, return a reference to a static empty piece.
        static ChessPiece emptyPiece(ChessPiece::PieceColour::NO_COLOUR, ChessPiece::PieceType::NO_TYPE);
        return emptyPiece;
    }
    return m_boardState.at(x + (y * boardSizeX));
}

void ChessBoard::UpdatePieceAtPosition()
{

}