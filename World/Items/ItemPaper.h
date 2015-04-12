#ifndef ITEMPAPER_H_INCLUDED
#define ITEMPAPER_H_INCLUDED

#include "Items.h"

class ItemPaper : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemPaper();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_PAPER;
        }

        bool updateItem();
        void updateWearing();
		void drawItem();

        static ItemPaper* createItemPaper();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);

		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMPAPER_H_INCLUDED
