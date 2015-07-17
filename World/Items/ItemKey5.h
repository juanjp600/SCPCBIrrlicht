#ifndef ITEMKEY5_H_INCLUDED
#define ITEMKEY5_H_INCLUDED

#include "Items.h"

class ItemKey5 : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemKey5();

        irr::core::vector2di drawCoords;

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_KEY5;
        }

        bool updateItem();
        void updateWearing();
		void drawItem();
		void setDrawCoords(irr::core::vector2di newCoords);

        static ItemKey5* createItemKey5();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);

		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMKEY5_H_INCLUDED
