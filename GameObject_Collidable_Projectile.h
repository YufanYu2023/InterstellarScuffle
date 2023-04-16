#ifndef GAMEOBJECT_COLLIDABLE_PROJECTILE_H_
#define GAMEOBJECT_COLLIDABLE_COLLECTIBLE_H_

#include "GameObject_Collidable.h"

namespace game
{
    class GameObjectCollidableProjectile : public GameObjectCollidable
    {
        public:
            GameObjectCollidableProjectile(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, const glm::vec3& initialVelocity, float collisionRadius, float duration);

            void Update(double delta_time, GLFWwindow* p_glfwWindow) override;
            void BoundsCheck(float maxXpostion, float maxYpostion) override;
            bool CollidingWith(GameObjectCollidable* p_otherGameObjectCollidable) override;
            void OnCollision() override;

            inline void SetTargetPtr(GameObject* newTarget_ptr) { target_ptr = newTarget_ptr; }
            inline void SetTargetMagnetism(float newtargetMagnetism) { targetMagnetism = newtargetMagnetism; }

        private:
            float duration;
            glm::vec3 lastPosition;
            float travelRotation();
            GameObject* target_ptr;  
            float targetMagnetism; 
    };
}

#endif 