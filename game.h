#ifndef GAME_H_
#define GAME_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "GameData.h"
#include "shader.h"
#include "GameObject.h"
#include "GameObject_Temporary.h"
#include "GameObject_Collidable_Mortal.h"
#include "GameObject_Collidable_Mortal_Enemy.h"
#include "GameObject_Collidable_Mortal_Player.h"
#include "BackGroundPane.h"


namespace game {

    // A class for holding the main game objects
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();

            // Call Init() before calling any other method
            // Initialize graphics libraries and main window
            void Init(void); 

            // Set up the game (scene, game objects, etc.)
            void Setup(void);

            // Run the game (keep the game active)
            void MainLoop(void); 

            void WaveSpawner(); 

            void SpawnWave();

            void SpawnSmolEnemy(glm::vec3 position, glm::vec3 velocity, glm::vec2 drag);
            void SpawnSniperEnemy(glm::vec3 position, glm::vec3 velocity, glm::vec2 drag);
            void SpawnBomberEnemy(glm::vec3 position, glm::vec3 velocity, glm::vec2 drag);
            void SpawnBossEnemy(glm::vec3 position, glm::vec3 velocity, glm::vec2 drag);
            


            void SpawnPowerUpHealth(glm::vec3 position, glm::vec3 velocity);
            void SpawnPowerUpMultiShot(glm::vec3 position, glm::vec3 velocityr);
            void SpawnPowerUpBigShot(glm::vec3 position, glm::vec3 velocity);

            void UpdateHealthUI();

        private:
            GameData* gameData_Ptr; 


            GLFWwindow* GLFWwindow_Ptr;// Main window: pointer to the GLFW window structure       
            Geometry* spriteGeometry_Ptr;// Sprite geometry Pointer  
            Geometry* particleGeometry_Ptr; // Particle geometry Pointer
            Shader spriteShader; // Shader for rendering sprites in the scene
            Shader particleShader; // Shader for rendering particles
            #define NUM_TEXTURES 32
            GLuint textures[NUM_TEXTURES];// References to textures

         
            GameObject* playerGameObject_Ptr;
            std::vector<BackGroundPane*> backGroundPanes_ptr;
            glm::vec2  gameScale;
            double current_time_;// Keep track of time

;            
            static void ResizeCallback(GLFWwindow* window, int width, int height);// Callback for when the window is resized

            //functions 
            void SetTexture(GLuint w, const char *fname);// Set a specific texture        
            void SetAllTextures();// Load all textures  
            void Update(glm::mat4 view_matrix, double delta_time);// Update the game based on user input and simulation

          

    }; // class Game

} // namespace game

#endif // GAME_H_
