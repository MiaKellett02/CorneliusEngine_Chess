//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: ChessPieceRendererComponent.h
// Created by: Mia Kellett
// Purpose: The purpose of this class is to represent a chess piece, ensuring it's rendered and handling it's logic.
// Date Created: 12/03/2026
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
//Project includes.
#include "Component.h"

//Library includes.

//Forward declarations.
class ChessBoard;

//Class definition.
class ChessPieceRendererComponent : public Component
{
public:
	//Constructor and destructor.
	ChessPieceRendererComponent(Entity* a_ownerEntity, ChessBoard* a_chessBoard, float a_updateTimeLimit) : Component(a_ownerEntity) {
		m_chessBoard = a_chessBoard;
		m_updateTimeLimitMax = a_updateTimeLimit;
	};
	~ChessPieceRendererComponent() {};

	// Inherited via Component
	void Update(double a_deltaTime) override;
	std::string GetComponentID() const override {
		return "ChessPieceRendererComponent";
	}

private:
	ChessBoard* m_chessBoard; // A pointer to the chess board, so this component can access the pieces on the board and render them.
	float m_updateTimeLimitMax; // The maximum time limit for updating the piece's position. This is to prevent the piece from updating every frame, which can cause performance issues.
	float m_updateTimeLimitCurrent = 0; // The current time limit for updating the piece's position. This is incremented by the delta time each frame, and when it reaches the maximum time limit, the piece's position is updated and this is reset to 0.
};