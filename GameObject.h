#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "geometry.h"
#include <vector>

#include "GameData.h"

namespace game 
{
    //!!The Compiler will not tell you if something is wong with a forward declaration!!
    class GameData;// forward declaration

    /*
    *   GameObject(Geometry*, Shader*, GLuint, GameData*, glm::vec3, float);
    * 
    *   GameObject is responsible for handling the rendering and updating of one object in the game world
    *   The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject 
    {
        public:
            // Constructor
            GameObject(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float rotation);

            virtual GameObject* GetAddress() { return this; }
            
            virtual void Update(double delta_time, GLFWwindow* p_glfwWindow);// Update the GameObject's state. Can be overriden in children
            virtual void UpdatePosition(double delta_time);
            virtual void UpdateRotation(double delta_time);

            virtual void BoundsCheck(float maxXpostion, float maxYpostion);

            void SetParrent(GameObject* p_newParrent);
            inline void addChild(GameObject* p_newChild) { children_Ptr.push_back(p_newChild); }

            // Renders the GameObject 
            virtual void Render(glm::mat4 view_matrix, double current_time);

            
            inline bool GetAlive() { return alive; }//used to communicate that this gameObject should be removed from the game 
            //inline int GetType() { return 0; }//used to communicate that this gameObject is of a given type, (0->Default, 1->Collidable)


            inline glm::vec3& GetPosition(void) { return position; }
            //inline glm::vec3& GetMovementVector(void) { return velocityDelta; }
            inline float GetRotation(void) { return rotation; }
            glm::vec3 GetForwardVector();
            glm::vec3 GetRightwardVector();

        
            inline void SetPosition(const glm::vec3& newPosition) { position = newPosition; }
            inline void SetLocalPosition(const glm::vec3& newlocalPosition) { localPosition = newlocalPosition; }
            inline void SetVelocity(const glm::vec3& newVelocity) { velocity = newVelocity; }
            //inline void SetMovementVector(const glm::vec3& newMovementVector) { velocityDelta = newMovementVector; }
            inline void SetAlive(bool newAlive) { alive = newAlive; }
            inline void SetScale(glm::vec2 newScale) { scale = newScale; }
            inline void SetDrag(glm::vec2 newDrag) { drag = newDrag; }
            inline void SetRotation(float newRotation) { rotation = newRotation; }
            inline void SetAngularDrag(float newAngularDrag) { angularDrag = newAngularDrag; }
            inline void SetAngularVelocity(float newAngularVelocity) { angularVelocity = newAngularVelocity; }


            inline void SetTexture(GLuint newTexture) { texture = newTexture; }

            void AddGlobalVelocity(glm::vec3 addedGlobalVelocity);
            void AddLocalVelocity(glm::vec3 addedLocalVelocity);            
            void AddAngularVelocity(float addedAngularVelocity); 



            //Helper
            glm::vec3 MoveTowards(glm::vec3 current, glm::vec3 goal, float maxDelta);
            float MoveTowards(float current, float goal, float maxDelta);




        protected:
            GameData* gameData_Ptr; 
            GameObject* parrent_Ptr;
            std::vector<GameObject*> children_Ptr;
            
            bool alive;//used to communicate that this gameObject should be removed from the game 

            Geometry* geometry_Ptr;
            Shader* shader_Ptr;
            GLuint texture;

            glm::vec2 scale;

            glm::vec3 position;
            glm::vec3 localPosition;
            glm::vec3 velocity;
            glm::vec2 drag;

            float rotation;
            float angularVelocity;
            float angularDrag;     
    };
} 

#endif 