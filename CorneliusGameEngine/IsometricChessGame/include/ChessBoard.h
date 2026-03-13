////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: ChessBoard.h
// Created by: Mia Kellett
// Purpose: The purpose of this class is to represent the chess board, ensuring it's pieces are rendered and handling it's logic.
// Date Created: 12/03/2026
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
//Project includes.
#include "ChessPiece.h"

//Library includes.
#include <vector>

//Forward declarations.

//Class definition.
class ChessBoard {
public:
	//Constructor and destructor.
	ChessBoard() {
		Initialize();
	}
	~ChessBoard();

	//Public functions.
	ChessPiece& GetPieceAtPosition(int x, int y);

	void UpdatePieceAtPosition();

private:
	//private variables.
	std::vector<ChessPiece> m_boardState; // 2D vector to represent the chess board. Each int can represent a different piece type and color.

	//private functions.
	void Initialize();
};