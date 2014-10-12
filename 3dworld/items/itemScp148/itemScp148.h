#ifndef ITEMSCP148_H_INCLUDED
#define ITEMSCP148_H_INCLUDED

#include "../items.h"

class itemScp148 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemScp148();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_SCP148;
        }

        bool updateItem();

        static itemScp148* createItemScp148();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMSCP148_H_INCLUDED
