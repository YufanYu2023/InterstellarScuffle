#include "GameObject_Collidable_Mortal_Player.h"
#include <GLFW/glfw3.h>
#include "VectorMath.h"
#include "Game.h"
#include <iostream>

namespace game
{
	GameObjectCollidableMortalPlayer::GameObjectCollidableMortalPlayer(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float collisionRadius, int health)
	: GameObjectCollidableMortal(geometry_Ptr, shader_Ptr, texture, gameData_Ptr, position, rotation, collisionRadius, health)
	{
		this->movementForce = 20.0f;
		//this->rotationSpeed = 1.0f; 
		this->drag = glm::vec2(0.001f, 0.001f);
		//this->normalDrag = 0.001f;
		//this->handbrakeDrag = 0.01f;
		this->shootKeyDown = false; 

		this->mutiShotTimer = 0; 
		this->bigShotTimer = 0;
	}

	// Update function for moving the player object around
	void GameObjectCollidableMortalPlayer::Update(double delta_time, GLFWwindow* p_glfwWindow)
	{	
		/* Turning based movement
		//turning
		AddAngularVelocity(GetADInput(p_glfwWindow) * rotationSpeed * (float)delta_time);

		if (glfwGetKey(p_glfwWindow, GLFW_KEY_W) == GLFW_PRESS) 
		{ 
			AddLocalVelocity(glm::vec3(0, (movementForce * (float)delta_time), 0));
		}

		if (glfwGetKey(p_glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			this->SetDrag(handbrakeDrag);
		}
		else
		{
			this->SetDrag(normalDrag);
		}
		*/

	
		AddLocalVelocity(GetWASDInput(p_glfwWindow) * movementForce * (float)delta_time);

		if (mutiShotTimer > 0) { mutiShotTimer = mutiShotTimer - delta_time; }
		if (bigShotTimer > 0) { bigShotTimer = bigShotTimer - delta_time; }

		

		//shooting
		if (glfwGetKey(p_glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if(shootKeyDown == false)
			{
				shoot();
				shootKeyDown = true; 
			}
		}
		else
		{
			shootKeyDown = false; 
		}

		GameObjectCollidableMortal::Update(delta_time, p_glfwWindow);

		//std::cout << "playerXPos:" << this->position.x << std::endl;
	}

	void GameObjectCollidableMortalPlayer::BoundsCheck(float maxXpostion, float maxYpostion)
	{
		//player can't go out of bounds
		if (this->position.x > maxXpostion) { this->position.x = maxXpostion;   this->velocity = glm::vec3(0, this->velocity.y, 0); }
		if (this->position.x < -maxXpostion) { this->position.x = -maxXpostion; this->velocity = glm::vec3(0, this->velocity.y, 0); }
		if (this->position.y > maxYpostion) { this->position.y = maxYpostion;   this->velocity = glm::vec3(this->velocity.x, 0, 0); }
		if (this->position.y < -maxYpostion) { this->position.y = -maxYpostion; this->velocity = glm::vec3(this->velocity.x, 0, 0); }
	}


	void GameObjectCollidableMortalPlayer::shoot()
	{
		glm::vec3 shotPosition = this->position; //this->position + (this->GetForwardVector() * 1.5f)
		glm::vec3 shotVelocity = this->velocity + (GetForwardVector() * 20.0f);

		int shotTexture = 14;//defalt 
		float shotCollisonSize = 0.5f;//defalt

		if(bigShotTimer > 0)
		{
			shotTexture = 15;
			shotCollisonSize = 1.5f;
		}
		
		if(mutiShotTimer <= 0)
		{
			//single shot
			gameData_Ptr->GetInstantiationManagerPtr()->InstantiateGameObjectCollidableProjectile(shotPosition, shotTexture, shotVelocity, this->rotation, shotCollisonSize, 3.0f, this->gameData_Ptr->GetMortalPlayerPtr());
		}
		else
		{
			//mutiShot
			gameData_Ptr->GetInstantiationManagerPtr()->InstantiateGameObjectCollidableProjectileCluster(shotPosition, shotTexture, shotVelocity, this->rotation, shotCollisonSize, 3.0f, this->gameData_Ptr->GetMortalPlayerPtr());
		}
	}

	glm::vec3 GameObjectCollidableMortalPlayer::GetWASDInput(GLFWwindow* p_glfwWindow)
	{
		glm::vec3 wsqeInput = glm::vec3(0.0, 0.0, 0.0);

		if (glfwGetKey(p_glfwWindow, GLFW_KEY_W) == GLFW_PRESS) { wsqeInput += glm::vec3(0.0, 1.0, 0.0); }//up
		if (glfwGetKey(p_glfwWindow, GLFW_KEY_S) == GLFW_PRESS) { wsqeInput += glm::vec3(0.0, -1.0, 0.0); }//down
		if (glfwGetKey(p_glfwWindow, GLFW_KEY_A) == GLFW_PRESS) { wsqeInput += glm::vec3(1.0, 0.0, 0.0); }//left
		if (glfwGetKey(p_glfwWindow, GLFW_KEY_D) == GLFW_PRESS) { wsqeInput += glm::vec3(-1.0, 0.0, 0.0); }//right

		wsqeInput = VectorMath::Clamp(wsqeInput, 1);

		return wsqeInput;
	}

	//these are not used ignore it
	glm::vec3 GameObjectCollidableMortalPlayer::GetWSQEInput(GLFWwindow* p_glfwWindow)
	{
		glm::vec3 wsqeInput = glm::vec3(0.0, 0.0, 0.0);

		if (glfwGetKey(p_glfwWindow, GLFW_KEY_W) == GLFW_PRESS) { wsqeInput += glm::vec3(0.0, 1.0, 0.0); }//up
		if (glfwGetKey(p_glfwWindow, GLFW_KEY_S) == GLFW_PRESS) { wsqeInput += glm::vec3(0.0, -1.0, 0.0); }//down
		if (glfwGetKey(p_glfwWindow, GLFW_KEY_Q) == GLFW_PRESS) { wsqeInput += glm::vec3(-1.0, 0.0, 0.0); }//left
		if (glfwGetKey(p_glfwWindow, GLFW_KEY_E) == GLFW_PRESS) { wsqeInput += glm::vec3(1.0, 0.0, 0.0); }//right

		wsqeInput = VectorMath::Clamp(wsqeInput, 1);

		return wsqeInput;
	}

	float GameObjectCollidableMortalPlayer::GetADInput(GLFWwindow* p_glfwWindow)
	{
		float adInput = 0; 

		if (glfwGetKey(p_glfwWindow, GLFW_KEY_A) == GLFW_PRESS) { adInput +=  1; }//port
		if (glfwGetKey(p_glfwWindow, GLFW_KEY_D) == GLFW_PRESS) { adInput += -1; }//starboard

		return adInput;
	}
} 
