#pragma once
//Project includes.
#include "Vector2.h"
#include "Colour.h"

//Library includes.
#include <string>
#include <vector>

//Forward declarations
class Renderer;
class Component;

class Entity {
public:
	//Constructor and destructor.
	Entity(Entity* a_parent, Vector2 a_pos, Vector2 a_size, std::string a_name, std::string a_tag, std::string a_texFilepath);
	~Entity();

	//Main Entity functions.
	void Update(double a_deltaTime);
	void ChangeTexture(std::string a_newTextureFilepath);

	//Getters and setters.
	std::string GetName() { return m_name; }
	void SetName(std::string a_name) { m_name = a_name; }
	std::string GetTag() { return m_tag; }
	std::string GetTexID() { return m_texID; }

	const Vector2& GetPosition() { return m_position; }
	void SetPosition(Vector2 a_newPos);
	void ChangePosition(Vector2 a_moveVector);
	Vector2 GetSize() { return m_size; }
	void SetSize(Vector2 a_newSize);
	Vector2 GetScreenPos() { return m_screenPos; }

	void AddComponent(Component* a_newComponent) { m_components.push_back(a_newComponent); }
	Component* GetComponentByID(std::string a_ID);

	void AddChild(Entity* a_child) { m_children.push_back(a_child); }

	void EnableRendering() { m_renderingEnabled = true; }
	void DisableRendering() { m_renderingEnabled = false; }
	const bool& IsRenderingEnabled() { return m_renderingEnabled; }
	
	void EnableIsometricRendering() { m_isometricRenderingEnabled = true; }
	void DisableIsometricRendering() { m_isometricRenderingEnabled = false; }
	const bool& IsIsometricRenderingEnabled() { return m_isometricRenderingEnabled; }

	Colour& GetEntityTint() { return m_entityTint; }
	void SetEntityTint(Colour a_newTint) { m_entityTint = a_newTint; }
private:
	//Entity components.
	std::vector<Component*> m_components;
	Entity* m_parent;
	std::vector<Entity*> m_children;
	bool m_renderingEnabled = true; // Rendering enabled by default.
	bool m_isometricRenderingEnabled = false; // Isometric rendering disabled by default.

	//Entity descriptive variables.
	std::string m_name;
	std::string m_tag;
	std::string m_texID;
	Colour m_entityTint = Colour(255,255,255);

	//Entity world info.
	Vector2 m_position;
	Vector2 m_size;

	//Screen position.
	Vector2 m_screenPos;

	//Private functions.
	void ParentNewPosition(Vector2 a_newParentPos);
	void UpdateChildPositions();
};