#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "particle_system.h"

namespace game {

    ParticleSystem::ParticleSystem(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation)
    : GameObject(geometry_Ptr, shader_Ptr, texture, gameData_Ptr, position, rotation)
    {

    }


    void ParticleSystem::Update(double delta_time, GLFWwindow* p_glfwWindow) 
    {
        if (this->parrent_Ptr == NULL) { alive = false; }

        //std::cout << "ahh" << std::endl;

	    // Call the parent's update method to move the object in standard way, if desired
	    //GameObject::Update(delta_time, p_glfwWindow);
    }

    
    void ParticleSystem::Render(glm::mat4 view_matrix, double current_time)
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
        if (this->parrent_Ptr != NULL)
        {
            glm::mat4 parent_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(this->parrent_Ptr->GetRotation()), glm::vec3(0.0, 0.0, 1.0));
            glm::mat4 parent_translation_matrix = glm::translate(glm::mat4(1.0f), this->parrent_Ptr->GetPosition());
            parent_transformation_matrix = parent_translation_matrix * parent_rotation_matrix;
        }

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
    

} // namespace game
