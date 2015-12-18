#ifndef NPC513_H_INCLUDED
#define NPC513_H_INCLUDED

#include "NPC.h"
#include <vector>

class NPC513 : public NPC {
    private:
        NPC513();
        ~NPC513();
        bool chasingPlayer = false;
        unsigned int rListIndex = 0;
        unsigned int wpListIndex = 0;
        irr::core::vector2di currRoom;
        std::vector<irr::core::vector2di> roomList;
        std::vector<irr::core::vector3df> waypointList;
        float currAngle = 0.f;

        float searchTimer = 0.f;

        irr::core::vector3df speed;
        float currFrame = 0.f;
    public:
        static irr::scene::IAnimatedMeshSceneNode* baseNode;
        virtual void update();
        virtual void updateModel();
        static NPC513* createNPC513();
        virtual void teleport(irr::core::vector3df newPos);
        virtual irr::core::vector3df getPosition() { return node->getPosition(); };
};

#endif // NPC513_H_INCLUDED
