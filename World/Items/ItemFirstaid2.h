#ifndef ITEMFIRSTAID2_H_INCLUDED
#define ITEMFIRSTAID2_H_INCLUDED

#include "Items.h"

class ItemFirstaid2 : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemFirstaid2();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_FIRSTAID2;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static ItemFirstaid2* createItemFirstaid2();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMFIRSTAID2_H_INCLUDED
