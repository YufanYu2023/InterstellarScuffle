#include "GameObject_Collidable_Collectable.h"


namespace game 
{
		GameObjectCollideableCollectable::GameObjectCollideableCollectable(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float collisionRadius, int type)
		: GameObjectCollidable(geometry_Ptr, shader_Ptr, texture, gameData_Ptr, position, rotation, collisionRadius)
		{
			this->type = type;
		}


		void GameObjectCollideableCollectable::OnCollision() {

			switch (type)
			{
				case 0:
				{
					/*
					int temp = gameData_Ptr->GetMortalPlayerPtr()->GetHealth();
					if (temp >= 3) {
						gameData_Ptr->GetMortalPlayerPtr()->SetHealth(temp + 1);
					}
					else {
						gameData_Ptr->GetMortalPlayerPtr()->SetHealth(temp + 2);
					}
					*/

					gameData_Ptr->GetMortalPlayerPtr()->ActivateHeathPack();
				}
				break;

				case 1:
				{
					gameData_Ptr->GetMortalPlayerPtr()->ActivateMutiShotTimer();
				}
				break;

				case 2: 
				{
					gameData_Ptr->GetMortalPlayerPtr()->ActivateBigShotTimer();
				}
				break;
			}

			gameData_Ptr->GetMortalPlayerPtr()->ModifyHealth(1);
			this->alive = false;
		}
}
