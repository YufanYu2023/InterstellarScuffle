#include "GameObject_Temporary.h"

namespace game 
{
	GameObjectTemporary::GameObjectTemporary(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float duration)
	: GameObject(geometry_Ptr, shader_Ptr, texture, gameData_Ptr, position, rotation)
	{
		this->duration = duration;
	}
	
	void GameObjectTemporary::Update(double delta_time, GLFWwindow* p_glfwWindow)
	{
		duration = (duration - delta_time);

		if (duration < 0)
		{
			this->alive = false; 
		}

		GameObject::Update(delta_time, p_glfwWindow);
	}
}
