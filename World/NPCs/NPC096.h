#ifndef NPC096_H_INCLUDED
#define NPC096_H_INCLUDED

#include "NPC.h"
#include <vector>

#include "../../Sound/SoundWrapper.h"

class NPC096 : public NPC {
    private:
        NPC096();
        ~NPC096();
        //static class btCollisionShape* shape;
        //static irr::scene::IAnimatedMeshSceneNode* baseNode; //USE THIS
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

        Sound* screamLoop;
        unsigned char screamChannel;

    public:
        static class btCollisionShape* shape;
        irr::scene::ISceneNode* boxNode;
        static irr::scene::IAnimatedMeshSceneNode* baseNode; //REMOVE THIS
        virtual void update();
        virtual void updateModel();
        static void setBase(class btCollisionShape* inShape,irr::scene::IMeshSceneNode* inNode);
        static NPC096* createNPC096();
        virtual void teleport(irr::core::vector3df newPos);
        virtual irr::core::vector3df getPosition() { return node->getPosition(); };
};

#endif // NPC096_H_INCLUDED
