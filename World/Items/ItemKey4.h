#ifndef ITEMKEY4_H_INCLUDED
#define ITEMKEY4_H_INCLUDED

#include "Items.h"

class ItemKey4 : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemKey4();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_KEY4;
        }

        bool updateItem();
        void updateWearing();
		void drawItem();

        static ItemKey4* createItemKey4();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);

		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMKEY4_H_INCLUDED
