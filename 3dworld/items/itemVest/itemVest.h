#ifndef ITEMVEST_H_INCLUDED
#define ITEMVEST_H_INCLUDED

#include "../items.h"

class itemVest : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemVest();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_VEST;
        }

        bool updateItem();

        static itemVest* createItemVest();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMVEST_H_INCLUDED
