#ifndef NPC173_H_INCLUDED
#define NPC173_H_INCLUDED

#include "NPC.h"
#include <vector>

#include "../../Sound/SoundWrapper.h"

class NPC173 : public NPC {
    private:
        NPC173();
        ~NPC173();
        //static class btCollisionShape* shape;
        //static irr::scene::IAnimatedMeshSceneNode* baseNode; //USE THIS
        bool chasingPlayer = false;
        unsigned int rListIndex = 0;
        unsigned int wpListIndex = 0;
        irr::core::vector2di currRoom;
        std::vector<irr::core::vector2di> roomList;
        std::vector<irr::core::vector3df> waypointList;
        float currAngle = 0.f;

        float searchTimer = 0.f;

        irr::core::vector3df memDir;
        irr::core::vector3df moveDir;

        irr::scene::ISceneNode* occlusionNode;
        //irr::core::vector3df prevPos[2];
    public:
        static class btCollisionShape* shape;
        irr::scene::ISceneNode* boxNode;
        static irr::scene::IMeshSceneNode* baseOcclusionNode;
        static irr::scene::IMeshSceneNode* baseNode;
        virtual void update();
        virtual void updateModel();
        static NPC173* createNPC173();
        virtual void teleport(irr::core::vector3df newPos);
        virtual irr::core::vector3df getPosition() { return node->getPosition(); };
};

#endif // NPC173_H_INCLUDED
