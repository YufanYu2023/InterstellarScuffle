#include "GameObject_Collidable.h"
#include "GameObject_Collidable_Mortal_Player.h"

namespace game{
    class GameObjectCollideableCollectable : public GameObjectCollidable
    {
    public:
        GameObjectCollideableCollectable(Geometry* geometry_Ptr, Shader* shader_Ptr, GLuint texture, GameData* gameData_Ptr, const glm::vec3& position, const float& rotation, float collisionRadius, int type);

 
        void OnCollision() override;

    private:
        int type;
    };
}