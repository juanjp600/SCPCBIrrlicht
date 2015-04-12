#ifndef ITEMBAT_H_INCLUDED
#define ITEMBAT_H_INCLUDED

#include "Items.h"

class ItemBat : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemBat();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_BAT;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static ItemBat* createItemBat();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMBAT_H_INCLUDED
