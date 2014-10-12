#ifndef ITEMSCP860_H_INCLUDED
#define ITEMSCP860_H_INCLUDED

#include "../items.h"

class itemScp860 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemScp860();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_SCP860;
        }

        bool updateItem();

        static itemScp860* createItemScp860();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMSCP860_H_INCLUDED
