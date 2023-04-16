#ifndef VECTORMATH_H_
#define VECTORMATH_H_

#include <glm/glm.hpp>

namespace game
{
    class VectorMath 
    {
        public:
            VectorMath();
            /* returns the magnitude (length) of the given vector */
            static float Magnitude(glm::vec3 vector);
            /* returns the normalized vector of the given vector */
            static glm::vec3 Normalize(glm::vec3 vector);
            /* returns the clamped vector of the given vector */
            static glm::vec3 Clamp(glm::vec3 vector, float maxMagnitude);

            static float Dot(glm::vec3 vectorA, glm::vec3 vectorB);

            static glm::vec3 VectorFromAtoB(glm::vec3 vectorA, glm::vec3 vectorB);

            static float AngleFormAtoB(glm::vec3 vectorA, glm::vec3 vectorB);

            /* returns vectorA moved towards vectorB */
            static glm::vec3 MoveATowardsB(glm::vec3 vectorA, glm::vec3 vectorB, float maxMoveMagnitude);



            /*
            returns the crossProduct vector of the given two vectors 
            Note: for 2D operations the vectorB should just be (0,0,1)
            */
            static glm::vec3 CrossProduct(glm::vec3 vectorA, glm::vec3 vectorB);

    };

} 
#endif 
