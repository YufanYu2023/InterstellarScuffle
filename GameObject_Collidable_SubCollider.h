#ifndef GAMEOBJECT_COLLIDABLE_SUBCOLLIDER_H_
#define GAMEOBJECT_COLLIDABLE_SUBCOLLIDER_H_

#include "GameObject_Collidable.h"

namespace game
{
    //class GameData;// forward declaration

    class GameObjectCollidableSubCollider : public GameObjectCollidable
    {
        public:
            GameObjectCollidableSubCollider(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float collisionRadius, GameObjectCollidable* collidableParrent_ptr);
            virtual void OnCollision() override;

         private:
            GameObjectCollidable* collidableParrent_ptr;
    };
}

#endif 