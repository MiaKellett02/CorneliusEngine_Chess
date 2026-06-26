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
			Vector2Int possibleMoveBlack = Vector2Int(m_piecePosition.x, (m_piecePosition.y + (i + 1)));
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
	//Board bounds for reference.
	int boardMin = 0;
	int boardMax = 7;

	//Check positive y direction for 7 tiles, until we hit a piece, or until we are out of bounds.
	for (int y = m_piecePosition.y + 1; y < (boardMax + 1); y++) {
		//Check if there is a piece in this position.
		ChessPiece& pieceInWay = m_chessBoard->GetPieceAtPosition(m_piecePosition.x, y);
		if (pieceInWay.GetPieceType() == PieceType::NO_TYPE) {
			//There is no piece in the way, this is a valid move.
			validMoves.push_back(Vector2Int(m_piecePosition.x, y));
			continue;
		}

		//There is a piece in the way, check if it is an enemy piece that can be captured.
		if (pieceInWay.GetPieceColour() != PieceColour::NO_COLOUR) {
			if (pieceInWay.GetPieceColour() != GetPieceColour() && pieceInWay.GetPieceType() != PieceType::KING) {
				//There is an enemy piece in the way that is not the king, this is a valid capture move.
				validMoves.push_back(Vector2Int(m_piecePosition.x, y));
			}
			else {
				//Not an enemy piece, so this is not a valid move.
			}
			break; // Stop checking in this direction after hitting a piece, as the rook cannot jump over pieces.
		}
	}

	//Check negative y direction for 7 tiles, until we hit a piece, or until we are out of bounds.
	for (int y = m_piecePosition.y - 1; y > (boardMin - 1); y--) {
		//Check if there is a piece in this position.
		ChessPiece& pieceInWay = m_chessBoard->GetPieceAtPosition(m_piecePosition.x, y);
		if (pieceInWay.GetPieceType() == PieceType::NO_TYPE) {
			//There is no piece in the way, this is a valid move.
			validMoves.push_back(Vector2Int(m_piecePosition.x, y));
			continue;
		}

		//There is a piece in the way, check if it is an enemy piece that can be captured.
		if (pieceInWay.GetPieceColour() != PieceColour::NO_COLOUR) {
			if (pieceInWay.GetPieceColour() != GetPieceColour() && pieceInWay.GetPieceType() != PieceType::KING) {
				//There is an enemy piece in the way that is not the king, this is a valid capture move.
				validMoves.push_back(Vector2Int(m_piecePosition.x, y));
			}
			else {
				//Not an enemy piece, so this is not a valid move.
			}
			break; // Stop checking in this direction after hitting a piece, as the rook cannot jump over pieces.
		}
	}

	//Check positive x direction for 7 tiles, until we hit a piece, or until we are out of bounds.
	for (int x = m_piecePosition.x + 1; x < (boardMax + 1); x++) {
		//Check if there is a piece in this position.
		ChessPiece& pieceInWay = m_chessBoard->GetPieceAtPosition(x, m_piecePosition.y);
		if (pieceInWay.GetPieceType() == PieceType::NO_TYPE) {
			//There is no piece in the way, this is a valid move.
			validMoves.push_back(Vector2Int(x, m_piecePosition.y));
			continue;
		}

		//There is a piece in the way, check if it is an enemy piece that can be captured.
		if (pieceInWay.GetPieceColour() != PieceColour::NO_COLOUR) {
			if (pieceInWay.GetPieceColour() != GetPieceColour() && pieceInWay.GetPieceType() != PieceType::KING) {
				//There is an enemy piece in the way that is not the king, this is a valid capture move.
				validMoves.push_back(Vector2Int(x, m_piecePosition.y));
			}
			else {
				//Not an enemy piece, so this is not a valid move.
			}
			break; // Stop checking in this direction after hitting a piece, as the rook cannot jump over pieces.
		}
	}

	//Check negative x direction for 7 tiles, until we hit a piece, or until we are out of bounds.
	for (int x = m_piecePosition.x - 1; x > (boardMin - 1); x--) {
		//Check if there is a piece in this position.
		ChessPiece& pieceInWay = m_chessBoard->GetPieceAtPosition(x, m_piecePosition.y);
		if (pieceInWay.GetPieceType() == PieceType::NO_TYPE) {
			//There is no piece in the way, this is a valid move.
			validMoves.push_back(Vector2Int(x, m_piecePosition.y));
			continue;
		}
		//There is a piece in the way, check if it is an enemy piece that can be captured.
		if (pieceInWay.GetPieceColour() != PieceColour::NO_COLOUR) {
			if (pieceInWay.GetPieceColour() != GetPieceColour() && pieceInWay.GetPieceType() != PieceType::KING) {
				//There is an enemy piece in the way that is not the king, this is a valid capture move.
				validMoves.push_back(Vector2Int(x, m_piecePosition.y));
			}
			else {
				//Not an enemy piece, so this is not a valid move.
			}
			break; // Stop checking in this direction after hitting a piece, as the rook cannot jump over pieces.
		}
	}
}

void ChessPiece::CalculateValidMovesForKnight(std::vector<Vector2Int>& validMoves)
{
	//Check positive y, left and right.
	int forwardsY = m_piecePosition.y + 2;
	if (forwardsY < 8) {
		//Check left.
		int leftX = m_piecePosition.x - 1;
		if (leftX > -1) {
			ChessPiece& pieceOnLeft = m_chessBoard->GetPieceAtPosition(leftX, forwardsY);
			if (pieceOnLeft.GetPieceColour() != GetPieceColour() && pieceOnLeft.GetPieceType() != PieceType::KING) {
				//There is no piece on the left, or there is an enemy piece that is not the king, this is a valid move.
				validMoves.push_back(Vector2Int(leftX, forwardsY));
			}
		}
		//Check right.
		int rightX = m_piecePosition.x + 1;
		if (rightX < 8) {
			ChessPiece& pieceOnRight = m_chessBoard->GetPieceAtPosition(rightX, forwardsY);
			if (pieceOnRight.GetPieceColour() != GetPieceColour() && pieceOnRight.GetPieceType() != PieceType::KING) {
				//There is no piece on the right, or there is an enemy piece that is not the king, this is a valid move.
				validMoves.push_back(Vector2Int(rightX, forwardsY));
			}
		}
	}

	//Check negative y, left and right.
	int backwardsY = m_piecePosition.y - 2;
	if (backwardsY > -1) {
		//Check left.
		int leftX = m_piecePosition.x - 1;
		if (leftX > -1) {
			ChessPiece& pieceOnLeft = m_chessBoard->GetPieceAtPosition(leftX, backwardsY);
			if (pieceOnLeft.GetPieceColour() != GetPieceColour() && pieceOnLeft.GetPieceType() != PieceType::KING) {
				//There is no piece on the left, or there is an enemy piece that is not the king, this is a valid move.
				validMoves.push_back(Vector2Int(leftX, backwardsY));
			}
		}
		//Check right.
		int rightX = m_piecePosition.x + 1;
		if (rightX < 8) {
			ChessPiece& pieceOnRight = m_chessBoard->GetPieceAtPosition(rightX, backwardsY);
			if (pieceOnRight.GetPieceColour() != GetPieceColour() && pieceOnRight.GetPieceType() != PieceType::KING) {
				//There is no piece on the right, or there is an enemy piece that is not the king, this is a valid move.
				validMoves.push_back(Vector2Int(rightX, backwardsY));
			}
		}
	}

	//Check positive x, up and down.
	int rightX = m_piecePosition.x + 2;
	if (rightX < 8) {
		//Check up.
		int upY = m_piecePosition.y - 1;
		if (upY > -1) {
			ChessPiece& pieceUp = m_chessBoard->GetPieceAtPosition(rightX, upY);
			if (pieceUp.GetPieceColour() != GetPieceColour() && pieceUp.GetPieceType() != PieceType::KING) {
				//There is no piece up, or there is an enemy piece that is not the king, this is a valid move.
				validMoves.push_back(Vector2Int(rightX, upY));
			}
		}
		//Check down.
		int downY = m_piecePosition.y + 1;
		if (downY < 8) {
			ChessPiece& pieceDown = m_chessBoard->GetPieceAtPosition(rightX, downY);
			if (pieceDown.GetPieceColour() != GetPieceColour() && pieceDown.GetPieceType() != PieceType::KING) {
				//There is no piece down, or there is an enemy piece that is not the king, this is a valid move.
				validMoves.push_back(Vector2Int(rightX, downY));
			}
		}
	}

	//Check negative x, up and down.
	int leftX = m_piecePosition.x - 2;
	if (leftX > -1) {
		//Check up.
		int upY = m_piecePosition.y - 1;
		if (upY > -1) {
			ChessPiece& pieceUp = m_chessBoard->GetPieceAtPosition(leftX, upY);
			if (pieceUp.GetPieceColour() != GetPieceColour() && pieceUp.GetPieceType() != PieceType::KING) {
				//There is no piece up, or there is an enemy piece that is not the king, this is a valid move.
				validMoves.push_back(Vector2Int(leftX, upY));
			}
		}
		//Check down.
		int downY = m_piecePosition.y + 1;
		if (downY < 8) {
			ChessPiece& pieceDown = m_chessBoard->GetPieceAtPosition(leftX, downY);
			if (pieceDown.GetPieceColour() != GetPieceColour() && pieceDown.GetPieceType() != PieceType::KING) {
				//There is no piece down, or there is an enemy piece that is not the king, this is a valid move.
				validMoves.push_back(Vector2Int(leftX, downY));
			}
		}
	}
}

void ChessPiece::CalculateValidMovesForBishop(std::vector<Vector2Int>& validMoves)
{
	//Do up-right diagonal up to 7 squares.
	for (int i = 0; i < 7; i++) {
		//Calculate the position of this square along the diagonal and check bounds.
		Vector2Int possibleMove = Vector2Int(m_piecePosition.x + (i + 1), m_piecePosition.y - (i + 1));
		if (possibleMove.x < 0 || possibleMove.x > 7 || possibleMove.y < 0 || possibleMove.y > 7) {
			continue; // Out of bounds, skip this square.
		}

		//In bounds, check if there is a piece in this position.
		ChessPiece& pieceInWay = m_chessBoard->GetPieceAtPosition(possibleMove.x, possibleMove.y);
		if (pieceInWay.GetPieceType() == PieceType::NO_TYPE) {
			//There is no piece in the way, this is a valid move.
			validMoves.push_back(possibleMove);
			continue;
		}

		//There is a piece in the way, check if it is an enemy piece that can be captured.
		if (pieceInWay.GetPieceColour() != PieceColour::NO_COLOUR) {
			if (pieceInWay.GetPieceColour() != GetPieceColour() && pieceInWay.GetPieceType() != PieceType::KING) {
				//There is an enemy piece in the way that is not the king, this is a valid capture move.
				validMoves.push_back(possibleMove);
			}
			else {
				//Not an enemy piece, so this is not a valid move.
			}
			break; // Stop checking in this direction after hitting a piece, as the bishop cannot jump over pieces.
		}
	}

	//Do the up-left diagonal up to 7 squares.
	for (int i = 0; i < 7; i++) {
		//Calculate the position of this square along the diagonal and check bounds.
		Vector2Int possibleMove = Vector2Int(m_piecePosition.x - (i + 1), m_piecePosition.y - (i + 1));
		if (possibleMove.x < 0 || possibleMove.x > 7 || possibleMove.y < 0 || possibleMove.y > 7) {
			continue; // Out of bounds, skip this square.
		}
		//In bounds, check if there is a piece in this position.
		ChessPiece& pieceInWay = m_chessBoard->GetPieceAtPosition(possibleMove.x, possibleMove.y);
		if (pieceInWay.GetPieceType() == PieceType::NO_TYPE) {
			//There is no piece in the way, this is a valid move.
			validMoves.push_back(possibleMove);
			continue;
		}
		//There is a piece in the way, check if it is an enemy piece that can be captured.
		if (pieceInWay.GetPieceColour() != PieceColour::NO_COLOUR) {
			if (pieceInWay.GetPieceColour() != GetPieceColour() && pieceInWay.GetPieceType() != PieceType::KING) {
				//There is an enemy piece in the way that is not the king, this is a valid capture move.
				validMoves.push_back(possibleMove);
			}
			else {
				//Not an enemy piece, so this is not a valid move.
			}
			break; // Stop checking in this direction after hitting a piece, as the bishop cannot jump over pieces.
		}
	}

	//Do the down-right diagonal up to 7 squares.
	for (int i = 0; i < 7; i++) {
		//Calculate the position of this square along the diagonal and check bounds.
		Vector2Int possibleMove = Vector2Int(m_piecePosition.x + (i + 1), m_piecePosition.y + (i + 1));
		if (possibleMove.x < 0 || possibleMove.x > 7 || possibleMove.y < 0 || possibleMove.y > 7) {
			continue; // Out of bounds, skip this square.
		}
		//In bounds, check if there is a piece in this position.
		ChessPiece& pieceInWay = m_chessBoard->GetPieceAtPosition(possibleMove.x, possibleMove.y);
		if (pieceInWay.GetPieceType() == PieceType::NO_TYPE) {
			//There is no piece in the way, this is a valid move.
			validMoves.push_back(possibleMove);
			continue;
		}
		//There is a piece in the way, check if it is an enemy piece that can be captured.
		if (pieceInWay.GetPieceColour() != PieceColour::NO_COLOUR) {
			if (pieceInWay.GetPieceColour() != GetPieceColour() && pieceInWay.GetPieceType() != PieceType::KING) {
				//There is an enemy piece in the way that is not the king, this is a valid capture move.
				validMoves.push_back(possibleMove);
			}
			else {
				//Not an enemy piece, so this is not a valid move.
			}
			break; // Stop checking in this direction after hitting a piece, as the bishop cannot jump over pieces.
		}
	}

	//Do the down-left diagonal up to 7 squares.
	for (int i = 0; i < 7; i++) {
		//Calculate the position of this square along the diagonal and check bounds.
		Vector2Int possibleMove = Vector2Int(m_piecePosition.x - (i + 1), m_piecePosition.y + (i + 1));
		if (possibleMove.x < 0 || possibleMove.x > 7 || possibleMove.y < 0 || possibleMove.y > 7) {
			continue; // Out of bounds, skip this square.
		}
		//In bounds, check if there is a piece in this position.
		ChessPiece& pieceInWay = m_chessBoard->GetPieceAtPosition(possibleMove.x, possibleMove.y);
		if (pieceInWay.GetPieceType() == PieceType::NO_TYPE) {
			//There is no piece in the way, this is a valid move.
			validMoves.push_back(possibleMove);
			continue;
		}
		//There is a piece in the way, check if it is an enemy piece that can be captured.
		if (pieceInWay.GetPieceColour() != PieceColour::NO_COLOUR) {
			if (pieceInWay.GetPieceColour() != GetPieceColour() && pieceInWay.GetPieceType() != PieceType::KING) {
				//There is an enemy piece in the way that is not the king, this is a valid capture move.
				validMoves.push_back(possibleMove);
			}
			else {
				//Not an enemy piece, so this is not a valid move.
			}
			break; // Stop checking in this direction after hitting a piece, as the bishop cannot jump over pieces.
		}
	}
}

void ChessPiece::CalculateValidMovesForQueen(std::vector<Vector2Int>& validMoves)
{
	//Queen is just a mix of rook and bishop moves, so we can just call the functions for those pieces to calculate the valid moves for the queen.
	CalculateValidMovesForRook(validMoves);
	CalculateValidMovesForBishop(validMoves);
}

void ChessPiece::CalculateValidMovesForKing(std::vector<Vector2Int>& validMoves)
{
}
