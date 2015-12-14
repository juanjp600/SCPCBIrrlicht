#ifndef NPC_H_INCLUDED
#define NPC_H_INCLUDED

#include <irrlicht.h>
#include "../GameObject.h"

class NPC : public GameObject {
    protected:
        NPC();
        ~NPC();
        irr::scene::ISceneNode* node;
        class btRigidBody* collider;
        //static class World* owner;
    public:
        virtual void update() =0;
        virtual void updateModel() =0;
        virtual void teleport(irr::core::vector3df newPos) =0;
        virtual irr::core::vector3df getPosition() =0;
};

#endif // NPC_H_INCLUDED
