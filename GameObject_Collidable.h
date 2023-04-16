#ifndef GAMEOBJECT_COLLIDABLE_H_
#define GAMEOBJECT_COLLIDABLE_H_

#include "GameObject.h"
#include <set>

namespace game
{
    //class GameData;// forward declaration

    class GameObjectCollidable : public GameObject
    {
        public:
            GameObjectCollidable(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float collisionRadius);

            //inline int GetType() { return 1; };//used to communicate that this gameObject is of a given type, (0->Default, 1->Collidable)

            virtual void Update(double delta_time, GLFWwindow* p_glfwWindow) override;

            inline float GetCollisionRadius(void) { return collisionRadius; }
            inline void SetCollisionRadius(float newCollisionRadius) { collisionRadius = newCollisionRadius; }

            /* Checks if this GameObjectCollidable is colliding with any other Collidable GameObjects */
            virtual void CheckForCollisions();
            /* Checks if this GameObjectCollidable is colliding wih the otjer GameObjectCollidable */
            virtual bool CollidingWith(GameObjectCollidable* otherGameObject_Ptr);

            /* Handels what happens when this GameObjectCollidable collides with something. Outputs: [0 neutral],[-1 dead] */
            virtual void OnCollision();

            void AddToCollisionWhiteList(GameObjectCollidable* newWhiteListGameObjectCollidablePtr);

        protected:
            std::set<GameObjectCollidable*>* collisionWhiteList_ptr;//GameObjectCollidables that this collidable will ignore 
            

        private:
            float collisionRadius; 
           
    };
}

#endif 
