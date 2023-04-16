#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>
#include <path_config.h>
#include "sprite.h"
#include "shader.h"
#include "game.h"
#include "GameObject.h"
#include "GameObject_Collidable_Collectable.h"
#include "GameObject_Collidable_SubCollider.h"
#include "GameObject_Temporary.h"
#include "particle_system.h"
#include "particles.h"

//to do
//consolidate game object lists


namespace game 
{
    // Some configuration constants
    // They are written here as global variables, but ideally they should be loaded from a configuration file

    // Globals that define the OpenGL window and viewport
    const char *window_title_g = "GameEngine";
    const unsigned int window_width_g = 800;
    const unsigned int window_height_g = 800;
    const unsigned int gameWindowWidth = 20;
    const unsigned int gameWindowHight = 20;
    const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);

    // Directory with game resources such as textures
    const std::string resources_directory_g = RESOURCES_DIRECTORY;

    GameObjectCollidableMortal* player;


    Game::Game(void)
    {
        // Don't do work in the constructor, leave it for the Init() function
    }

    Game::~Game()
    {
        // Free memory for all objects
        // Only need to delete objects that are not automatically freed
        /*
        delete sprite_;
        for (int i = 0; i < game_objects_.size(); i++){
            delete game_objects_[i];
        }
        */

        // Close window
        glfwDestroyWindow(GLFWwindow_Ptr);
        glfwTerminate();
    }



    void Game::Init(void)
    {

        // Initialize the window management library (GLFW)
        if (!glfwInit()) {
            throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
        }

        // Set window to not resizable
        // Required or else the calculation to get cursor pos to screenspace will be incorrect
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

        // Create a window and its OpenGL context
        GLFWwindow_Ptr = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
        if (!GLFWwindow_Ptr) {
            glfwTerminate();
            throw(std::runtime_error(std::string("Could not create window")));
        }

        // Make the window's OpenGL context the current one
        glfwMakeContextCurrent(GLFWwindow_Ptr);

        // Initialize the GLEW library to access OpenGL extensions
        // Need to do it after initializing an OpenGL context
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
        }

        // Set event callbacks
        glfwSetFramebufferSizeCallback(GLFWwindow_Ptr, ResizeCallback);

        // Initialize sprite geometry
        spriteGeometry_Ptr = new Sprite();
        spriteGeometry_Ptr->CreateGeometry();

        // Initialize particle geometry
        particleGeometry_Ptr = new Particles();
        particleGeometry_Ptr->CreateGeometry();

        // Initialize sprite shader
        spriteShader.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/sprite_fragment_shader.glsl")).c_str());

        // Initialize particle shader
        particleShader.Init((resources_directory_g + std::string("/particle_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/particle_fragment_shader.glsl")).c_str());



        gameData_Ptr = new GameData();


        InstantiationManager* instantiationManager_Ptr = new InstantiationManager(gameData_Ptr, spriteGeometry_Ptr, particleGeometry_Ptr, &spriteShader, &particleShader, textures);
        gameData_Ptr->SetInstantiationManagerPtr(instantiationManager_Ptr);


        // Initialize game values
        current_time_ = 0.0;
        gameScale = glm::vec2(0.25f, 0.25f);
    }



    void Game::Setup(void)// Setup the game world
    {
        // Load textures
        SetAllTextures();


        //space station setup 
        GameObject* spaceStation_Ptr = new GameObject(spriteGeometry_Ptr, &spriteShader, textures[7], gameData_Ptr, glm::vec3(0, 0, 0), 0);
        spaceStation_Ptr->SetScale(glm::vec2(7.0f, 7.0f));
        spaceStation_Ptr->SetAngularVelocity(0.01f);
        spaceStation_Ptr->AddGlobalVelocity(glm::vec3(0.0f, -5.0f, 0.0f));

        ParticleSystem* spaceStationParticlesA_Ptr = new ParticleSystem(particleGeometry_Ptr, &particleShader, textures[0], gameData_Ptr, glm::vec3(2.1f, -2.6f, 0.0f), 45.0f);
        spaceStationParticlesA_Ptr->SetScale(glm::vec2(0.4f, 0.4f));
        spaceStationParticlesA_Ptr->SetParrent(spaceStation_Ptr);

        ParticleSystem* spaceStationParticlesB_Ptr = new ParticleSystem(particleGeometry_Ptr, &particleShader, textures[0], gameData_Ptr, glm::vec3(-0.5f, 3.1f, 0.0f), -170.0f);
        spaceStationParticlesB_Ptr->SetScale(glm::vec2(0.6f, 0.6f));
        spaceStationParticlesB_Ptr->SetParrent(spaceStation_Ptr);

        gameData_Ptr->PushToGameObjects(spaceStationParticlesA_Ptr);
        gameData_Ptr->PushToGameObjects(spaceStationParticlesB_Ptr);
        gameData_Ptr->PushToGameObjects(spaceStation_Ptr);


        //init player
        GameObjectCollidableMortalPlayer* player_Ptr = new GameObjectCollidableMortalPlayer(spriteGeometry_Ptr, &spriteShader, textures[0], gameData_Ptr, glm::vec3(5, -9,0), 0, 0.5f, 3);
        
        playerGameObject_Ptr = player_Ptr;
        gameData_Ptr->SetPlayerPtr(player_Ptr);
        gameData_Ptr->SetMortalPlayerPtr(player_Ptr);
        gameData_Ptr->PushToCollidableGameObjects(player_Ptr);





        


        //setup health ui
        GameObject* healthUI = new GameObject(spriteGeometry_Ptr, &spriteShader, textures[6], gameData_Ptr, glm::vec3(-18.0f, -19.0f, 0.0f), 0);
        healthUI->SetScale(glm::vec2(3.33333333333f,1.0f ));
        gameData_Ptr->SetHealth(healthUI);
        gameData_Ptr->PushToGameObjects(healthUI);
        //dynamic cast player into a collidable mortal game object
        player = dynamic_cast<GameObjectCollidableMortal*>(gameData_Ptr->GetPlayerPtr());


        //setup the background star tiles
        backGroundPanes_ptr.push_back(new BackGroundPane(spriteGeometry_Ptr, &spriteShader, textures[30], 40, glm::vec3(0, 40, 0), -0.5, 40, -40));
        backGroundPanes_ptr.push_back(new BackGroundPane(spriteGeometry_Ptr, &spriteShader, textures[31], 40, glm::vec3(0, 0, 0), -0.5, 40, -40));
    }


    void Game::ResizeCallback(GLFWwindow* window, int width, int height)
    {

        // Set OpenGL viewport based on framebuffer width and height
        glViewport(0, 0, width, height);
    }


    void Game::SetTexture(GLuint w, const char *fname)
    {
        // Bind texture buffer
        glBindTexture(GL_TEXTURE_2D, w);

        // Load texture from a file to the buffer
        int width, height;
        unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);

        // Texture Wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Texture Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }


    void Game::SetAllTextures(void)
    {
        // Load all textures that we will need
        glGenTextures(NUM_TEXTURES, textures);
        SetTexture(textures[0], (resources_directory_g+std::string("/textures/Ships/PlayerShipSprite.png")).c_str());
        SetTexture(textures[1], (resources_directory_g + std::string("/textures/Ships/SmolEnemySprite.png")).c_str());
        SetTexture(textures[2], (resources_directory_g + std::string("/textures/Ships/SniperEnemySprite.png")).c_str());
        SetTexture(textures[3], (resources_directory_g + std::string("/textures/Ships/BomberEnemySprite.png")).c_str());
        SetTexture(textures[4], (resources_directory_g + std::string("/textures/Ships/BossEnemySprite.png")).c_str());
        SetTexture(textures[5], (resources_directory_g + std::string("/textures/Ships/BossTurretEnemySprite.png")).c_str());
        SetTexture(textures[6], (resources_directory_g + std::string("/textures/Ships/BossTwinTurretEnemySprite.png")).c_str());
        SetTexture(textures[7], (resources_directory_g + std::string("/textures/SpaceStation.png")).c_str());
        

        //projectiles and explosions
        SetTexture(textures[10], (resources_directory_g + std::string("/textures/ExplosionSprite.png")).c_str());
        SetTexture(textures[11], (resources_directory_g + std::string("/textures/Projectiles/smallProjectile.png")).c_str());
        SetTexture(textures[12], (resources_directory_g + std::string("/textures/Projectiles/fastProjectile.png")).c_str());
        SetTexture(textures[13], (resources_directory_g + std::string("/textures/Projectiles/missile.png")).c_str());
        SetTexture(textures[14], (resources_directory_g + std::string("/textures/Projectiles/PlayerProjectileNormal.png")).c_str());
        SetTexture(textures[15], (resources_directory_g + std::string("/textures/Projectiles/PlayerProjectileBig.png")).c_str());
        //SetTexture(textures[21], (resources_directory_g + std::string("/textures/orb.png")).c_str());

        

        //health ui 
        SetTexture(textures[23], (resources_directory_g + std::string("/textures/zero_heart.png")).c_str());
        SetTexture(textures[24], (resources_directory_g + std::string("/textures/one_heart.png")).c_str());
        SetTexture(textures[25], (resources_directory_g + std::string("/textures/two_heart.png")).c_str());
        SetTexture(textures[26], (resources_directory_g + std::string("/textures/three_heart.png")).c_str());

        //power ups
        SetTexture(textures[27], (resources_directory_g + std::string("/textures/PowerUpHealth.png")).c_str());
        SetTexture(textures[28], (resources_directory_g + std::string("/textures/PowerUpMultiShot.png")).c_str());
        SetTexture(textures[29], (resources_directory_g + std::string("/textures/PowerUpBigShot.png")).c_str());

        //background star tiles
        SetTexture(textures[30], (resources_directory_g + std::string("/textures/BackGroundPanes/Starfield01.png")).c_str());
        SetTexture(textures[31], (resources_directory_g + std::string("/textures/BackGroundPanes/Starfield02.png")).c_str());
        glBindTexture(GL_TEXTURE_2D, textures[0]);
    }


    void Game::MainLoop(void)
    {
        // Loop while the user did not close the window
        double last_time = glfwGetTime();
        while (!glfwWindowShouldClose(GLFWwindow_Ptr)){

            // Clear background
            glClearColor(viewport_background_color_g.r,
                         viewport_background_color_g.g,
                         viewport_background_color_g.b, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Set view to zoom out
            float cameraZoom = 0.05f;
            //glm::mat4 viewTransform = glm::translate(glm::mat4(1.0f), -playerGameObject_Ptr->GetPosition());//tracks the player
            glm::mat4 viewScale = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom));     
            glm::mat4 view_matrix = viewScale;

            // Calculate delta time
            double current_time = glfwGetTime();
            double delta_time = current_time - last_time;
            last_time = current_time;

            // Update other events like input handling
            glfwPollEvents();

            // Update the game
            Update(view_matrix, delta_time);

            // Push buffer drawn in the background onto the display
            glfwSwapBuffers(GLFWwindow_Ptr);
        }
    }


    void Game::Update(glm::mat4 view_matrix, double delta_time)
    {
        // Update time
        current_time_ += delta_time;

        WaveSpawner();

       // std::cout << "wave:" << gameData_Ptr->GetWave() << std::endl;

        //check player health
        UpdateHealthUI();

       

        //Updates all GameObjects
        for (int i = 0; i < gameData_Ptr->GetGameObjectsPtr()->size(); i++)
        {
            GameObject* p_currentGameObject = gameData_Ptr->GetGameObjectsPtr()->at(i);
            //check if the game object is alive if dead remove it from the vector
            if (p_currentGameObject->GetAlive() == false)
            {
                gameData_Ptr->GetGameObjectsPtr()->erase(gameData_Ptr->GetGameObjectsPtr()->begin() + i);
            }
            else
            {
                p_currentGameObject->Update(delta_time, GLFWwindow_Ptr);
                p_currentGameObject->Render(view_matrix, current_time_);
                p_currentGameObject->BoundsCheck(gameWindowWidth, gameWindowHight);
            }
        }
        
        //Updates all Collidable GameObjects
        for (int i = 0; i < gameData_Ptr->GetCollidableGameObjectsPtr()->size(); i++)
        {
            GameObjectCollidable* p_currentCollidableGameObject = gameData_Ptr->GetCollidableGameObjectsPtr()->at(i);

            if (p_currentCollidableGameObject->GetAlive() == false)
            {
                delete p_currentCollidableGameObject;
                gameData_Ptr->GetCollidableGameObjectsPtr()->erase(gameData_Ptr->GetCollidableGameObjectsPtr()->begin() + i);
            }
            else
            {
                p_currentCollidableGameObject->Update(delta_time, GLFWwindow_Ptr);
                p_currentCollidableGameObject->Render(view_matrix, current_time_);
                p_currentCollidableGameObject->BoundsCheck(gameWindowWidth, gameWindowHight);
            }
        }
        

      

        
        // Update and render all objects in the backgroundtiles array
        for (int i = 0; i < backGroundPanes_ptr.size(); i++)
        {
            BackGroundPane* p_currentPane = backGroundPanes_ptr[i];
            p_currentPane->Update(delta_time);
            p_currentPane->Render(view_matrix, current_time_);
        }
    }

    void Game::WaveSpawner()
    {
        if((gameData_Ptr->GetEnemies() <= 0) && (gameData_Ptr->GetWave() < 5))
        {
            gameData_Ptr->IncrementWave();
            SpawnWave();
            //std::cout << "Wave:" << gameData_Ptr->GetWave() << std::endl;
        }
    }
    //setup how many enemies to spawn per wave
    void Game::SpawnWave()
    {
        switch (gameData_Ptr->GetWave())
        {
            case 0://Wave 0
            {
                std::cout << "Use the WASD keys to move around, and SPACE to fire!" << std::endl;
            }
            break;
            case 1://Wave 1
            {
                SpawnPowerUpHealth(glm::vec3(-17, 36, 0), glm::vec3(0.0f, -5.0f, 0.0f));
                SpawnPowerUpMultiShot(glm::vec3(3, 37, 0), glm::vec3(0.0f, -5.0f, 0.0f));
                SpawnPowerUpBigShot(glm::vec3(12, 23, 0), glm::vec3(0.0f, -5.0f, 0.0f));

                SpawnSmolEnemy(glm::vec3(-19, 24, 0),  glm::vec3(1.9f, -3.0f, 0.0f),   glm::vec2(0.0f, 0.3f));
                SpawnSmolEnemy(glm::vec3(0, 22, 0),    glm::vec3(2.2f, -3.0f, 0.0f),   glm::vec2(0.0f, 0.3f));
                SpawnSmolEnemy(glm::vec3(-19, 20, 0),  glm::vec3(2.1f, -3.0f, 0.0f),  glm::vec2(0.0f, 0.3f));
                SpawnSniperEnemy(glm::vec3(-9, 30, 0), glm::vec3(0.0f, -4.0f, 0.0f), glm::vec2(0.0f, 0.6f));//stops near top screen

            }
            break;
            case 2://Wave 2
            {
                SpawnPowerUpHealth(glm::vec3(-19, 38, 0), glm::vec3(0.0f, -5.0f, 0.0f));
                SpawnPowerUpMultiShot(glm::vec3(-2, 23, 0), glm::vec3(0.0f, -5.0f, 0.0f));
                SpawnPowerUpBigShot(glm::vec3(15, 39, 0), glm::vec3(0.0f, -5.0f, 0.0f));

                SpawnSmolEnemy(glm::vec3(-16, 22, 0), glm::vec3(-2.1f, -2.0f, 0.0f), glm::vec2(0, 0));
                SpawnSmolEnemy(glm::vec3(0, 24, 0),   glm::vec3(1.9f, -2.0f, 0.0f), glm::vec2(0, 0));
                SpawnSmolEnemy(glm::vec3(-8, 26, 0),  glm::vec3(-2.0f, -2.0f, 0.0f), glm::vec2(0, 0));
                SpawnSmolEnemy(glm::vec3(18, 28, 0), glm::vec3(1.8f, -2.0f, 0.0f), glm::vec2(0, 0));
                SpawnSmolEnemy(glm::vec3(5, 30, 0), glm::vec3(2.2f, -2.0f, 0.0f), glm::vec2(0, 0));

                SpawnSniperEnemy(glm::vec3(11, 32, 0), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f));
                SpawnSniperEnemy(glm::vec3(-9, 34, 0), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f));
            }
            break;
            case 3://Wave 3
            {
                //good 
                SpawnPowerUpHealth(glm::vec3(9, 20, 0), glm::vec3(0.0f, -5.0f, 0.0f));
                SpawnPowerUpMultiShot(glm::vec3(-11, 28, 0), glm::vec3(0.0f, -5.0f, 0.0f));
                SpawnPowerUpBigShot(glm::vec3(-5, 38, 0), glm::vec3(0.0f, -5.0f, 0.0f));

                //spawn Bomber wing
                SpawnBomberEnemy(glm::vec3(-7, 26, 0), glm::vec3(-1.06f, -2.1f, 0.0f), glm::vec2(0.1f, 0.2f));
                SpawnBomberEnemy(glm::vec3(-1, 26, 0), glm::vec3(0.0f, -1.9f, 0.0f),   glm::vec2(0.1f, 0.2f));
                SpawnBomberEnemy(glm::vec3(4, 26, 0),  glm::vec3(1.0f, -2.0f, 0.0f),   glm::vec2(0.1f, 0.2f));

                SpawnSniperEnemy(glm::vec3(-9, 22, 0), glm::vec3(-0.005f, -1.8f, 0.0f), glm::vec2(0, 0.12f));
                SpawnSniperEnemy(glm::vec3(3, 22, 0), glm::vec3(-0.005f, -1.7f, 0.0f),  glm::vec2(0, 0.12f));
                SpawnSniperEnemy(glm::vec3(12, 22, 0), glm::vec3(0.005f, -2.2f, 0.0f),  glm::vec2(0, 0.12f));
                SpawnSniperEnemy(glm::vec3(-15, 22, 0), glm::vec3(0.005f, -2.3f, 0.0f), glm::vec2(0, 0.12f));
            }
            break;
            case 4://Wave 4
            {
                SpawnPowerUpHealth(glm::vec3(-3, 30, 0), glm::vec3(0.0f, -5.0f, 0.0f));
                SpawnPowerUpMultiShot(glm::vec3(-15, 22, 0), glm::vec3(0.0f, -5.0f, 0.0f));
                SpawnPowerUpBigShot(glm::vec3(8, 20, 0), glm::vec3(0.0f, -5.0f, 0.0f));

                SpawnBossEnemy(glm::vec3(-5, 22, 0), glm::vec3(-5.0f, -3.0f, 0.0f), glm::vec2(0, 0.3f));

                SpawnBomberEnemy(glm::vec3(-20, 26, 0), glm::vec3(1.0f, -2.1f, 0.0f), glm::vec2(0.2f, 0.2f));
                SpawnBomberEnemy(glm::vec3(-7, 26, 0), glm::vec3(-1.0f, -1.9f, 0.0f), glm::vec2(0.2f, 0.2f));
                SpawnBomberEnemy(glm::vec3(19, 26, 0), glm::vec3(1.0f, -2.0f, 0.0f), glm::vec2(0.2f, 0.2f));
                SpawnBomberEnemy(glm::vec3(13, 26, 0), glm::vec3(-0.5f, -2.2f, 0.0f), glm::vec2(0.2f, 0.2f));

                SpawnSniperEnemy(glm::vec3(-9, 24, 0), glm::vec3(-0.005f, -2.3f, 0.0f), glm::vec2(0, 0.12f));
                SpawnSniperEnemy(glm::vec3(3, 24, 0), glm::vec3(-0.005f, -2.0f, 0.0f), glm::vec2(0, 0.13f));
                SpawnSniperEnemy(glm::vec3(12, 24, 0), glm::vec3(0.005f, -2.2f, 0.0f), glm::vec2(0, 0.11f));
                SpawnSniperEnemy(glm::vec3(-15, 24, 0), glm::vec3(0.005f, -1.7f, 0.0f), glm::vec2(0, 0.11f));
            }
            break;
            case 5://Wave 5
            {
                std::cout << "Congratulation you win!" << std::endl;
            }
            break;

        }
    }

    //spawners for different type of enemy
    void Game::SpawnSmolEnemy(glm::vec3 position, glm::vec3 velocity, glm::vec2 drag)
    {
        GameObjectCollidableMortalEnemy* newGameObject_ptr = new GameObjectCollidableMortalEnemy(spriteGeometry_Ptr, &spriteShader, textures[1], gameData_Ptr, position, 180, 0.5f, 1);
        newGameObject_ptr->AddGlobalVelocity(velocity);
        newGameObject_ptr->SetDrag(drag);
        newGameObject_ptr->SetProjectileType(0);
        newGameObject_ptr->SetFireFrequencey(0.69f);

        gameData_Ptr->PushToCollidableGameObjects(newGameObject_ptr);
        gameData_Ptr->IncrementEnemies();
    }

    void Game::SpawnSniperEnemy(glm::vec3 position, glm::vec3 velocity, glm::vec2 drag)
    {
        GameObjectCollidableMortalEnemy* newGameObject_ptr = new GameObjectCollidableMortalEnemy(spriteGeometry_Ptr, &spriteShader, textures[2], gameData_Ptr, position, 180, 0.5f, 1);
        newGameObject_ptr->AddGlobalVelocity(velocity);
        newGameObject_ptr->SetDrag(drag);
        newGameObject_ptr->SetProjectileType(1);
        newGameObject_ptr->SetFireFrequencey(3.0f);
        newGameObject_ptr->SetTurnSpeed(100);
        newGameObject_ptr->SetTargetGameObject(gameData_Ptr->GetPlayerPtr());

        gameData_Ptr->PushToCollidableGameObjects(newGameObject_ptr);
        gameData_Ptr->IncrementEnemies();
    }

    void Game::SpawnBomberEnemy(glm::vec3 position, glm::vec3 velocity, glm::vec2 drag)
    {
        GameObjectCollidableMortalEnemy* newGameObject_ptr = new GameObjectCollidableMortalEnemy(spriteGeometry_Ptr, &spriteShader, textures[3], gameData_Ptr, position, 180, 1.5f, 3);
        newGameObject_ptr->SetScale(glm::vec2(3, 1));
        newGameObject_ptr->AddGlobalVelocity(velocity);
        newGameObject_ptr->SetDrag(drag);
        newGameObject_ptr->SetProjectileType(2);
        newGameObject_ptr->SetFireFrequencey(3.0f);

        gameData_Ptr->PushToCollidableGameObjects(newGameObject_ptr);
        gameData_Ptr->IncrementEnemies();
    }

    void Game::SpawnBossEnemy(glm::vec3 position, glm::vec3 velocity, glm::vec2 drag)
    {
        GameObjectCollidableMortalEnemy* newGameObject_ptr = new GameObjectCollidableMortalEnemy(spriteGeometry_Ptr, &spriteShader, textures[4], gameData_Ptr, position, 180, 0.1f, 6);
        newGameObject_ptr->SetScale(glm::vec2(5, 2));
        newGameObject_ptr->AddGlobalVelocity(velocity);
        newGameObject_ptr->SetDrag(drag);


       
        //port turret
        GameObjectCollidableMortalEnemy* portTurret_ptr = new GameObjectCollidableMortalEnemy(spriteGeometry_Ptr, &spriteShader, textures[5], gameData_Ptr, glm::vec3(0,0,0), 180, 0.5f, 3);
        portTurret_ptr->SetParrent(newGameObject_ptr);
        portTurret_ptr->SetLocalPosition(glm::vec3(-2, -0.5, 0));
        portTurret_ptr->SetProjectileType(1);
        portTurret_ptr->SetFireFrequencey(3.0f);
        portTurret_ptr->SetTurnSpeed(50);
        portTurret_ptr->SetTargetGameObject(gameData_Ptr->GetPlayerPtr());
        

        portTurret_ptr->AddToCollisionWhiteList(newGameObject_ptr);
        newGameObject_ptr->AddToCollisionWhiteList(portTurret_ptr);


        //chin turret
        GameObjectCollidableMortalEnemy* chinTurret_ptr = new GameObjectCollidableMortalEnemy(spriteGeometry_Ptr, &spriteShader, textures[6], gameData_Ptr, glm::vec3(0, 0, 0), 180, 0.5f, 3);
        chinTurret_ptr->SetParrent(newGameObject_ptr);
        chinTurret_ptr->SetLocalPosition(glm::vec3(0, -0.9, 0));
        chinTurret_ptr->SetProjectileType(0);
        chinTurret_ptr->SetFireFrequencey(0.4f);
        chinTurret_ptr->SetTurnSpeed(25);
        chinTurret_ptr->SetTargetGameObject(gameData_Ptr->GetPlayerPtr());
        

        chinTurret_ptr->AddToCollisionWhiteList(newGameObject_ptr);
        newGameObject_ptr->AddToCollisionWhiteList(chinTurret_ptr);


        //starboard turret
        GameObjectCollidableMortalEnemy* starTurret_ptr = new GameObjectCollidableMortalEnemy(spriteGeometry_Ptr, &spriteShader, textures[5], gameData_Ptr, glm::vec3(0, 0, 0), 180, 0.5f, 3);
        starTurret_ptr->SetParrent(newGameObject_ptr);
        starTurret_ptr->SetLocalPosition(glm::vec3(2, -0.5, 0));
        starTurret_ptr->SetProjectileType(1);
        starTurret_ptr->SetFireFrequencey(3.0f);
        starTurret_ptr->SetTurnSpeed(50);
        starTurret_ptr->SetTargetGameObject(gameData_Ptr->GetPlayerPtr());
        

        starTurret_ptr->AddToCollisionWhiteList(newGameObject_ptr);
        newGameObject_ptr->AddToCollisionWhiteList(starTurret_ptr);


        gameData_Ptr->PushToCollidableGameObjects(portTurret_ptr);
        gameData_Ptr->PushToCollidableGameObjects(starTurret_ptr); 
        gameData_Ptr->PushToCollidableGameObjects(newGameObject_ptr);
        gameData_Ptr->PushToCollidableGameObjects(chinTurret_ptr);
        
        gameData_Ptr->IncrementEnemies();
        gameData_Ptr->IncrementEnemies();
        gameData_Ptr->IncrementEnemies();
        gameData_Ptr->IncrementEnemies();
    }

    //spawners for different type of collectable
    void Game::SpawnPowerUpHealth(glm::vec3 position, glm::vec3 velocity)
    {
        GameObjectCollideableCollectable* newCollectable = new GameObjectCollideableCollectable(spriteGeometry_Ptr, &spriteShader, textures[27], gameData_Ptr, position, 0, 1, 0);
        newCollectable->AddGlobalVelocity(velocity);
        gameData_Ptr->PushToGameObjects(newCollectable);
    }

    void Game::SpawnPowerUpMultiShot(glm::vec3 position, glm::vec3 velocity)
    {
        GameObjectCollideableCollectable* newCollectable = new GameObjectCollideableCollectable(spriteGeometry_Ptr, &spriteShader, textures[28], gameData_Ptr, position, 0, 1, 1);
        newCollectable->AddGlobalVelocity(velocity);
        gameData_Ptr->PushToGameObjects(newCollectable);
    }

    void Game::SpawnPowerUpBigShot(glm::vec3 position, glm::vec3 velocity)
    {
        GameObjectCollideableCollectable* newCollectable = new GameObjectCollideableCollectable(spriteGeometry_Ptr, &spriteShader, textures[29], gameData_Ptr, position, 0, 1, 2);
        newCollectable->AddGlobalVelocity(velocity);
        gameData_Ptr->PushToGameObjects(newCollectable);
    }

    void Game::UpdateHealthUI() {

        if (player->GetAlive() && gameData_Ptr->GetPlayerPtr() != NULL) {
            if (player->GetHealth() >= 3) {

                gameData_Ptr->GetHealth()->SetTexture(textures[26]);
                gameData_Ptr->GetHealth()->SetScale(glm::vec2(3.33333333333f,1.0f));

            }
            else if (player->GetHealth() == 2) {
                gameData_Ptr->GetHealth()->SetTexture(textures[25]);
                gameData_Ptr->GetHealth()->SetScale(glm::vec2(3.33333333333f, 1.0f));
            }
            else if (player->GetHealth() == 1)
            {
                gameData_Ptr->GetHealth()->SetTexture(textures[24]);
                gameData_Ptr->GetHealth()->SetScale(glm::vec2(3.33333333333f, 1.0f));
            }
            else
            {
                gameData_Ptr->GetHealth()->SetTexture(textures[23]);
            }
        }
    }

    /*
    //instances a Projectile into the game 
    void InstantiateProjectile(GameObjectCollidableProjectile* p_newProjectile)
    {
        std::cout << "new Projectile go BRRRRR!!!!" << std::endl;
    }
    */
       
} // namespace game
