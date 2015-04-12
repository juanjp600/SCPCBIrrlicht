#ifndef ITEMSCP513_H_INCLUDED
#define ITEMSCP513_H_INCLUDED

#include "Items.h"

class ItemScp513 : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemScp513();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_SCP513;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static ItemScp513* createItemScp513();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMSCP513_H_INCLUDED
