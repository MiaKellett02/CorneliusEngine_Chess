#include "ChessPieceRendererComponent.h"
#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Logging.h"
#include "Entity.h"
#include "Colour.h"

void ChessPieceRendererComponent::Update(double a_deltaTime)
{
	//If the current time is less than or equal to 0, run the timer and skip the logic.
	if (m_updateTimeLimitCurrent > 0) {
		m_updateTimeLimitCurrent -= a_deltaTime;
		return;
	}

	//Reset the timer.
	m_updateTimeLimitCurrent = m_updateTimeLimitMax;

	//Do the logic.
	//Get the entity this component is attached to.
	Entity* ownerEntity = GetOwnerEntity();
	// If the entity is null, log an error and early out.
	if (ownerEntity == nullptr) {
		CorneliusEngine::LogError("ChessPieceRendererComponent is not attached to an entity.");
		return;
	}

	//If isometric rendering is not enabled for the entity, log a warning and enable it.
	if (!ownerEntity->IsIsometricRenderingEnabled()) {
		CorneliusEngine::LogWarning("ChessPieceRendererComponent is attached to an entity that does not have isometric rendering enabled. Enabling it now.");
		ownerEntity->EnableIsometricRendering();
	}

	//Ensure rendering is enabled for the entity, otherwise the piece will not be visible.
	if (!ownerEntity->IsRenderingEnabled()) {
		ownerEntity->EnableRendering();
	}

	//Check what the current piece is and what it's type is, and set the entity's texture accordingly.
	ChessPiece& entityPiece = m_chessBoard->GetPieceAtPosition(ownerEntity->GetPosition().x, ownerEntity->GetPosition().y);

	switch (entityPiece.GetPieceType()) {
	case ChessPiece::PieceType::NO_TYPE: {
		ownerEntity->DisableRendering(); // If there is no piece, disable rendering for this entity.
		break;
	}
	case ChessPiece::PieceType::PAWN: {
		if (entityPiece.GetPieceColour() == ChessPiece::PieceColour::WHITE) {
			ownerEntity->ChangeTexture("resources/WhitePawn.png");
		}
		else if (entityPiece.GetPieceColour() == ChessPiece::PieceColour::BLACK) {
			ownerEntity->ChangeTexture("resources/BlackPawn.png");
		}

		if (entityPiece.GetIsHovered()) {
			ownerEntity->SetEntityTint(Colour(175, 175, 175)); // If the piece is being hovered over, tint it a light grey to indicate this.
		}
		else {
			ownerEntity->SetEntityTint(Colour(255, 255, 255)); // If the piece is not being hovered over, ensure it is not tinted.
		}


		ownerEntity->EnableRendering();
		break;
	}
	default: {
		// For other piece types, we can add more cases here to set the appropriate texture based on the piece type and colour.
		CorneliusEngine::LogWarning("ChessPieceRendererComponent does not have a case for this piece type. No texture will be set for this piece.");
		break;
	}
	}
}