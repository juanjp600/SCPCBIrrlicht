#ifndef ITEMFINERADIO_H_INCLUDED
#define ITEMFINERADIO_H_INCLUDED

#include "../items.h"

class itemFineradio : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemFineradio();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_FINERADIO;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static itemFineradio* createItemFineradio();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMFINERADIO_H_INCLUDED
