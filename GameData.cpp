#include "GameData.h"
#include <vector>
#include "GameObject.h"
#include "GameObject_Collidable.h"
#include "GameObject_Collidable_Mortal_Enemy.h"
namespace game {

    GameData::GameData()
    {
        this->p_gameObjects = new std::vector<GameObject*>();
        this->p_collidableGameObjects = new std::vector<GameObjectCollidable*>();
        this->enemies = 0;
        this->wave = -1;//should be -1
        this->currentWave = 0;
    }


    InstantiationManager* GameData::GetInstantiationManagerPtr()
    {
        return this->instantiationManagerPtr; 
    }
} 