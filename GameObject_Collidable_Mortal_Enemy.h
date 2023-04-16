#ifndef GAMEOBJECT_COLLIDABLE_MORTAL_ENEMY_H_
#define GAMEOBJECT_COLLIDABLE_MORTAL_ENEMY_H_

#include "GameObject_Collidable_Mortal.h"
#include <GLFW/glfw3.h>

namespace game
{
    class GameObjectCollidableMortalEnemy : public GameObjectCollidableMortal
    {
        public:
            GameObjectCollidableMortalEnemy(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float collisionRadius, int health);

            void Update(double delta_time, GLFWwindow* p_glfwWindow) override;
            void BoundsCheck(float maxXpostion, float maxYpostion) override;
            void OnCollision() override;

            void FireProjectile(); 
            inline void SetProjectileType(int newProjectileType) { projectileType = newProjectileType; }
            inline void SetTargetGameObject(game::GameObject* p_newtargetGameObject) { p_targetGameObject = p_newtargetGameObject; }
            inline void SetFireFrequencey(float newFireFrequency) { fireFrequency = newFireFrequency; }
            inline void SetTurnSpeed(float newTurnSpeed) { turnSpeed = newTurnSpeed; }
            
 
        private:
            int projectileType;//0:slow, 1:fast, 2:missile 
            game::GameObject* p_targetGameObject;
            //float orbitRadius;
            float fireTimmer; 
            float fireFrequency;
            float turnSpeed;
    };
}

#endif 
