#ifndef ITEMKEY3_H_INCLUDED
#define ITEMKEY3_H_INCLUDED

#include "Items.h"

class ItemKey3 : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemKey3();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_KEY3;
        }

        bool updateItem();
        void updateWearing();
		void drawItem();

        static ItemKey3* createItemKey3();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);

		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMKEY3_H_INCLUDED
