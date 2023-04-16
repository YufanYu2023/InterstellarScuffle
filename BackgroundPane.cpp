#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "BackGroundPane.h"

namespace game {

    BackGroundPane::BackGroundPane(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, const float& scale, const glm::vec3& startPosition, const float& downwardVelocity, const float& resetHight, const float& desetHight)
    {
        this->geometry_Ptr = geometry_Ptr;
        this->shader_Ptr = shader_Ptr;
        this->texture = texture;

        this->position = startPosition;
        this->velocity = glm::vec3(0.0f, downwardVelocity, 0.0f);
        this->scale = glm::vec2(scale, scale);

        this->rotation = 0; 

        this->resetHight = resetHight;
        this->desetHight = desetHight;
    }

    void BackGroundPane::Update(double delta_time)
    {
        position = position + (velocity * (float)delta_time);

        if(position.y < desetHight)
        {
            position.y = resetHight;           
        }
    }

    void BackGroundPane::Render(glm::mat4 view_matrix, double current_time)
    {
        // Set up the shader
        shader_Ptr->Enable();

        // Set up the view matrix
        shader_Ptr->SetUniformMat4("view_matrix", view_matrix);

        // Set up the this's transformation matrix
        glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), this->position);// Set up the translation matrix for the shader 
        glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(this->scale.x, this->scale.y, 1.0));// Setup the scaling matrix for the shader
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation), glm::vec3(0.0, 0.0, 1.0));// Set up the rotation matrix for the shader

        // Setup the transformation matrix for the shader
        glm::mat4 transformation_matrix = translation_matrix * scaling_matrix * rotation_matrix;

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
} // namespace game
