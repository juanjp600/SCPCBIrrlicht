#ifndef NPC178_H_INCLUDED
#define NPC178_H_INCLUDED

#include "NPC.h"
#include <vector>

class NPC178 : public NPC {
    private:
        NPC178();
        ~NPC178();
        bool chasingPlayer = false;
        unsigned int rListIndex = 0;
        //unsigned int rShift = 0;
        //unsigned int rDist = 0;
        //unsigned char rDir = 0; //0 = right, 1 = up
        unsigned int wpListIndex = 0;
        irr::core::vector2di currRoom;
        std::vector<irr::core::vector2di> roomList;
        std::vector<irr::core::vector3df> waypointList;
        float currAngle = 0.f;

        float searchTimer = 0.f;

        irr::core::vector3df memDir;
        char skipStuckCheck = 0;
        irr::core::vector3df prevPos;
        int walkTimer = 300.f;
        int minWalkTimer = -700;
        int spinTimer = -100;
    public:
        static class btCollisionShape* shape;
        irr::scene::ISceneNode* boxNode;
        static irr::scene::IAnimatedMeshSceneNode* baseNode;
        virtual void update();
        virtual void updateModel();
        static NPC178* createNPC178();
        virtual void teleport(irr::core::vector3df newPos);
        virtual irr::core::vector3df getPosition();
};

#endif // NPC178_H_INCLUDED
