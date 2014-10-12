#ifndef ITEMHAND2_H_INCLUDED
#define ITEMHAND2_H_INCLUDED

#include "../items.h"

class itemHand2 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemHand2();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_HAND2;
        }

        bool updateItem();

        static itemHand2* createItemHand2();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMHAND2_H_INCLUDED
