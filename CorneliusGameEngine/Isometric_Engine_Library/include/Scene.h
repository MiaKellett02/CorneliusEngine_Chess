#pragma once

//Project includes.
#include "IsometricTilemap.h"

//Library includes.
#include <string>
#include <vector>

//Forward declarations.
class Entity;

//Class definition.
class Scene {
public:
	//Constructor and destructor.
	Scene(std::string a_sceneName) {
		m_sceneName = a_sceneName;

		m_sceneTilemaps = std::vector<IsometricTilemap*>(); // Ensure the tilemap array is instantiated.
	};

	~Scene() {};

	//Standard Scene Functions.
	virtual void SetupScene();
	virtual void UpdateScene(double a_deltaTime);
	virtual void ShutdownScene();

	//Tilemap handling functions.
	std::vector<IsometricTilemap*>& GetSceneTileMaps() { return m_sceneTilemaps; }
	void AddNewTilemapToSceneRenderList(IsometricTilemap* a_tilemap) { m_sceneTilemaps.push_back(a_tilemap); }
	void RemoveTilemapByName(const std::string& a_tilemapName);

	//Entity handling functions.
	Entity* GetEntityByName(const std::string& a_name);
	std::vector<Entity*> GetEntitiesWithTag(const std::string& a_tag);
	std::vector<Entity*>& GetAllEntities() { 
		return m_gameEntities;
	}
	void DestroyEntity(Entity* a_entityToDestroy);
	void DestroyAllEntities();

	//Getters
	std::string GetSceneName() { return m_sceneName; }

protected:
	//Private/class inherited variables.
	std::string m_sceneName;
	bool m_sceneShutdown = false;

	//Scene variables.
	std::vector<IsometricTilemap*> m_sceneTilemaps; // Give a scene the ability to render multiple tilemaps.
	// Items in the vector are pointers as this tilemap should only point to 
	// tilemaps owned by other components of the application. Each entry
	// in this vector is not owned by this vector.

//Game entity variables.
	std::vector<Entity*> m_gameEntities;

	//Private Functions.
	void ShuffleEntities();
};