#include "GameObject_Collidable_Mortal_Enemy.h"
#include <GLFW/glfw3.h>
#include "VectorMath.h"
#include <iostream>

namespace game
{
	GameObjectCollidableMortalEnemy::GameObjectCollidableMortalEnemy(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float collisionRadius, int health)
	: GameObjectCollidableMortal(geometry_Ptr, shader_Ptr, texture, gameData_Ptr, position, rotation, collisionRadius, health)
	{
		//this->state = state;
		//this->movementSpeed = 2.5f; 
		//this->targetPosition = targetPosition;
		this->p_targetGameObject = p_targetGameObject;
		//this->orbitRadius = orbitRadius;

		this->fireTimmer = 0;
		this->fireFrequency = 0.0;

		this->turnSpeed = 0;
	}

	void GameObjectCollidableMortalEnemy::Update(double delta_time, GLFWwindow* p_glfwWindow)
	{	
		if (fireFrequency > 0)
		{
			fireTimmer = fireTimmer - (float)delta_time;
			if (fireTimmer <= 0)
			{
				FireProjectile();
				fireTimmer = fireFrequency;
			}
		}

		if(turnSpeed != 0)
		{
			glm::vec3 vectorToTarget = VectorMath::VectorFromAtoB(this->GetPosition(), p_targetGameObject->GetPosition());
			float angleToVectorToTarget = VectorMath::AngleFormAtoB(this->GetRightwardVector(), vectorToTarget);
			rotation = rotation - (angleToVectorToTarget * turnSpeed * delta_time);
		}

		GameObjectCollidableMortal::Update(delta_time, p_glfwWindow);
	}

	void GameObjectCollidableMortalEnemy::BoundsCheck(float maxXpostion, float maxYpostion)
	{
		if ((this->position.x > maxXpostion) || (this->position.x < -maxXpostion))//check if we are out of bounds 
		{
			this->velocity = glm::vec3(-this->velocity.x, this->velocity.y, 0);
		}

		if (this->position.y < -maxYpostion)//check if we are out of bounds
		{
			this->gameData_Ptr->DecrementEnemies();
			this->alive = false; 
		}
	}

	void GameObjectCollidableMortalEnemy::OnCollision()
	{
		this->ModifyHealth(-1);

		if (this->GetHealth() <= 0)
		{
			this->gameData_Ptr->DecrementEnemies();
			this->alive = false;
			gameData_Ptr->GetInstantiationManagerPtr()->InstantiateExplosion(this->position);
		}
	}

	void GameObjectCollidableMortalEnemy::FireProjectile()
	{
		//fire correct projectile according to projectileType
		switch (this->projectileType) {
		case 0:
			gameData_Ptr->GetInstantiationManagerPtr()->InstantiateSlowProjectile(this->position, this->GetForwardVector() * 8.0f, this->GetRotation() + 180, 7.0f);
			break;
		case 1:
			gameData_Ptr->GetInstantiationManagerPtr()->InstantiateFastProjectile(this->position, this->GetForwardVector() * 16.0f, this->GetRotation() + 180, 7.0f);
			break;
		case 2:
			gameData_Ptr->GetInstantiationManagerPtr()->InstantiateMissile(this->position, glm::vec3(0, 0, 0), 7.0f);
			break;
		}
	}

	/*
	glm::vec3 GameObjectCollidableMortalEnemy::FindMaintainRadiusMovementVector()
	{
		glm::vec3 vectorToTargetGameObject = VectorMath::VectorFromAtoB(this->GetPosition(), p_targetGameObject->GetPosition());
		float orbitRadiusDifference = VectorMath::Magnitude(vectorToTargetGameObject) - orbitRadius;
		return VectorMath::Clamp(-vectorToTargetGameObject * orbitRadiusDifference, 1) * movementSpeed;
	}

	glm::vec3 GameObjectCollidableMortalEnemy::FindMaintainOrbitMovementVector(float orbitDirection)
	{
		glm::vec3 vectorToTargetGameObject = VectorMath::VectorFromAtoB(this->GetPosition(), p_targetGameObject->GetPosition());

		float orbitRadiusDifference = VectorMath::Magnitude(vectorToTargetGameObject) - orbitRadius;
		glm::vec3 orbitRadiusCorrectionVector = ( - vectorToTargetGameObject * orbitRadiusDifference);

		glm::vec3 roughOrbitMovementVector = VectorMath::CrossProduct(vectorToTargetGameObject, glm::vec3(0.0, 0.0, orbitDirection));

		return VectorMath::Normalize(roughOrbitMovementVector + orbitRadiusCorrectionVector) * movementSpeed;
	}
	*/
}
