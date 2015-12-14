#ifndef ITEMKEY1_H_INCLUDED
#define ITEMKEY1_H_INCLUDED

#include "Items.h"

class ItemKey1 : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemKey1();

        irr::core::vector2di drawCoords;

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_KEY1;
        }

        bool updateItem();
        void updateWearing();
		void drawItem();
		void setDrawCoords(irr::core::vector2di newCoords);

        static ItemKey1* createItemKey1();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);

		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();

        virtual bool refineItem(REFINE_SETTINGS setting,irr::core::aabbox3df intake,irr::core::aabbox3df output,Item*& result);
};

#endif // ITEMKEY1_H_INCLUDED
