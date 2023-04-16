
#include "InstantiationManager.h"
#include "GameData.h"
#include "GameObject_Collidable_Projectile.h"
#include "GameObject_Temporary.h"
#include "particle_system.h"
#include "particles.h"
#include "GameObject_Temporary.h"

namespace game 
{ 
    InstantiationManager::InstantiationManager(GameData* p_gameData, Geometry* p_spriteGeometry, Geometry* p_particlesGeometry, Shader* p_spriteShader, Shader* p_particleShader, GLuint* p_textures)
    {
        //get pointers to the game data and the geometry
        this->gameData_Ptr = p_gameData;
        this->p_spriteGeometry = p_spriteGeometry;
        this->p_particlesGeometry = p_particlesGeometry;
        this->p_spriteShader = p_spriteShader;
        this->p_particleShader = p_particleShader;
        this->p_textures = p_textures;
    }

    void InstantiationManager::InstantiateGameObject(GameObject* p_newGameObject)
    {
        gameData_Ptr->PushToGameObjects(p_newGameObject);
    }

    void InstantiationManager::InstantiateGameObjectCollidable(GameObjectCollidable* p_newGameObjectCollidable)
    {
        gameData_Ptr->PushToCollidableGameObjects(p_newGameObjectCollidable);
    }

    void InstantiationManager::InstantiateGameObjectCollidableProjectile(glm::vec3& position, int textureIndex, glm::vec3 initialVelocity, float initialRotation, float collisionRadius, float duration, GameObjectCollidable* emmiter_Ptr = NULL)
    {
        //create a new projectile
        GameObjectCollidableProjectile* p_newProjectile = new GameObjectCollidableProjectile(this->p_spriteGeometry, this->p_spriteShader, this->p_textures[textureIndex], this->gameData_Ptr, position, initialRotation, initialVelocity, collisionRadius, duration);
        
        p_newProjectile->SetScale(glm::vec2(collisionRadius * 2.0f, collisionRadius * 2.0f));

        if (emmiter_Ptr != NULL) 
        {
            emmiter_Ptr->AddToCollisionWhiteList(p_newProjectile);
            p_newProjectile->AddToCollisionWhiteList(emmiter_Ptr);
        }

        //GameObject* p_particles = new ParticleSystem(glm::vec3(0, 0, 0), this->p_particlesGeometry, this->p_particleShader, this->p_textures[4], glm::vec2(0.1, 0.6));
        //p_particles->SetParrent(p_newProjectile);

       // p_instantiatedGameObjects.push_back(p_particles);
        //p_instantiatedGameObjects.push_back(p_newProjectile);

        //p_gameData->PushToGameObjects(p_particles);

        //push it to the game data
        gameData_Ptr->PushToCollidableGameObjects(p_newProjectile);
    }

    void InstantiationManager::InstantiateGameObjectCollidableProjectileCluster(glm::vec3& position, int textureIndex, glm::vec3 initialVelocity, float initialRotation, float collisionRadius, float duration, GameObjectCollidable* emmiter_Ptr = NULL)
    {
        glm::vec3 angledShotVelocityP = (initialVelocity * 0.8f) + glm::vec3(glm::length(initialVelocity) * 0.2, 0, 0);
        glm::vec3 angledShotVelocityN = (initialVelocity * 0.8f) + glm::vec3(glm::length(initialVelocity) * -0.2, 0, 0);
        //set up a whicte list for the projectiles
        GameObjectCollidableProjectile* p_newProjectileA = new GameObjectCollidableProjectile(this->p_spriteGeometry, this->p_spriteShader, this->p_textures[textureIndex], this->gameData_Ptr, position, initialRotation, angledShotVelocityN, collisionRadius, duration);
        GameObjectCollidableProjectile* p_newProjectileB = new GameObjectCollidableProjectile(this->p_spriteGeometry, this->p_spriteShader, this->p_textures[textureIndex], this->gameData_Ptr, position, initialRotation, initialVelocity, collisionRadius,  duration);
        GameObjectCollidableProjectile* p_newProjectileC = new GameObjectCollidableProjectile(this->p_spriteGeometry, this->p_spriteShader, this->p_textures[textureIndex], this->gameData_Ptr, position, initialRotation, angledShotVelocityP, collisionRadius, duration);

        p_newProjectileA->SetScale(glm::vec2(collisionRadius * 2.0f, collisionRadius * 2.0f));
        p_newProjectileB->SetScale(glm::vec2(collisionRadius * 2.0f, collisionRadius * 2.0f));
        p_newProjectileC->SetScale(glm::vec2(collisionRadius * 2.0f, collisionRadius * 2.0f));

        p_newProjectileA->AddToCollisionWhiteList(p_newProjectileB);
        p_newProjectileA->AddToCollisionWhiteList(p_newProjectileC);
        p_newProjectileB->AddToCollisionWhiteList(p_newProjectileA);
        p_newProjectileB->AddToCollisionWhiteList(p_newProjectileC);
        p_newProjectileC->AddToCollisionWhiteList(p_newProjectileA);
        p_newProjectileC->AddToCollisionWhiteList(p_newProjectileB);

        if (emmiter_Ptr != NULL)
        {
            emmiter_Ptr->AddToCollisionWhiteList(p_newProjectileA);
            emmiter_Ptr->AddToCollisionWhiteList(p_newProjectileB);
            emmiter_Ptr->AddToCollisionWhiteList(p_newProjectileC);
            p_newProjectileA->AddToCollisionWhiteList(emmiter_Ptr);
            p_newProjectileB->AddToCollisionWhiteList(emmiter_Ptr);
            p_newProjectileC->AddToCollisionWhiteList(emmiter_Ptr);
        }

       


        //GameObject* p_particles = new ParticleSystem(glm::vec3(0, 0, 0), this->p_particlesGeometry, this->p_particleShader, this->p_textures[4], glm::vec2(0.1, 0.6));
        //p_particles->SetParrent(p_newProjectile);

       // p_instantiatedGameObjects.push_back(p_particles);
        //p_instantiatedGameObjects.push_back(p_newProjectile);

        //p_gameData->PushToGameObjects(p_particles);

        gameData_Ptr->PushToCollidableGameObjects(p_newProjectileA);
        gameData_Ptr->PushToCollidableGameObjects(p_newProjectileB);
        gameData_Ptr->PushToCollidableGameObjects(p_newProjectileC);
    }

    void InstantiationManager::InstantiateSlowProjectile(glm::vec3& position, glm::vec3 initialVelocity, float initialRotation, float duration)
    {

        GameObjectCollidableProjectile* p_newProjectile = new GameObjectCollidableProjectile(this->p_spriteGeometry, this->p_spriteShader, this->p_textures[11], this->gameData_Ptr, position, initialRotation, initialVelocity, 1, duration);
        p_newProjectile->SetCollisionRadius(0.1f);
        p_newProjectile->SetTargetPtr(this->gameData_Ptr->GetPlayerPtr());

        gameData_Ptr->PushToGameObjects(p_newProjectile);
    }


    void InstantiationManager::InstantiateFastProjectile(glm::vec3& position, glm::vec3 initialVelocity, float initialRotation, float duration)
    {
        GameObjectCollidableProjectile* p_newProjectile = new GameObjectCollidableProjectile(this->p_spriteGeometry, this->p_spriteShader, this->p_textures[12], this->gameData_Ptr, position, initialRotation, initialVelocity, 1, duration);
        p_newProjectile->SetCollisionRadius(0.1f);
        p_newProjectile->SetTargetPtr(this->gameData_Ptr->GetPlayerPtr());

        gameData_Ptr->PushToGameObjects(p_newProjectile);
    }


    void InstantiationManager::InstantiateMissile(glm::vec3& position, glm::vec3 initialVelocity, float duration)
    {
        GameObjectCollidableProjectile* p_newProjectile = new GameObjectCollidableProjectile(this->p_spriteGeometry, this->p_spriteShader, this->p_textures[13], this->gameData_Ptr, position, 0, initialVelocity, 1, duration);
        p_newProjectile->SetCollisionRadius(0.1f);
        p_newProjectile->SetTargetPtr(this->gameData_Ptr->GetPlayerPtr());
        p_newProjectile->SetTargetMagnetism(10.0f);
        gameData_Ptr->PushToGameObjects(p_newProjectile);
    }


    void InstantiationManager::InstantiateExplosion(glm::vec3& position)//create a new explosion gameObject  
    { 
        GameObjectTemporary* newExplosion_ptr = new GameObjectTemporary(this->p_spriteGeometry, this->p_spriteShader, this->p_textures[10], this->gameData_Ptr, position, 0, 1.618f);
        newExplosion_ptr->SetScale(glm::vec2(3, 3));
        gameData_Ptr->PushToGameObjects(newExplosion_ptr);   

    }
} 
