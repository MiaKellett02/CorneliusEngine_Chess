#pragma once

//Forward Declarations.
class SDL_Window;
class SDL_Renderer;
class SDL_Surface;
class SDL_Texture;
class Entity;
class IsometricTilemap;

//Library includes.
#include <vector>
#include <string>
#include <map>

//Project includes.
#include "Vector2.h"

class Renderer {
public:
	//Constructor and destructor.
	Renderer();
	~Renderer();

	//Main renderer Functions.
	Vector2Int Initialise(const std::string& a_appName, int a_screenWidth, int a_screenHeight, bool fullScreen, bool a_runAtMonitorResolution);
	void ClearScreen();
	void Render(const std::vector<Entity*>& a_entitiesToRender);
	void RenderTilemap(IsometricTilemap* a_tilemapToRender);
	void Shutdown();

	//Functions to handle multiple textures.
	void CreateTexture(std::string a_textureID, std::string a_filepath);

	//Camera handling functions.
	void SetCameraOffset(Vector2Int& a_newPos) {
		m_cameraOffset = a_newPos;
		//m_cameraOffset = Vector2Int(0, 0);
	}
	void MoveCamera(Vector2Int& a_moveAmount) {
		m_cameraOffset = m_cameraOffset + a_moveAmount;
		//m_cameraOffset = Vector2Int(0, 0);
	}

	//Renderer Utility Functions
	Vector2Int GetScreenPosFromIsometricCoords(const Vector2Int& a_isometricCoordinate);
	Vector2Int GetIsometricCoordsFromScreenPos(const Vector2Int& a_screenPos);
	Vector2Int GetIsometricGridPosFromScreenCoords(const Vector2Int& a_screenCoords, bool a_useCamOffset);
	Vector2Int GetCameraOffset() { return m_cameraOffset; }
	int GetMonitorRefreshRate();

private:
	//Private Variables.
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	Vector2Int m_cameraOffset;
	Vector2Int m_isometricTileSize;

	//Texture variables.
	std::map<std::string, SDL_Texture*> m_textureMap;
};