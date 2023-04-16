#include "GameObject_Collidable.h"
#include "VectorMath.h"
#include <glm/gtc/matrix_transform.hpp> 
#include <iostream>
#include "GameData.h"

namespace game
{
	GameObjectCollidable::GameObjectCollidable(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float collisionRadius)
	: GameObject(geometry_Ptr, shader_Ptr, texture, gameData_Ptr, position, rotation)
	{
		this->collisionRadius = collisionRadius;
		this->collisionWhiteList_ptr = new std::set<GameObjectCollidable*>();
	}

	void GameObjectCollidable::Update(double delta_time, GLFWwindow* p_glfwWindow)
	{
		CheckForCollisions();

		GameObject::Update(delta_time, p_glfwWindow);
	}


	void GameObjectCollidable::CheckForCollisions()
	{
		if (gameData_Ptr == NULL) 
		{ 
			std::cout << "gameData_Ptr is NULL, can not check for collisions" << std::endl; 
			return; 
		}

		
		for (int i = 0; i < gameData_Ptr->GetCollidableGameObjectsPtr()->size(); i++)
		{
			GameObjectCollidable* otherCollidableGameObject_Ptr = gameData_Ptr->GetCollidableGameObjectsPtr()->at(i);

			if (this->GetAddress() != otherCollidableGameObject_Ptr->GetAddress())
			{
				if (this->CollidingWith(otherCollidableGameObject_Ptr) == true)
				{
					otherCollidableGameObject_Ptr->OnCollision();
					this->OnCollision();
				}
			}
		}		
	}

	bool GameObjectCollidable::CollidingWith(GameObjectCollidable* otherGameObjectCollidable_Ptr)
	{
		//returns false if otherGameObjectCollidable_Ptr is in this's collisionWhiteList
		if(collisionWhiteList_ptr->find(otherGameObjectCollidable_Ptr) != collisionWhiteList_ptr->end()){ return false; }

		float distance = glm::length(this->GetPosition() - otherGameObjectCollidable_Ptr->GetPosition());
		return((distance - this->GetCollisionRadius() - otherGameObjectCollidable_Ptr->GetCollisionRadius()) < 0);
	}

	void GameObjectCollidable::OnCollision()
	{
		std::cout << "GameObjectCollidable::OnCollision" << std::endl;
	}

	void GameObjectCollidable::AddToCollisionWhiteList(GameObjectCollidable* newWhiteListGameObjectCollidablePtr)
	{
		this->collisionWhiteList_ptr->insert(newWhiteListGameObjectCollidablePtr);
	}
}
