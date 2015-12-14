#ifndef NPC106_H_INCLUDED
#define NPC106_H_INCLUDED

#include "NPC.h"
#include <vector>

class NPC106 : public NPC {
    private:
        NPC106();
        ~NPC106();
        bool chasingPlayer = false;
        unsigned int rListIndex = 0;
        unsigned int wpListIndex = 0;
        irr::core::vector2di currRoom;
        std::vector<irr::core::vector2di> roomList;
        std::vector<irr::core::vector3df> waypointList;
        float currAngle = 0.f;

        float searchTimer = 0.f;

        irr::core::vector3df speed;
    public:
        irr::scene::ISceneNode* boxNode;
        static irr::scene::IAnimatedMeshSceneNode* baseNode;
        virtual void update();
        virtual void updateModel();
        static NPC106* createNPC106();
        virtual void teleport(irr::core::vector3df newPos);
        virtual irr::core::vector3df getPosition() { return node->getPosition(); };
};

#endif // NPC106_H_INCLUDED
