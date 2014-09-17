#ifndef ITEMGASMASK_H_INCLUDED
#define ITEMGASMASK_H_INCLUDED

#include "../items.h"

class itemGasMask : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;
        static dynRegister* dynamics;

        itemGasMask();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_GASMASK;
        }

        bool updateItem() {
            return false;
        }

        std::string getName() {
            return "Eyedrops";
        }

        //IMesh*,ISceneNode* parent=0,s32 id = -1,const core::vector3df& position = core::vector3df(0,0,0),
        //const core::vector3df& rotation = core::vector3df(0,0,0),const core::vector3df& scale = core::vector3df(0,0,0),bool alsoAddIfMeshPointerZero = false

        static itemGasMask* createItemGasMask();

        static void setMeshNode(irr::scene::IMeshSceneNode* &node);
        static void setDynamics(dynRegister* dyn);

        void Pick();
        void Unpick(irr::core::vector3df position);
};

#endif // ITEMGASMASK_H_INCLUDED
