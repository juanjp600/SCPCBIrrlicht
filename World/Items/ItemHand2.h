#ifndef ITEMHAND2_H_INCLUDED
#define ITEMHAND2_H_INCLUDED

#include "Items.h"

class ItemHand2 : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemHand2();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_HAND2;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static ItemHand2* createItemHand2();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMHAND2_H_INCLUDED
