#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED

#include <irrlicht.h>
#include <irrDynamics.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <string>

enum class itemTempIDs : unsigned short {
    ITEM_EYEDROPS,
    ITEM_GASMASK
};

class world;

class item {
    protected:
        irr::scene::ISceneNode* irrObj;
        btRigidBody* rbody;
        bool picked = true;
        float state[3];
        static dynRegister* dynamics;
    public:
        virtual bool updateItem() =0;
        virtual std::string getName() =0;
        virtual itemTempIDs getTempID() =0;

        virtual float getState(int i) {
            return state[i];
        }
        virtual bool getPicked() {
            return picked;
        }

        virtual void Pick() =0;
        virtual void Unpick(irr::core::vector3df position) =0;

        static void setDynamics(dynRegister* dyn);

        item();
        ~item();
};

#include "itemEyedrops/itemEyedrops.h"
#include "itemGasMask/itemGasMask.h"

#endif // ITEMS_H_INCLUDED
