#ifndef ITEMHAND_H_INCLUDED
#define ITEMHAND_H_INCLUDED

#include "Items.h"

class ItemHand : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemHand();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_HAND;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static ItemHand* createItemHand();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMHAND_H_INCLUDED
