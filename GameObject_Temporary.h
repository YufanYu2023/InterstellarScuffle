#ifndef GAMEOBJECT_TEMPORARY_H_
#define GAMEOBJECT_TEMPORARY_H_

#include "GameObject.h"

namespace game 
{   
    // Inherits from GameObject
    class GameObjectTemporary : public GameObject
    {
        public:
            GameObjectTemporary(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float duration);


            void Update(double delta_time, GLFWwindow* p_glfwWindow) override;
            inline int getDuration() { return duration; }

        private:
            float duration;
    }; 
    

} // namespace game

#endif 
