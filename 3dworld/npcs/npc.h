#ifndef NPC_H_INCLUDED
#define NPC_H_INCLUDED

#include <irrlicht.h>

class npc {
    private:
        npc();
        ~npc();
    protected:
        irr::scene::ISceneNode* node;
        class btRigidBody* collider;
        static class world* owner;
    public:
        virtual void update() =0;
};

#endif // NPC_H_INCLUDED
