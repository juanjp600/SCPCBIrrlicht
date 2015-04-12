#ifndef ITEM18VRADIO_H_INCLUDED
#define ITEM18VRADIO_H_INCLUDED

#include "Items.h"

class Item18vradio : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        Item18vradio();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_18VRADIO;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static Item18vradio* createItem18vradio();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEM18VRADIO_H_INCLUDED
