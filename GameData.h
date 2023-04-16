#ifndef GAMEDATA_H_
#define GAMEDATA_H_

#include <vector>
#include "InstantiationManager.h"


namespace game
{   
    //!!The Compiler will not tell you if something is wong with a forward declaration!!
    class GameObject;// forward declaration
    class GameObjectCollidable;// forward declaration
    class InstantiationManager; // forward declaration
    class GameObjectCollidableMortalEnemy; // forward declaration
    class GameObjectCollidableMortalPlayer;

    /*
        GameData is responsible for holding all of the games important information
        so that other objects can more easily access said important information
    */
    class GameData
    {
        public:
            GameData();
            inline void SetInstantiationManagerPtr(InstantiationManager* instantiationManagerPtr) { this->instantiationManagerPtr = instantiationManagerPtr; }
            inline void SetMortalPlayerPtr(GameObjectCollidableMortalPlayer* newPlayer_ptr) { mortalPlayer_ptr = newPlayer_ptr; }
            inline void SetPlayerPtr(GameObject* newPlayer_ptr) { player_ptr = newPlayer_ptr; }

            inline GameObjectCollidableMortalPlayer* GetMortalPlayerPtr() { return mortalPlayer_ptr; }
            inline GameObject* GetPlayerPtr() { return player_ptr; }



            inline void PushToGameObjects(GameObject* p_newGameObject) { p_gameObjects->push_back(p_newGameObject); }
            inline std::vector<GameObject*>* GetGameObjectsPtr() { return p_gameObjects; }

            inline void PushToCollidableGameObjects(GameObjectCollidable* p_newCollidableGameObject) { p_collidableGameObjects->push_back(p_newCollidableGameObject); }
            inline std::vector<GameObjectCollidable*>* GetCollidableGameObjectsPtr() { return p_collidableGameObjects; }

            InstantiationManager* GetInstantiationManagerPtr();

            //change enemy count
            inline void IncrementEnemies() { enemies++; }
            inline void DecrementEnemies() { if (enemies > 0){ enemies--; } }
            inline int GetEnemies() { return enemies; }

            //swithc to next wave
            inline void IncrementWave() { wave++; }
            inline int GetWave() { return wave; }

            //keep track of current wave
            inline void IncrementCurrentWave() { currentWave++; }
            inline int GetCurrentWave() { return currentWave; }

            inline GameObject* GetHealth() { return health; }
            inline void SetHealth(GameObject* newHealth) { health = newHealth; }

           

        private:
            std::vector<GameObject*> * p_gameObjects;
            std::vector<GameObjectCollidable*>* p_collidableGameObjects;

            InstantiationManager* instantiationManagerPtr;//used to more easly allow gameobjects to Instantiate new gameObjects into the world 
            GameObjectCollidableMortalPlayer* mortalPlayer_ptr;
            GameObject* player_ptr;

            GameObject* health;
            int enemies;
            int wave;
            int currentWave;
            //std::vector<GameObjectCollidableMortalEnemy> *p_enemies;
    };
}

#endif 