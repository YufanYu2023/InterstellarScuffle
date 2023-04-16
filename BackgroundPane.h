#ifndef BACKGROUNDPANE_H_
#define BACKGROUNDPANE_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "geometry.h"

namespace game
{
    class BackGroundPane
    {
        public:
            BackGroundPane(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, const float& scale, const glm::vec3& startPosition, const float& downwardVelocity, const float& resetHight, const float& desetHight);

            virtual void Update(double delta_time);
            virtual void Render(glm::mat4 view_matrix, double current_time);

            inline glm::vec3& GetPosition(void) { return position; }
            inline glm::vec3& GetVelocity(void) { return velocity; }
 
            inline void SetPosition(const glm::vec3& newPosition) { position = newPosition; }
            inline void SetVelocity(const glm::vec3& newVelocity) { velocity = newVelocity; }


        protected:
            Geometry* geometry_Ptr;
            Shader* shader_Ptr;
            GLuint texture;

            glm::vec3 position;
            glm::vec3 velocity;
            glm::vec2 scale;

            float rotation;

            float resetHight; 
            float desetHight;
    };
}

#endif 