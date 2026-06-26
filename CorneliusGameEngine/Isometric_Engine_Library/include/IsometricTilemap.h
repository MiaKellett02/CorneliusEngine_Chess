#pragma once

//Library includes.
#include <vector>
#include <array>
#include <string>

//Project includes.
#include "Renderer.h"
#include "Colour.h"

//Class defintion.
class IsometricTilemap {
public:
	//Structs
	struct TileType {
		std::string sprite;//Id of the sprite in the Renderer resource map.
		std::string name; //Name of the tile to enable mapping grid positions to each tile etc...

		TileType(std::string a_name, std::string a_spriteID) {
			sprite = a_spriteID;
			name = a_name;
		}
	};

	struct Tile {
		int tileTypeIndex;
		bool isVisible;
		Colour tint;

		Tile(int a_tileTypeIndex, bool a_visible, Colour col) {
			tileTypeIndex = a_tileTypeIndex;
			isVisible = a_visible;
			tint = col;
		}
	};

	//Constructor and destructor.
	IsometricTilemap() { };

	IsometricTilemap(const std::string& a_tilemapName, int a_width, int a_height, int a_tilesToReserve = 10) {
		m_width = a_width;
		m_height = a_height;
		m_tilemapName = a_tilemapName;

		//Reserve space for some tiles in the tile vector.
		m_tileTypes.reserve(a_tilesToReserve);

		//Construct the tilemap and default to the first tile available.
		Tile newTile = Tile(0, true, Colour(255, 255, 255));
		m_tileMap = std::vector<Tile>(a_width * a_height, newTile);
	}

	//Functions.
	void AddNewTileType(const std::string& a_tileName, const std::string& a_tileSpriteLocation, Renderer& a_renderer) {
		TileType newTile = TileType(a_tileName, a_tileSpriteLocation);
		m_tileTypes.push_back(newTile);
		a_renderer.CreateTexture(a_tileSpriteLocation, a_tileSpriteLocation);
	}

	TileType& GetTileDataFromIndex(int a_index) {
		return m_tileTypes[a_index];
	}

	bool IsValidPosition(Vector2Int& a_checkPosition) {
		if (a_checkPosition.GetX() >= 0 &&
			a_checkPosition.GetX() < GetWidth() &&
			a_checkPosition.GetY() >= 0 &&
			a_checkPosition.GetY() < GetHeight()) {
			return true;
		}

		return false;
	}

	Tile& GetTile(Vector2Int a_pos) {
		int tileAccessIndex = a_pos.GetX() + a_pos.GetY() * this->GetWidth();
		return this->GetTilemapList()[tileAccessIndex];
	}

	int GetWidth() {
		return m_width;
	}

	int GetHeight() {
		return m_height;
	}

	const std::string& GetTilemapName() {
		return m_tilemapName;
	}

	std::vector<Tile>& GetTilemapList() {
		return m_tileMap;
	}

private:
	std::vector<Tile> m_tileMap;
	std::vector<TileType> m_tileTypes;

	int m_width;
	int m_height;

	std::string m_tilemapName;
};