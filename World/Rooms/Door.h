#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED

#include <irrlicht.h>

class Door {
    private:
        Door();
        ~Door();
        irr::scene::ISceneNode* doorNode1;
        irr::scene::ISceneNode* doorNode2;
        irr::scene::ISceneNode* buttonNode1;
        irr::scene::ISceneNode* buttonNode2;
        irr::scene::ISceneNode* frameNode;
        class btRigidBody* collider;
        //static class World* owner;

        unsigned char type;
    public:
        virtual void update();
        virtual void forceToggle();
        virtual irr::core::vector3df getPosition();
        virtual void setPosition(irr::core::vector3df newPos);
        virtual void setRotation(float newAngle);
        static class World* owner;
        static class irrDynamics* dynamics;
        static class Player* player;

        static irr::scene::IMeshSceneNode* baseFrameNode;
        //0 = standard, 1 = hcz doors, 2 = blast doors
        static irr::scene::IMeshSceneNode* baseDoorNode[3];
        //0 = button, 1 = keycard slot, 2 = keypad, 3 = fingerprint scanner
        static irr::scene::IMeshSceneNode* baseButtonNode[4];

        static Door* createDoor(unsigned char doorType=0,unsigned char keycard=0,short code=0,unsigned char scanner=0);
};

#endif // DOOR_H_INCLUDED
