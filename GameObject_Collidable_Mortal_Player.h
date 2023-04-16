#ifndef GAMEOBJECT_COLLIDABLE_MORTAL_PLAYER_H_
#define GAMEOBJECT_COLLIDABLE_MORTAL_PLAYER_H_

#include "GameObject_Collidable_Mortal.h"
#include <GLFW/glfw3.h>


namespace game
{
    class GameObjectCollidableMortalPlayer : public GameObjectCollidableMortal
    {
        public:
            GameObjectCollidableMortalPlayer(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float collisionRadius, int health);

            void Update(double delta_time, GLFWwindow* p_glfwWindow) override;
            void BoundsCheck(float maxXpostion, float maxYpostion) override;

            inline void ActivateHeathPack() { this->SetHealth(3); }
            inline void ActivateMutiShotTimer() { this->mutiShotTimer = 5; }
            inline void ActivateBigShotTimer() { this->bigShotTimer = 5; }
            

        private:
            void shoot();
            glm::vec3 GetWASDInput(GLFWwindow* p_glfwWindow);
            glm::vec3 GetWSQEInput(GLFWwindow* p_glfwWindow);
            float GetADInput(GLFWwindow* p_glfwWindow);

            float movementForce;
            bool shootKeyDown; 

            float mutiShotTimer;
            float bigShotTimer;
    };
}

#endif 
