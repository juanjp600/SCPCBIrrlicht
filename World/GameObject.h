#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED

#include <irrlicht.h>

class GameObject {
    protected:
        static class World* owner;
        static class irrDynamics* dynamics;
        static irr::scene::ISceneManager* irrSmgr;
        static irr::video::IVideoDriver* irrDriver;
        static class MainEventReceiver* irrReceiver;
        static class Player* mainPlayer;
        static unsigned short screenWidth; static unsigned short screenHeight;
};

#endif // GAMEOBJECT_H_INCLUDED
