//Project includes.
#include "Entity.h"
#include "Renderer.h"
#include "Component.h"
#include "Application.h"

//Library includes
#include <iostream>


Entity::Entity(Entity* a_parent, Vector2 a_pos, Vector2 a_size, std::string a_name, std::string a_tag, std::string a_texFilepath)
{
	//Populate entity variables.
	m_parent = a_parent;
	if (a_parent != nullptr) {
		m_parent->AddChild(this);
		//Since this entity has a parent ensure it's pos is relative to parent.
		Vector2 parentPos = a_parent->GetPosition();
		Vector2 worldParentedPos = parentPos + a_pos;
		m_position = worldParentedPos;
	}
	else {
		m_position = a_pos;
	}
	m_size = a_size;
	m_name = a_name;
	m_tag = a_tag;
	m_texID = a_texFilepath;

	//Calculate the screen pos.
	m_screenPos.x = m_position.x - (m_size.x / 2);
	m_screenPos.y = m_position.y - (m_size.y / 2);

	//Ensure entity texture is created using entity tag rather than name.
	Application::Instance()->GetRenderer().CreateTexture(m_texID, a_texFilepath);
}

Entity::~Entity()
{
	//Destroy all components.
	for (int i = 0; i < m_components.size(); i++) {
		Component* componentToDestroy = m_components[i];
		delete componentToDestroy;
	}

	//Destroy all children.
	for (int i = 0; i < m_children.size(); i++) {
		Entity* childToDestroy = m_children[i];

		//Get the application class to destroy the entity to handle ensuring it is removed correctly.
		Application::Instance()->GetActiveScene()->DestroyEntity(childToDestroy);
	}
}

void Entity::Update(double a_deltaTime)
{
	//Update all of this entities components.
	for (int i = 0; i < m_components.size(); i++) {
		m_components[i]->Update(a_deltaTime);
	}
}

void Entity::ChangeTexture(std::string a_newTextureFilepath)
{
	m_texID = a_newTextureFilepath;

	//Ensure entity texture is created using entity tag rather than name.
	Application::Instance()->GetRenderer().CreateTexture(m_texID, a_newTextureFilepath);
}

void Entity::SetPosition(Vector2 a_newPos)
{
	//Bounds check.
	//if (Application::Instance()->IsPositionOutOfBound(a_newPos)) {
	//	//std::cout << m_name << " tried to move to a position is out of bounds." << std::endl;
	//	return; //Don't update position as it is out of bounds.
	//}

	//Update the position.
	if (m_parent != nullptr) {
		//Since this entity has a parent ensure it's pos is relative to parent.
		Vector2 parentPos = m_parent->GetPosition();
		m_position.x = parentPos.x + a_newPos.x;
		m_position.y = parentPos.y + a_newPos.y;
	}
	else {
		//Set the position.
		m_position = a_newPos;
	}

	//Tell any children to update their positions.
	UpdateChildPositions();

	//Update the screen pos.
	m_screenPos.x = m_position.x - (m_size.x / 2);
	m_screenPos.y = m_position.y - (m_size.y / 2);
}

void Entity::ChangePosition(Vector2 a_moveVector)
{
	//Move the entity in the direction passed.
	Vector2 newPos = Vector2(m_position);
	newPos.x += a_moveVector.x;
	newPos.y += a_moveVector.y;

	SetPosition(newPos);
}

void Entity::SetSize(Vector2 a_newSize)
{
	//Set the new size.
	m_size = a_newSize;

	//Update the screen pos
	m_screenPos.x = m_position.x - (m_size.x / 2);
	m_screenPos.y = m_position.y - (m_size.y / 2);
}

Component* Entity::GetComponentByID(std::string a_ID)
{
	for (int i = 0; i < m_components.size(); i++) {
		Component* checkComponent = m_components[i];
		if (checkComponent->GetComponentID() == a_ID) {
			return checkComponent;
		}
	}


	//If nothing was found return nullptr.
	return nullptr;
}

void Entity::ParentNewPosition(Vector2 a_newParentPos)
{
	//Just get this entity to update it's own position to where it currently is.
	//Set pos function handles moving relative to parent.
	//This function is just to tell all children to move.
	SetPosition(m_position);
}

void Entity::UpdateChildPositions()
{
	for (int i = 0; i < m_children.size(); i++) {
		//Tell each child object to update their positions.
		m_children[i]->ParentNewPosition(m_position);
	}
}
