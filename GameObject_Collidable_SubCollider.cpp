#include "GameObject_Collidable.h"
#include "GameObject_Collidable_SubCollider.h"

namespace game
{
	GameObjectCollidableSubCollider::GameObjectCollidableSubCollider(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float collisionRadius, GameObjectCollidable* collidableParrent_ptr)
	: GameObjectCollidable(geometry_Ptr, shader_Ptr, texture, gameData_Ptr, position, rotation, collisionRadius)
	{
		this->collidableParrent_ptr = collidableParrent_ptr;
	}


	void GameObjectCollidableSubCollider::OnCollision()
	{
		if(collidableParrent_ptr != NULL)
		{
			collidableParrent_ptr->OnCollision();
		}
	}
}
