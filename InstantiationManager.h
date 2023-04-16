#ifndef INSTANTIATIONMANAGER_H_
#define INSTANTIATIONMANAGER_H_

#include "shader.h"
#include <GLFW/glfw3.h>
#include <vector>

#include "geometry.h"



namespace game
{
    class GameData;// forward declaration
    class GameObject;// forward declaration //might be unnecessary
    class GameObjectCollidable;// forward declaration //might be unnecessary

    class InstantiationManager
    {
        //in charge of instantiating all the game objects
        public:
            InstantiationManager(GameData*, Geometry*, Geometry*, Shader*, Shader*, GLuint*);
            void InstantiateGameObject(GameObject*);
            void InstantiateGameObjectCollidable(GameObjectCollidable*);
            void InstantiateGameObjectCollidableProjectile(glm::vec3& position, int textureIndex, glm::vec3 initialVelocity, float initialRotation, float collisionRadius, float duration, GameObjectCollidable* emmiter_Ptr);
            void InstantiateGameObjectCollidableProjectileCluster(glm::vec3& position, int textureIndex, glm::vec3 initialVelocity, float initialRotation, float collisionRadius, float duration, GameObjectCollidable* emmiter_Ptr);
            void InstantiateSlowProjectile(glm::vec3& position, glm::vec3 initialVelocity, float initialRotation, float duration);
            void InstantiateFastProjectile(glm::vec3& position, glm::vec3 initialVelocity, float initialRotation, float duration);
            void InstantiateMissile(glm::vec3& position, glm::vec3 initialVelocity, float duration);
            void InstantiateExplosion(glm::vec3& position);

        private:
            GameData* gameData_Ptr;
            Geometry* p_spriteGeometry;
            Geometry* p_particlesGeometry; 
            Shader* p_spriteShader;
            Shader* p_particleShader;
            GLuint* p_textures;
            //glm::vec2* p_gameScale;
            //std::vector<GameObject*> p_instantiatedGameObjects;//made obsolete by p_gameData 
    };
} 
#endif 
