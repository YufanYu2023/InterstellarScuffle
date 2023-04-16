#include "GameObject_Collidable.h"
#include "GameObject_Collidable_Mortal.h"

namespace game
{
	GameObjectCollidableMortal::GameObjectCollidableMortal(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float collisionRadius, int health)
	: GameObjectCollidable(geometry_Ptr, shader_Ptr, texture, gameData_Ptr, position, rotation, collisionRadius)
	{
		this->health = health;
	}


	void GameObjectCollidableMortal::Update(double delta_time, GLFWwindow* p_glfwWindow)
	{
		if (this->GetHealth() <= 0)
		{
			this->alive = false;
			gameData_Ptr->GetInstantiationManagerPtr()->InstantiateExplosion(this->position);
		}

		GameObjectCollidable::Update(delta_time, p_glfwWindow);
	}


	void GameObjectCollidableMortal::OnCollision()
	{
		this->ModifyHealth(-1);
	}
} 
