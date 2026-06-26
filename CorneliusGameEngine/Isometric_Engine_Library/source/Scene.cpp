//Project includes.
#include "Scene.h"
#include "Entity.h"
#include "Logging.h"
#include "Application.h"

//Library includes.
#include <iostream>
#include <random>
#include <algorithm>
#include <assert.h>

//Function definitions.
void Scene::SetupScene()
{
	m_sceneShutdown = false;
	assert(m_gameEntities.size() == 0);
}

void Scene::UpdateScene(double a_deltaTime)
{
	assert(!m_sceneShutdown);
	if (m_sceneShutdown) {
		CorneliusEngine::LogError("SCENE IS SHUTDOWN SO SHOULD NOT BE UPDATING.");
	}
	for (int i = 0; i < m_gameEntities.size(); i++) {
		Entity* currentEntity = m_gameEntities[i];
		currentEntity->Update(a_deltaTime);
	}
}

void Scene::ShutdownScene()
{
	//destroy all entities in the scene.
	DestroyAllEntities();
	m_sceneShutdown = true;
}

void Scene::RemoveTilemapByName(const std::string& a_tilemapName) 
{ 
	m_sceneTilemaps.erase(
		std::remove_if(m_sceneTilemaps.begin(), m_sceneTilemaps.end(), 
			[&a_tilemapName](IsometricTilemap* tilemap) 
			{ 
				return tilemap->GetTilemapName() == a_tilemapName; 
			}), 
		m_sceneTilemaps.end());
}

Entity* Scene::GetEntityByName(const std::string& a_name)
{
	for (int i = 0; i < m_gameEntities.size(); i++) {
		if (m_gameEntities[i]->GetName() == a_name) {
			return m_gameEntities[i];
		}
	}

	//No entity matched.
	CorneliusEngine::LogWarning("No entity was found of name: " + a_name);
	return nullptr;
}

std::vector<Entity*> Scene::GetEntitiesWithTag(const std::string& a_tag)
{
	std::vector<Entity*> entitiesWithTag;
	for (int i = 0; i < m_gameEntities.size(); i++) {
		if (m_gameEntities[i]->GetTag() == a_tag) {
			entitiesWithTag.push_back(m_gameEntities[i]);
		}
	}

	return entitiesWithTag;
}

void Scene::DestroyEntity(Entity* a_entityToDestroy)
{
	//Ensure entity is valid.
	if (a_entityToDestroy == nullptr) {
		return; //Early out.
	}

	//Remove the entity from the list of current game entities.
	auto newEnd = std::remove(m_gameEntities.begin(), m_gameEntities.end(), a_entityToDestroy);
	m_gameEntities.erase(newEnd, m_gameEntities.end());

	//Free it's memory
	//std::cout << "Destroying entity: " << a_entityToDestroy->GetName() << std::endl;
	delete a_entityToDestroy;
}

void Scene::DestroyAllEntities()
{
	assert(!m_sceneShutdown);
	//Destroy all entities untill there is nothing left.
	while (m_gameEntities.size() > 0) {
		DestroyEntity(m_gameEntities[0]);
	}
}

void Scene::ShuffleEntities()
{
	//Create a random device and seed it.
	std::random_device rng;
	std::mt19937 randFunc(rng());

	//Shuffle the entities vector.
	std::shuffle(m_gameEntities.begin(), m_gameEntities.end(), randFunc);
}