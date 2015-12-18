#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "KinematicCharacterController/KinematicCharacterController.h"
#include "GameObject.h"

const unsigned int inventory_size = 10;

class Player : public GameObject {
    private:
        btVector3 linearVelocity;
        float walkingSpeed;
        //irr::scene::ICameraSceneNode* Camera;
//#if 0
        btRigidBody* capsule;
//#endif
        btCollisionShape* standingShape;
        btCollisionShape* crouchingShape;
//#if defined(PLAYER_PENETRATION_RECOVER)
        btPairCachingGhostObject* ghostObject;
        CharacterController* controller;
//#endif
        irr::core::vector3df selfRotation;

        Item* inventory[inventory_size];
        unsigned char selectedItem = inventory_size;
        unsigned char prevSelectedItem = inventory_size;
        bool temporarySelection = false;
        unsigned char wearingGasMask = inventory_size;
        unsigned char wearing714 = inventory_size;

		float height,radius;

        float crouchState = 0.f;
        bool crouched = false;

        bool dead = false;
        irr::core::vector3df forceDir; float playerControl = 1.f;

        float sprintTimer = 0;

        Sound* breathSound[5][2];
        unsigned char currBreathSound = 4;
        Sound* stepSound[4][2][4]; //(normal/metal/pocket dimension/forest, walk/run, id)
        float shakeFactor = 0.f;

        float shake = 0.f;

        btVector3 dynSpeed = btVector3(0.f,0.f,0.f);
        float dir = 0;

        bool lastMouseDown[2];

        bool recoverFromPenetration();

        irr::core::vector3df tPos,tTarget,tUpVec;
    public:

        irr::scene::ICameraSceneNode* camera;

        Player(World* own,irr::scene::ISceneManager* smgr,irrDynamics* dyn,MainEventReceiver* receiver,float height=24.1f,float radius=3.2f,float mass=5.0f);
        //mass should stay low if you want the Player to be able the climb up stairs
        ~Player();
        void update(); void resetSpeeds(); void updateHead();

        void resetCam();
        void reflectNX(); //-X
        void reflectPX(); //+X
        void reflectNY(); //-Y
        void reflectPY(); //+Y
        void reflectNZ(); //-Z
        void reflectPZ(); //+Z

        float yaw = 0.f,pitch = 0.f,roll = 0.f;

        float blinkTimer=100.0,stamina=100.0;

        bool addToInventory(Item* it);
        Item* takeFromInventory(unsigned char slot,bool rmove=true);
        unsigned char moveToSlot(unsigned char srcSlot,unsigned char destSlot);

        void selectItem(unsigned char index,bool temporary=false);
        void selectGasMask(Item* it);
        void select714(Item* it);
        bool drawSelectedItem();

        void setVisible(bool isVisible);

        void booststamina(float add,float clamp);

        void teleport(irr::core::vector3df position);

        bool seesMeshNode(irr::scene::IMeshSceneNode* node);
        bool seesBoundingBox(irr::core::aabbox3df bbox,irr::scene::SViewFrustum frust);

        irr::scene::IMeshSceneNode* testNode;

        irr::core::vector3df getPosition() {
            return btToIrrVec(controller->getPosition());//irr::core::vector3df();//capsule->getCenterOfMassPosition()[0],capsule->getCenterOfMassPosition()[1],capsule->getCenterOfMassPosition()[2]);
        }

        bool getLastMouseDown(unsigned char index) {
			return lastMouseDown[index];
		}

		const irr::scene::SViewFrustum* getViewFrustum();

		/*std::string getItemName(unsigned char slot);
		std::string getItemInvImg(unsigned char slot);
		ItemTempIDs getItemTempID(unsigned char slot);*/
		unsigned char getSelectedItem();
		void selectPrevItem();

		bool noclip = false;

		void forceWalk(irr::core::vector3df inForcedDir,float inPlayerControl);
};

#endif // PLAYER_H_INCLUDED
