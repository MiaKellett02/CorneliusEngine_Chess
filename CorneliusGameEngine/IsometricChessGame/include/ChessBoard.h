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
#include "ChessPlayer.h"

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

	//Copy constructor.
	ChessBoard(const ChessBoard& a_otherChessBoard)
		: m_gameGrid(a_otherChessBoard.m_gameGrid),
		m_boardState(a_otherChessBoard.m_boardState),
		m_whitePlayer(a_otherChessBoard.m_whitePlayer),
		m_blackPlayer(a_otherChessBoard.m_blackPlayer),
		m_isWhitePlayersTurn(a_otherChessBoard.m_isWhitePlayersTurn)
	{
		// No assignment needed, all members are initialized directly
	}

	~ChessBoard();

	//Public functions.
	ChessPiece& GetPieceAtPosition(int x, int y);
	GameGrid& GetGameGrid() { return m_gameGrid; }

	ChessBoard GetGameBoardState(const ChessBoard& a_mainBoard); // Will return a copy of the chess board state, which can be used for things like pathfinding and AI logic without affecting the actual game state.
	bool MakeGameMove(Vector2Int a_startPos, Vector2Int a_endPos); // Will move a piece from the start position to the end position, if the move is valid. This will also handle any logic related to making a move, such as capturing pieces, checking for check/checkmate, and updating the game state accordingly.
																   // Returns true if the move was successful, false if the move was invalid.

	bool IsWhitePlayersTurn() const { return m_isWhitePlayersTurn; } // Returns whether it is currently the white player's turn. If false, it is the black player's turn.

	void UpdateChessBoard(double a_deltaTime);

private:
	// private variables.
	std::vector<ChessPiece> m_boardState; // 2D vector to represent the chess board. Each position on the game grid has a ChessPiece associated with it in this vector.
	GameGrid m_gameGrid; // This is the grid that handles rendering the isometric tiles for the chess board. 

	//Chess player variables.
	ChessPlayer m_whitePlayer; // The white player in the chess game.
	ChessPlayer m_blackPlayer; // The black player in the chess game.

	//Game State variables.
	bool m_isWhitePlayersTurn = true; // Whether it is currently the white player's turn. If false, it is the black player's turn.

	// Variables to handle timer for updating the chess board. This is used to ensure the chess board is updated at a consistent rate, rather than every frame.
	const float UPDATE_INTERVAL = 0.016f; // Update every 16 milliseconds (60 updates per second).
	float currentUpdateTime = 0.0f; // A timer to track the time since the last update of the chess board.

	//private functions.
	void Initialize(const std::string& a_defaultTileSprite);
};