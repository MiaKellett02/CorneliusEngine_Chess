////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: ChessPiece.h
// Created by: Mia Kellett
// Purpose: The purpose of this class is to represent a chess piece, ensuring it's rendered and handling it's logic.
// Date Created: 12/03/2026
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

//Project includes.
#include "Vector2.h"
#include "Logging.h"

//Library includes.
#include <vector>

//Forward declarations.
class ChessBoard;

//Class definition.
class ChessPiece
{
public:
	//Chess Utility.
	static enum PieceType {
		NO_TYPE = -1,
		PAWN,
		ROOK,
		KNIGHT,
		BISHOP,
		QUEEN,
		KING,
	};
	static enum PieceColour {
		NO_COLOUR = -1,
		WHITE,
		BLACK,
	};

	//Constructor and destructor.
	ChessPiece(PieceColour a_pieceColour, PieceType a_pieceType, ChessBoard* a_chessBoard, Vector2Int a_piecePosition) {
		//Default to no piece.
		UpdatePiece(a_pieceColour, a_pieceType);
		m_chessBoard = a_chessBoard;
		m_piecePosition = Vector2Int(a_piecePosition.x, a_piecePosition.y);
	}

	//Public functions.
	void UpdatePiece(PieceColour a_pieceColour, PieceType a_pieceType) {
		m_pieceColour = a_pieceColour;
		m_pieceType = a_pieceType;
	}

	//Getters.
	PieceColour& GetPieceColour() { return m_pieceColour; }
	PieceType& GetPieceType() { return m_pieceType; }
	bool& GetIsHovered() { return m_isHovered; }

	std::vector<Vector2Int>& GetAllValidPieceMoves();

	//Setters.
	void SetPieceColour(PieceColour a_pieceColour) { m_pieceColour = a_pieceColour; }
	void SetPieceType(PieceType a_pieceType) { m_pieceType = a_pieceType; }
	void SetIsHovered(bool a_isHovered) { m_isHovered = a_isHovered; }
private:
	//Private variables.
	PieceColour m_pieceColour; // The colour of the piece (white or black).
	PieceType m_pieceType; // The type of the piece (pawn, rook, knight, bishop, queen, king).
	bool m_isHovered = false; // Whether the piece is currently being hovered over by the mouse cursor, used for rendering purposes.

	Vector2Int m_piecePosition; // The position of the piece on the chess board, used for movement and interaction logic.

	ChessBoard* m_chessBoard = nullptr; // A pointer to the chess board that this piece is on, used for accessing the game grid and other pieces on the board for movement and interaction logic.

	//Private functions.
	void CalculateValidMovesForPawn(std::vector<Vector2Int>& validMoves);
	void CalculateValidMovesForRook(std::vector<Vector2Int>& validMoves);
	void CalculateValidMovesForKnight(std::vector<Vector2Int>& validMoves);
	void CalculateValidMovesForBishop(std::vector<Vector2Int>& validMoves);
	void CalculateValidMovesForQueen(std::vector<Vector2Int>& validMoves);
	void CalculateValidMovesForKing(std::vector<Vector2Int>& validMoves);
};