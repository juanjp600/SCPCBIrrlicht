#ifndef ITEMHAZMATSUIT3_H_INCLUDED
#define ITEMHAZMATSUIT3_H_INCLUDED

#include "Items.h"

class ItemHazmatsuit3 : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemHazmatsuit3();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_HAZMATSUIT3;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();
		void setDrawCoords(irr::core::vector2di newCoords);

        static ItemHazmatsuit3* createItemHazmatsuit3();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);

		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMHAZMATSUIT3_H_INCLUDED
