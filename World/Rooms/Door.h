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

        irr::core::vector3df closedShift;
        irr::core::vector3df openShift;

        bool isSolid = true;

        unsigned char doorType;
        unsigned char keycard;
        short code;
        unsigned char scanner;

        unsigned char buttonIndex;
    public:
        virtual void update();
        virtual void disable();
        virtual void forceToggle();
        virtual void toggle(unsigned char inKeycard,short inCode,unsigned char inScanner);
        virtual irr::core::vector3df getPosition();
        float getRotation();
        virtual irr::core::vector3df getButtonPosition(unsigned char index);
        float getButtonRotation(unsigned char index);
        unsigned char getButtonIndex();
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

        static class btBoxShape* shape;

        static class Sound* openSound[2][3];
        static class Sound* closeSound[2][3];

        static Door* createDoor(unsigned char inDoorType=0,unsigned char inKeycard=0,short inCode=0,unsigned char inScanner=0);

        float openState = 0.f;
        bool open = true;
};

#endif // DOOR_H_INCLUDED
