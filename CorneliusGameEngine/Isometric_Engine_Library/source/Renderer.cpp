//Project includes.
#include "Renderer.h"
#include "Entity.h"
#include "Logging.h"
#include "IsometricTilemap.h"
#include "Application.h"

//Library includes
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <assert.h>

//Functions.
Renderer::Renderer() : m_cameraOffset(0, 0), m_renderer(nullptr), m_window(nullptr)
{

}

Renderer::~Renderer()
{

}

Vector2Int Renderer::Initialise(const std::string& a_appName, int a_screenWidth, int a_screenHeight, bool fullScreen, bool a_runAtMonitorResolution)
{
	int desiredWidth = a_screenWidth;
	int desiredHeight = a_screenHeight;

	//Setup SDL.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
		CorneliusEngine::LogError("Unable to initialize SDL: " + std::string(SDL_GetError()));
		return Vector2Int(-1, -1);
	}

	SDL_DisplayMode displayMode;
	if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
		std::cerr << "Failed to get display mode: " << SDL_GetError() << std::endl; SDL_Quit();
		CorneliusEngine::LogError("Unable to get display mode: " + std::string(SDL_GetError()));
		return Vector2Int(-1, -1);
	}

	//Setup normal renderer variables.
	if (a_runAtMonitorResolution) {
		desiredWidth = displayMode.w;
		desiredHeight = displayMode.h;
		CorneliusEngine::Log("Screen dimensions (from renderer init): (" + std::to_string(desiredWidth) + "," + std::to_string(desiredHeight) + ")");
	}
	int tileMult = 6;
	m_isometricTileSize = Vector2Int(desiredHeight / tileMult, desiredHeight / tileMult);

	//Initialise image handling library.
	if (IMG_Init(IMG_INIT_PNG) == 0) {
		CorneliusEngine::LogError("Unable to initialise SDL2_image.");
		return Vector2Int(-1, -1);
	}

	//Create the window.
	if (fullScreen) {
		m_window = SDL_CreateWindow(a_appName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, desiredWidth, desiredHeight, SDL_WINDOW_FULLSCREEN);
	}
	else {
		if (a_runAtMonitorResolution) {
			m_window = SDL_CreateWindow(a_appName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, desiredWidth, desiredHeight, SDL_WINDOW_BORDERLESS);
		}
		else {
			m_window = SDL_CreateWindow(a_appName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, desiredWidth, desiredHeight, SDL_WINDOW_OPENGL);
		}
	}

	//Ensure the window was correctly initialised.
	if (m_window == NULL) {
		CorneliusEngine::LogError("Unable to initialise program window.");
		return Vector2Int(-1, -1);
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_renderer == NULL) {
		CorneliusEngine::LogError("Unable to initialise renderer.");
		return Vector2Int(-1, -1);
	}

	//If we get here then the renderer has been initialised successfully.
	CorneliusEngine::Log("SDL has been initialised.");


	//Return the screen dimensions to update the application.
	return Vector2Int(desiredWidth, desiredHeight);
}

void Renderer::ClearScreen()
{
	//Clear the screen.
	SDL_RenderClear(m_renderer);
}

void Renderer::Render(const std::vector<Entity*>& a_entitiesToRender)
{
	//Ensure all entities are copied to the renderer.
	for (int i = 0; i < a_entitiesToRender.size(); i++) {
		Entity* entity = a_entitiesToRender[i]; //Get the entity.

		//Construct the rect for it to be rendered in.
		SDL_Rect renderRect;
		renderRect.x = (int)entity->GetScreenPos().x + m_cameraOffset.x; //pos x
		renderRect.y = (int)entity->GetScreenPos().y + m_cameraOffset.y; //pos y
		renderRect.w = (int)entity->GetSize().x; //width
		renderRect.h = (int)entity->GetSize().y; //height

		//Get the texture.
		SDL_Texture* entityTexture = m_textureMap[entity->GetTexID()];

		//Render the entity.
		SDL_RenderCopy(m_renderer, entityTexture, NULL, &renderRect);
	}

	//Draw to the screen.
	SDL_RenderPresent(m_renderer);
}

void Renderer::RenderTilemap(IsometricTilemap* a_tilemapToRender)
{
	//Setup variables.
	using TileType = IsometricTilemap::TileType;
	using Tile = IsometricTilemap::Tile;
	const std::vector<Tile>& tilesToRender = a_tilemapToRender->GetTilemapList();
	const int WIDTH = a_tilemapToRender->GetWidth();
	const int HEIGHT = a_tilemapToRender->GetHeight();
	const int AREA = WIDTH * HEIGHT;

	//Render.
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			//Get the Tile data.
			int tileAccessIndex = x + y * WIDTH;
			if (tileAccessIndex < 0 || tileAccessIndex >= AREA) {
				CorneliusEngine::LogError("Tile access index is out of bounds on tilemap (inside renderer.cpp 'RenderTilemap').");
			}

			Tile currentTile = tilesToRender[tileAccessIndex];
			TileType& currentTileType = a_tilemapToRender->GetTileDataFromIndex(currentTile.tileTypeIndex);
			const std::string& tileTextureID = currentTileType.sprite;
			SDL_Texture* tileTexture = m_textureMap[tileTextureID];

			//Tint the tile to be the correct colour.
			Uint8 r = currentTile.tint.r;
			Uint8 g = currentTile.tint.g;
			Uint8 b = currentTile.tint.b;
			SDL_SetTextureColorMod(tileTexture, r, g, b);

			//Get the tile information.
			Vector2Int tileScreenPos = GetScreenPosFromIsometricCoords(Vector2Int(x, y)) + m_cameraOffset;
			SDL_Rect renderRect;
			renderRect.x = tileScreenPos.x;
			renderRect.y = tileScreenPos.y;
			renderRect.w = m_isometricTileSize.x;
			renderRect.h = m_isometricTileSize.y;

			//Render the tile.
			SDL_RenderCopy(m_renderer, tileTexture, NULL, &renderRect);
		}
	}
}

void Renderer::Shutdown()
{
	CorneliusEngine::Log("Shutting down renderer.");
	//Destroy all textures in the map.
	for (auto& texturePair : m_textureMap) {
		SDL_Texture* texture = texturePair.second;
		SDL_DestroyTexture(texture);
	}

	//Destroy the SDL components for the renderer..
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	IMG_Quit();
}

void Renderer::CreateTexture(std::string a_textureID, std::string a_filepath)
{
	//Check if the texture already exists in the map.
	if (m_textureMap.count(a_textureID) > 0) {
		//std::cout << a_textureID << " has already been initialised." << std::endl;
		return; //Early out as we don't need to recreate the texture.
	}

	//Load the image..
	SDL_Surface* imgSurface = IMG_Load(a_filepath.c_str());
	if (imgSurface == NULL) {
		CorneliusEngine::LogError("Unable to load image: " + std::string(IMG_GetError()));
		return;
	}

	//Create the texture.
	SDL_Texture* imgTexture = SDL_CreateTextureFromSurface(m_renderer, imgSurface);
	if (imgTexture == NULL) {
		CorneliusEngine::LogError("Unable to create texture.");
		return;
	}

	//Add it to the map.
	m_textureMap[a_textureID] = imgTexture;
	CorneliusEngine::Log("Loading texture: " + a_textureID);

	//Free the surface memory as it is no longer needed.
	SDL_FreeSurface(imgSurface);
}

Vector2Int Renderer::GetScreenPosFromIsometricCoords(const Vector2Int& a_isometricCoordinate)
{
	// Extract the tile size
	const int tileWidth = (m_isometricTileSize.x / 2);
	const int tileHeight = (m_isometricTileSize.y / 2);

	// Calculate screen X and Y coordinates
	Application* instance = Application::Instance();
	int screenX = (a_isometricCoordinate.x * tileWidth) + (a_isometricCoordinate.y * -1 * tileWidth);
	screenX -= tileWidth;
	int screenY = (a_isometricCoordinate.x * 0.5f * tileHeight) + (a_isometricCoordinate.y * 0.5f * tileHeight);

	// Apply camera offset
	screenX += m_cameraOffset.x;
	screenY += m_cameraOffset.y;

	return Vector2Int((int)screenX, (int)screenY);
}

Vector2Int Renderer::GetIsometricGridPosFromScreenCoords(const Vector2Int& a_screenCoords, bool a_useCamOffset)
{
	// Extract the tile size 
	const int tileWidth = m_isometricTileSize.x;
	const int halfWidth = tileWidth / 2;

	const int tileHeight = m_isometricTileSize.y;
	const int halfHeight = tileHeight / 2;

	// Remove camera offset 
	int screenX = a_screenCoords.x;
	if (a_useCamOffset) {
		screenX -= m_cameraOffset.x * 2;
	}
	screenX += halfWidth / 2;

	int screenY = a_screenCoords.y;
	if (a_useCamOffset) {
		screenY -= m_cameraOffset.y * 2;
	}
	screenY -= tileHeight;

	// Calculate isometric coordinates 
	int isoX = (screenX / halfWidth + 2 * screenY / halfHeight) / 2;
	isoX += 1;
	int isoY = (2 * screenY / halfHeight - screenX / halfWidth) / 2;
	isoY += 1;

	return Vector2Int(isoX, isoY);
}

int Renderer::GetMonitorRefreshRate()
{
	SDL_DisplayMode displayMode;
	if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
	{
		CorneliusEngine::LogError("Failed to get display mode: " + std::string(SDL_GetError()));
		return 0;
	}
	int refreshRate = displayMode.refresh_rate;
	return refreshRate;
}
