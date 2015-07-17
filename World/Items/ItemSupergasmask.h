#ifndef ITEMSUPERGASMASK_H_INCLUDED
#define ITEMSUPERGASMASK_H_INCLUDED

#include "Items.h"

class ItemSupergasmask : public Item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new Item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        ItemSupergasmask();

        //static irr::scene::IMesh* getMesh();
    public:

        ItemTempIDs getTempID() {
            return ItemTempIDs::ITEM_SUPERGASMASK;
        }

        bool updateItem();
        void updateWearing();
		void drawItem();
		void setDrawCoords(irr::core::vector2di newCoords);

        static ItemSupergasmask* createItemSupergasmask();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);

		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMSUPERGASMASK_H_INCLUDED
