#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

const unsigned int inventory_size = 10;

class player {
    private:
        world* owner;
        irr::scene::ISceneManager* irrSmgr;
        irrDynamics* dynamics;
        MainEventReceiver* irrReceiver;
        btVector3 prevLinearVelocity;
        float walkingSpeed;
        irr::scene::ICameraSceneNode* Camera;
        btRigidBody* Capsule;
#if defined(PLAYER_PENETRATION_RECOVER)
        btPairCachingGhostObject* ghostObject;
#endif
        irr::core::vector3df selfRotation;

        item* inventory[inventory_size];

		float height,radius;

        float crouchState = 0.f;
        bool crouched = false;

        bool dead = false;

        float sprintTimer = 0;

        sound* breathSound[5][2];
        unsigned char currBreathSound = 4;
        sound* stepSound[4][2][4]; //(normal/metal/pocket dimension/forest, walk/run, id)
        float shakeFactor = 0.f;

        float shake = 0.f;

        btVector3 dynSpeed = btVector3(0.f,0.f,0.f);
        float dir = 0;

        bool lastMouseDown[2];

        bool recoverFromPenetration();

        irr::core::vector3df tPos,tTarget,tUpVec;
    public:
        player(world* own,irr::scene::ISceneManager* smgr,irrDynamics* dyn,MainEventReceiver* receiver,float height=24.1f,float radius=3.2f,float mass=5.0f);
        //mass should stay low if you want the player to be able the climb up stairs
        ~player();
        void update(); void resetSpeeds(); void updateHead();

        void resetCam();
        void reflectNX(); //-X
        void reflectPX(); //+X
        void reflectNY(); //-Y
        void reflectPY(); //+Y
        void reflectNZ(); //-Z
        void reflectPZ(); //+Z

        float yaw = 0.f,pitch = 0.f,roll = 0.f;

        float BlinkTimer=100.0,Stamina=100.0;

        bool addToInventory(item* it);
        item* takeFromInventory(unsigned char slot);
        unsigned char moveToSlot(unsigned char srcSlot,unsigned char destSlot);

        void teleport(irr::core::vector3df position);

        bool seesMeshNode(irr::scene::IMeshSceneNode* node);
        bool seesBoundingBox(irr::core::aabbox3df bbox,irr::scene::SViewFrustum frust);

        irr::scene::IMeshSceneNode* testNode;

        irr::core::vector3df getPosition() {
            return irr::core::vector3df(Capsule->getCenterOfMassPosition()[0],Capsule->getCenterOfMassPosition()[1],Capsule->getCenterOfMassPosition()[2]);
        }

        bool getLastMouseDown(unsigned char index) {
			return lastMouseDown[index];
		}

		const irr::scene::SViewFrustum* getViewFrustum();

		std::string getItemName(unsigned char slot);
		std::string getItemInvImg(unsigned char slot);

		bool noclip = false;
};

#endif // PLAYER_H_INCLUDED
