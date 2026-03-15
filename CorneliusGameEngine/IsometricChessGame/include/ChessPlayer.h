///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: ChessPlayer.h
// Author: Mia Kellett
// Date Created: 15/03/2026
// Purpose: The chess player represents a player in the chess game, whether human or AI. It contains information about the player's colour, 
//			and any other relevant information about the player that may be needed for the game logic or UI.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
//Project includes.
#include "ChessPiece.h"
#include "GameGrid.h"
#include "ChessPlayer.h"

//Library includes.
#include <vector>

//Forward declarations.
class ChessBoard;

//Class definition.
class ChessPlayer {
public:
	//Constructors and destructors.
	ChessPlayer();
	~ChessPlayer();

	//Copy constructor.
	ChessPlayer(const ChessPlayer& a_otherPlayer) {
		m_isHumanPlayer = a_otherPlayer.m_isHumanPlayer;
		m_isBlackPlayer = a_otherPlayer.m_isBlackPlayer;
		selectedPosOne = a_otherPlayer.selectedPosOne;
		selectedPosTwo = a_otherPlayer.selectedPosTwo;
	}

	//Public Functions.
	void SetIsHumanPlayer(bool a_isHumanPlayer) { m_isHumanPlayer = a_isHumanPlayer; }
	bool GetIsHumanPlayer() const { return m_isHumanPlayer; }

	void SetIsBlackPlayer(bool a_isBlackPlayer) { m_isBlackPlayer = a_isBlackPlayer; }
	bool GetIsBlackPlayer() const { return m_isBlackPlayer; }

	void Update(double a_deltaTime, ChessBoard* a_mainChessBoard);

private:
	//Private Variables.
	bool m_isHumanPlayer; // Whether this player is a human player or an AI player.
	bool m_isBlackPlayer; // Whether this player is the black player or the white player. True for black, false for white.

	// Some variables to store the selected positions for pathfinding. If the position is (-1, -1), it is not selected.
	const Vector2Int UNSELECTED_VALUE = Vector2Int(-1, -1);
	Vector2Int selectedPosOne = UNSELECTED_VALUE;
	Vector2Int selectedPosTwo = UNSELECTED_VALUE;
	float buttonPressCooldown = 0.2f; // Cooldown time in seconds to prevent multiple button presses from being registered too quickly.
	float currentCooldownTime = 0.0f; // Timer to track the cooldown time for button presses.

	//Private Functions.
	void HandleHumanPlayerInput(double a_deltaTime, ChessBoard* a_mainChessBoard);
};