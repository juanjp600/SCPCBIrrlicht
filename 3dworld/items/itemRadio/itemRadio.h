#ifndef ITEMRADIO_H_INCLUDED
#define ITEMRADIO_H_INCLUDED

#include "../items.h"

class itemRadio : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemRadio();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_RADIO;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static itemRadio* createItemRadio();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMRADIO_H_INCLUDED
