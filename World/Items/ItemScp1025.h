#ifndef ITEMSCP1025_H_INCLUDED
#define ITEMSCP1025_H_INCLUDED

#include "Items.h"

class ItemScp1025 : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemScp1025();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_SCP1025;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static ItemScp1025* createItemScp1025();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMSCP1025_H_INCLUDED
