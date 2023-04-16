#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "GameObject.h"
#include "VectorMath.h"

namespace game {

    GameObject::GameObject(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float rotation)
    {
        // Initialize all attributes
        this->gameData_Ptr = gameData_Ptr;
        this->parrent_Ptr = NULL;

        this->alive = true;

        this->geometry_Ptr = geometry_Ptr;
        this->shader_Ptr = shader_Ptr;
        this->texture = texture;

        this->scale = glm::vec2(1,1);//hmmmm

        this->position = position;
        this->localPosition = glm::vec3(0,0,0);
        this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        this->drag = glm::vec2(0.0f, 0.0f);

        this->rotation = rotation;
        this->angularVelocity = 0;
        this->angularDrag = 0.5f; 
    }


    void GameObject::Update(double delta_time, GLFWwindow* p_glfwWindow) 
    {
        UpdatePosition(delta_time);
        UpdateRotation(delta_time);
    }

    void GameObject::UpdatePosition(double delta_time) 
    {
        //velocity = VectorMath::MoveATowardsB(velocity, glm::vec3(0, 0, 0), drag * (float)delta_time);
        velocity.x = MoveTowards(velocity.x, 0, (drag.x * (float)delta_time));
        velocity.y = MoveTowards(velocity.y, 0, (drag.y * (float)delta_time));
        //AddGlobalVelocity(VectorMath::Normalize(-velocity) * (drag * (float)delta_time));


        if(parrent_Ptr == NULL)
        {
            position = position + (velocity * (float)delta_time);
        }
        else
        {
            this->position = parrent_Ptr->GetPosition() + this->localPosition;
        }
    }
    
    void GameObject::UpdateRotation(double delta_time)
    {
        //angularVelocity = MoveTowards(angularVelocity, 0, (angularDrag * (float)delta_time));
        angularVelocity = MoveTowards(angularVelocity, 0, 0);
        rotation = rotation + angularVelocity;
    }

    void GameObject::BoundsCheck(float maxXpostion, float maxYpostion)
    {
        if((this->position.x > maxXpostion) || (this->position.x < -maxXpostion))//check if we are out of bounds 
        {
            this->velocity = glm::vec3(-this->velocity.x, this->velocity.y, 0);
        }

        /*
        if ((this->position.y > maxYpostion) || (this->position.y < -maxYpostion))//check if we are out of bounds 
        {
            this->velocity = glm::vec3(this->velocity.x, -this->velocity.y, 0);
        }
        */
    }

    void GameObject::SetParrent(GameObject* p_newParrent) 
    { 
        parrent_Ptr = p_newParrent; 

        this->SetPosition(this->GetPosition() + parrent_Ptr->GetPosition());
    }


    void GameObject::Render(glm::mat4 view_matrix, double current_time) 
    {
        // Set up the shader
        shader_Ptr->Enable();

        // Set up the view matrix
        shader_Ptr->SetUniformMat4("view_matrix", view_matrix);
        
        // Set up the this's transformation matrix
        glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), this->position);// Set up the translation matrix for the shader 
        glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(this->scale.x, this->scale.y, 1.0));// Setup the scaling matrix for the shader
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation), glm::vec3(0.0, 0.0, 1.0));// Set up the rotation matrix for the shader

        // Set up the parents transformation matrix
        glm::mat4 parent_transformation_matrix = glm::mat4(1.0f);
        /*
        if(this->parrent_Ptr != NULL)
        {
            glm::mat4 parent_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(this->parrent_Ptr->GetRotation()), glm::vec3(0.0, 0.0, 1.0));
            glm::mat4 parent_translation_matrix = glm::translate(glm::mat4(1.0f), this->parrent_Ptr->GetPosition());
            parent_transformation_matrix = parent_translation_matrix * parent_rotation_matrix;
        }
        */
        
        // Setup the transformation matrix for the shader
        glm::mat4 transformation_matrix = parent_transformation_matrix * translation_matrix * scaling_matrix * rotation_matrix;

        // Set the transformation matrix in the shader
        shader_Ptr->SetUniformMat4("transformation_matrix", transformation_matrix);

        // Set the time in the shader
        shader_Ptr->SetUniform1f("time", current_time);

        //shader_->SetUniform1f("katuration", 1);

        // Set up the geometry
        geometry_Ptr->SetGeometry(shader_Ptr->GetShaderProgram());

        // Bind the entity's texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // Draw the entity
        glDrawElements(GL_TRIANGLES, geometry_Ptr->GetSize(), GL_UNSIGNED_INT, 0);
    }

    void GameObject::AddGlobalVelocity(glm::vec3 addedGlobalVelocity) { velocity = (velocity + addedGlobalVelocity); }
    void GameObject::AddLocalVelocity(glm::vec3 addedLocalVelocity)//1:y -> up, 1:x -> right 
    {
        glm::vec3 addedLocalVelocityForwardComponent = (GetForwardVector() * addedLocalVelocity.y); 
        glm::vec3 addedLocalVelocityRightComponent = (GetRightwardVector() * addedLocalVelocity.x);
        velocity = (velocity + addedLocalVelocityForwardComponent + addedLocalVelocityRightComponent);
    }

    void GameObject::AddAngularVelocity(float addedAngularVelocity)
    {
        angularVelocity = angularVelocity + addedAngularVelocity;
    }



    glm::vec3 GameObject::GetForwardVector()
    {
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
        glm::vec4 normalVector = glm::vec4(0, 1, 0, 0);
        glm::vec4 rotatedNormalVector = normalVector * rotation_matrix;
        return glm::vec3(-rotatedNormalVector.x, rotatedNormalVector.y, rotatedNormalVector.z);
    }


    glm::vec3 GameObject::GetRightwardVector()
    {
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation + 90), glm::vec3(0.0, 0.0, 1.0));
        glm::vec4 normalVector = glm::vec4(0, 1, 0, 0);
        glm::vec4 rotatedNormalVector = normalVector * rotation_matrix;
        return glm::vec3(-rotatedNormalVector.x, rotatedNormalVector.y, rotatedNormalVector.z);
    }

    glm::vec3 GameObject::MoveTowards(glm::vec3 current, glm::vec3 goal, float maxDelta)
    {
        glm::vec3 result = glm::vec3(0, 0, 0);
        result.x = -MoveTowards(current.x, goal.x, maxDelta);
        result.y = -MoveTowards(current.y, goal.y, maxDelta);
        result.z = -MoveTowards(current.z, goal.z, maxDelta);
        return result;
    }

    float GameObject::MoveTowards(float current, float goal, float maxDelta)
    {
        int polarity = 1;//1: we need to step up, -1:we need to step down
        if (current < goal) { polarity = -1; }

        //step towards the goal 
        float result = current - (maxDelta * polarity);

        float highFence = result;
        float lowFence = current;
        if (polarity == -1)
        {
            highFence = current;
            lowFence = result;
        }

        
        if ((lowFence <= goal) && (goal <= highFence))//check if we overshot the goal
        { 
            return goal;//if so, return goal
        }
        else
        {
            return result;//if not, return current moved towards goal 
        }
    }

} // namespace game
