#ifndef NPC_H_INCLUDED
#define NPC_H_INCLUDED

#include <irrlicht.h>

class npc {
    protected:
        npc();
        ~npc();
        irr::scene::ISceneNode* node;
        class btRigidBody* collider;
        //static class world* owner;
    public:
        virtual void update() =0;
        virtual void teleport(irr::core::vector3df newPos) =0;
        virtual irr::core::vector3df getPosition() =0;
        static class world* owner;
};

#endif // NPC_H_INCLUDED
