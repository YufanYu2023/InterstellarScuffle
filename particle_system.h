#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include "GameObject.h"

namespace game {

    // Inherits from GameObject
    class ParticleSystem : public GameObject {

        public:
            ParticleSystem(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation);

            void Update(double delta_time, GLFWwindow* p_glfwWindow) override;

            void Render(glm::mat4 view_matrix, double current_time);

    }; // class ParticleSystem

} // namespace game

#endif // PARTICLE_SYSTEM_H_
