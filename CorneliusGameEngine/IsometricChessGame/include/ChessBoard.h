////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: ChessBoard.h
// Created by: Mia Kellett
// Purpose: The purpose of this class is to represent the chess board, ensuring it's pieces are rendered and handling it's logic.
// Date Created: 12/03/2026
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
//Project includes.
#include "ChessPiece.h"
#include "GameGrid.h"

//Library includes.
#include <vector>

//Forward declarations.

//Class definition.
class ChessBoard {
public:
	//Constructor and destructor.
	ChessBoard(const std::string& a_defaultTileSprite) {
		Initialize(a_defaultTileSprite);
	}
	~ChessBoard();

	//Public functions.
	ChessPiece& GetPieceAtPosition(int x, int y);
	GameGrid& GetGameGrid() { return m_gameGrid; }

	void UpdateChessBoard(double a_deltaTime);

private:
	// private variables.
	std::vector<ChessPiece> m_boardState; // 2D vector to represent the chess board. Each position on the game grid has a ChessPiece associated with it in this vector.
	GameGrid m_gameGrid; // This is the grid that handles rendering the isometric tiles for the chess board. 

	// Some variables to store the selected positions for pathfinding. If the position is (-1, -1), it is not selected.
	const Vector2Int UNSELECTED_VALUE = Vector2Int(-1, -1);
	Vector2Int selectedPosOne = UNSELECTED_VALUE;
	Vector2Int selectedPosTwo = UNSELECTED_VALUE;

	// Variables to handle timer for updating the chess board. This is used to ensure the chess board is updated at a consistent rate, rather than every frame.
	const float UPDATE_INTERVAL = 0.016f; // Update every 16 milliseconds (60 updates per second).
	float currentUpdateTime = 0.0f; // A timer to track the time since the last update of the chess board.

	//private functions.
	void Initialize(const std::string& a_defaultTileSprite);
};