//Project includes.
#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Vector2.h"
#include "Logging.h"

std::vector<Vector2Int>& ChessPiece::GetAllValidPieceMoves()
{
	static std::vector<Vector2Int> validMoves; // Static vector to store the valid moves for this piece. This is static to avoid having to create a new vector every time this function is called, which can be expensive in terms of performance. The vector will be cleared and repopulated with the valid moves for this piece each time this function is called.
	validMoves.clear(); // Clear the vector before populating it with the valid moves for this piece.   

	switch (m_pieceType) {
	case PieceType::PAWN: {
		// TODO: Add pawn move logic
		CalculateValidMovesForPawn(validMoves);
		break;
	}
	case PieceType::ROOK: {
		// TODO: Add rook move logic
		CalculateValidMovesForRook(validMoves);
		break;
	}
	case PieceType::KNIGHT: {
		// TODO: Add knight move logic
		CalculateValidMovesForKnight(validMoves);
		break;
	}
	case PieceType::BISHOP: {
		// TODO: Add bishop move logic
		CalculateValidMovesForBishop(validMoves);
		break;
	}
	case PieceType::QUEEN: {
		// TODO: Add queen move logic
		CalculateValidMovesForQueen(validMoves);
		break;
	}
	case PieceType::KING: {
		// TODO: Add king move logic
		CalculateValidMovesForKing(validMoves);
		break;
	}
	default: {
		CorneliusEngine::LogError("Invalid piece type for calculating valid moves.");
		break;
	}
	}


	//Return the result.
	return validMoves;
}

void ChessPiece::CalculateValidMovesForPawn(std::vector<Vector2Int>& validMoves)
{
	// Valid pawn moves are 2 forward if it hasn't moved yet, or 1 forward if it has moved, 
	// and 1 diagonally forward if there is an enemy piece there. 
	// The direction of forward is determined by the colour of the pawn (white pawns move up the board, black pawns move down the board).
	bool isBlack = (m_pieceColour == PieceColour::BLACK);
	int forwardDirection = isBlack ? 1 : -1; // Black pawns move down the board (positive y direction), white pawns move up the board (negative y direction).

	//Figure out the starting row for this pawn based on its colour, as this will determine whether it can move 2 forward or not.
	int startingY;
	if (isBlack) {
		startingY = 1; // Black pawns start on the second row (y = 1).
	}
	else {
		startingY = 6; // White pawns start on the seventh row (y = 6).
	}

	bool canMoveTwoForward = (m_piecePosition.y == startingY); // A pawn can move 2 forward if it is on its starting row.
	int movementAmmount = canMoveTwoForward ? 2 : 1; // A pawn can move 2 forward if it is on its starting row, otherwise it can only move 1 forward.
	for (int i = 0; i < movementAmmount; i++) {
		if (isBlack) {
			Vector2Int possibleMoveBlack = Vector2Int(m_piecePosition.x, (m_piecePosition.y + (i + 1)) ); 
			ChessPiece& pieceInWayBlack = m_chessBoard->GetPieceAtPosition(possibleMoveBlack.x, possibleMoveBlack.y);
			if (pieceInWayBlack.GetPieceType() == PieceType::NO_TYPE) {
				//No piece in the way, this is a valid move.
				validMoves.push_back(possibleMoveBlack);
			}
		}
		else {
			//Is white.
			Vector2Int possibleMoveWhite = Vector2Int(m_piecePosition.x, (m_piecePosition.y - (i + 1)));
			ChessPiece& pieceInWayWhite = m_chessBoard->GetPieceAtPosition(possibleMoveWhite.x, possibleMoveWhite.y);
			if (pieceInWayWhite.GetPieceType() == PieceType::NO_TYPE) {
				//No piece in the way, this is a valid move.
				validMoves.push_back(possibleMoveWhite);
			}
		}
	}

	//Calculate diagonal captures.
	//Check the two diagonally forward positions for an enemy piece that can be captured.
	Vector2Int diagonalLeft = Vector2Int(m_piecePosition.x - 1, m_piecePosition.y + forwardDirection);
	ChessPiece& pieceOnDiagonalLeft = m_chessBoard->GetPieceAtPosition(diagonalLeft.x, diagonalLeft.y);	
	if (pieceOnDiagonalLeft.GetPieceColour() != PieceColour::NO_COLOUR && pieceOnDiagonalLeft.GetPieceColour() != GetPieceColour() && pieceOnDiagonalLeft.GetPieceType() != PieceType::KING) {
		//If an enemy piece that is not the king is on the diagonal, this is a valid capture move.
		validMoves.push_back(diagonalLeft);
	}
	
	Vector2Int diagonalRight = Vector2Int(m_piecePosition.x + 1, m_piecePosition.y + forwardDirection);
	ChessPiece& pieceOnDiagonalRight = m_chessBoard->GetPieceAtPosition(diagonalRight.x, diagonalRight.y);
	if (pieceOnDiagonalRight.GetPieceColour() != PieceColour::NO_COLOUR && pieceOnDiagonalRight.GetPieceColour() != GetPieceColour() && pieceOnDiagonalRight.GetPieceType() != PieceType::KING) {
		//If an enemy piece that is not the king is on the diagonal, this is a valid capture move.
		validMoves.push_back(diagonalRight);
	}
}

void ChessPiece::CalculateValidMovesForRook(std::vector<Vector2Int>& validMoves)
{
}

void ChessPiece::CalculateValidMovesForKnight(std::vector<Vector2Int>& validMoves)
{
}

void ChessPiece::CalculateValidMovesForBishop(std::vector<Vector2Int>& validMoves)
{
}

void ChessPiece::CalculateValidMovesForQueen(std::vector<Vector2Int>& validMoves)
{
}

void ChessPiece::CalculateValidMovesForKing(std::vector<Vector2Int>& validMoves)
{
}
