#ifndef NPC096_H_INCLUDED
#define NPC096_H_INCLUDED

#include "npc.h"

class npc096 : public npc {
    private:
        npc096();
        ~npc096();
        static class btCollisionShape* shape;
        static irr::scene::IMeshSceneNode* baseNode;
        bool chasingPlayer = false;
    public:
        virtual void update();
        static void setBase(class btCollisionShape* inShape,irr::scene::IMeshSceneNode* inNode);
};

#endif // NPC096_H_INCLUDED
