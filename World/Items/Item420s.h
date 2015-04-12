#ifndef ITEM420S_H_INCLUDED
#define ITEM420S_H_INCLUDED

#include "Items.h"

class Item420s : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        Item420s();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_420S;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static Item420s* createItem420s();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEM420S_H_INCLUDED
