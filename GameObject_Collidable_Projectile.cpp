#include "GameObject_Collidable_Projectile.h"
#include "VectorMath.h"
#include <glm/gtc/matrix_transform.hpp>

namespace game
{
	GameObjectCollidableProjectile::GameObjectCollidableProjectile(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, const glm::vec3& initialVelocity, float collisionRadius, float duration)
	: GameObjectCollidable(geometry_Ptr, shader_Ptr, texture, gameData_Ptr, position, rotation, collisionRadius)
	{
		this->velocity = initialVelocity;
		this->duration = duration;
		this->targetMagnetism = 0;

		this->target_ptr = NULL; 
	}

	void GameObjectCollidableProjectile::Update(double delta_time, GLFWwindow* p_glfwWindow)
	{
		duration = duration - delta_time;

		if(duration < 0){ this->alive = false; }

		if(targetMagnetism != 0)
		{
			this->AddGlobalVelocity(VectorMath::Clamp(this->position - this->target_ptr->GetPosition(), 1) * -targetMagnetism * (float)delta_time);//add a force towards the target
		}
 
		lastPosition = this->position;

		GameObjectCollidable::Update(delta_time, p_glfwWindow);
	}

	void GameObjectCollidableProjectile::BoundsCheck(float maxXpostion, float maxYpostion)
	{
		if ((this->position.x > maxXpostion) || (this->position.x < -maxXpostion))//check if we are out of bounds 
		{
			this->velocity = glm::vec3(-this->velocity.x, this->velocity.y, 0);
		}

		if (this->position.y > maxYpostion)//check if we are out of bounds
		{
			this->alive = false;
		}

		if (this->position.y < -maxYpostion)//check if we are out of bounds
		{
			this->alive = false;
		}
	}

	bool GameObjectCollidableProjectile::CollidingWith(GameObjectCollidable* other_ptr)
	{
		/*
		//glm::vec4 otherRelativeWorldPosition = glm::vec4((p_otherc->GetPosition().x - this->position.x), (p_otherc->GetPosition().y - this->position.y), (p_otherc->GetPosition().z - this->position.z), 0);
		//glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(travelRotation()), glm::vec3(0.0, 0.0, 1.0));
		//glm::vec4 otherRelativeLocalPosition = otherRelativeWorldPosition * rotation_matrix;

		//std::cout << "whiteListSize:" << whiteList.size() << std::endl;
		*/


		if (this->target_ptr == NULL)
		{
			//returns false if otherGameObjectCollidable_Ptr is in this's collisionWhiteList
			if (this->collisionWhiteList_ptr->find(other_ptr) != this->collisionWhiteList_ptr->end()) { return false; }
		}
		else
		{
			if (other_ptr != target_ptr) { return false; }
		}

		float distance = glm::length(this->GetPosition() - other_ptr->GetPosition());
		return((distance - this->GetCollisionRadius() - other_ptr->GetCollisionRadius()) < 0);
	}

	float GameObjectCollidableProjectile::travelRotation()
	{
		glm::vec3 travelVector = this->position - this->lastPosition; 
		return VectorMath::AngleFormAtoB(glm::vec3(0, 1, 0), travelVector);
	}


	void GameObjectCollidableProjectile::OnCollision()
	{
		this->alive = false;
	}
}
