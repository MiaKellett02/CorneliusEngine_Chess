#pragma once

//Project includes.
#include "Scene.h"
#include "Application.h"
#include "IsometricTilemap.h"

//Library includes.
#include <string>
#include <iostream>

//Class definition.
class MainMenuScene : public Scene {
public:
	//Constructor and destructor.
	MainMenuScene(std::string a_sceneName, const std::string& a_defaultTileSprite)
		: Scene(a_sceneName)
	{
		//Ensure the default tile sprite is loaded into the renderer & cached for when we setup the background tilemap.
		Application::Instance()->GetRenderer().CreateTexture(a_defaultTileSprite, a_defaultTileSprite);
		m_defaultTileSprite = a_defaultTileSprite;
	}

	~MainMenuScene();

	// Inherited via Scene
	void SetupScene() override;
	void UpdateScene(double a_deltaTime) override;
	void ShutdownScene() override;

private:
	//Private variables.
	bool m_firstFrame = true; // A flag to check if it's the first frame of the scene.
	bool m_prevMousePressed = false; // Tracks the previous mouse button state to detect a fresh click.
	IsometricTilemap m_backgroundTilemap; // Background tilemap displayed on the main menu.

	std::string m_defaultTileSprite = "";

	//Private functions.
};
