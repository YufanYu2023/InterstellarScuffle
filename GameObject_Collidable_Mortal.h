
#ifndef GAMEOBJECT_COLLIDABLE_MORTAL_H_
#define GAMEOBJECT_COLLIDABLE_MORTAL_H_

#include "GameObject_Collidable.h"

namespace game
{
    class GameObjectCollidableMortal : public GameObjectCollidable
    {
        public:
            GameObjectCollidableMortal(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float collisionRadius, int health);
            virtual void Update(double delta_time, GLFWwindow* p_glfwWindow) override;
            inline int GetHealth() { return health; }
            inline void SetHealth(int newHealth) { health = newHealth; }
            inline void ModifyHealth(int healthDelta) { health += healthDelta; }
            virtual void OnCollision() override;

        private:
            int health; 
    };
}

#endif 