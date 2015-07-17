#ifndef ITEMHAZMATSUIT2_H_INCLUDED
#define ITEMHAZMATSUIT2_H_INCLUDED

#include "Items.h"

class ItemHazmatsuit2 : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemHazmatsuit2();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_HAZMATSUIT2;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();
		void setDrawCoords(irr::core::vector2di newCoords);

        static ItemHazmatsuit2* createItemHazmatsuit2();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);

		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMHAZMATSUIT2_H_INCLUDED
