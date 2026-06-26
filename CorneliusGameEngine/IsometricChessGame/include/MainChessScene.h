#pragma once

//Project includes.
#include "Scene.h"
#include "Application.h"

//Library includes.
#include <string>
#include <iostream>

//Forward Declarations.
class ChessBoard;

//Class defintion.
class MainChessScene : public Scene {
public:
	//Constructor and destructor.
	MainChessScene(std::string a_sceneName, const std::string& a_defaultTileSprite) 
		: Scene(a_sceneName)
	{
		//Ensure the default tile sprite is loaded into the renderer & is cached for when we setup the chessboard.
		Application::Instance()->GetRenderer().CreateTexture(a_defaultTileSprite, a_defaultTileSprite);
		m_defaultTileSprite = a_defaultTileSprite;

		//Ensure the chess piece textures are loaded into the renderer.
		//Load the white pieces.
		Application::Instance()->GetRenderer().CreateTexture("resources/WhitePawn.png", "resources/WhitePawn.png");
		Application::Instance()->GetRenderer().CreateTexture("resources/WhiteRook.png", "resources/WhiteRook.png");
		Application::Instance()->GetRenderer().CreateTexture("resources/WhiteKnight.png", "resources/WhiteKnight.png");
		Application::Instance()->GetRenderer().CreateTexture("resources/WhiteBishop.png", "resources/WhiteBishop.png");
		Application::Instance()->GetRenderer().CreateTexture("resources/WhiteQueen.png", "resources/WhiteQueen.png");
		Application::Instance()->GetRenderer().CreateTexture("resources/WhiteKing.png", "resources/WhiteKing.png");

		//Load the black pieces.
		Application::Instance()->GetRenderer().CreateTexture("resources/BlackPawn.png", "resources/BlackPawn.png");
		Application::Instance()->GetRenderer().CreateTexture("resources/BlackRook.png", "resources/BlackRook.png");
		Application::Instance()->GetRenderer().CreateTexture("resources/BlackKnight.png", "resources/BlackKnight.png");
		Application::Instance()->GetRenderer().CreateTexture("resources/BlackBishop.png", "resources/BlackBishop.png");
		Application::Instance()->GetRenderer().CreateTexture("resources/BlackQueen.png", "resources/BlackQueen.png");
		Application::Instance()->GetRenderer().CreateTexture("resources/BlackKing.png", "resources/BlackKing.png");
	}

	~MainChessScene();

	// Inherited via Scene
	void SetupScene() override;
	void UpdateScene(double a_deltaTime) override;
	void ShutdownScene() override;


private:
	//Private variabes.
	bool m_firstFrame = true; // A flag to check if it's the first frame of the scene. This can be used to perform any setup that needs to happen after the scene is loaded, but before the first update.
	ChessBoard* m_chessBoard = nullptr;

	std::string m_defaultTileSprite = "";

	//Private functions.
};