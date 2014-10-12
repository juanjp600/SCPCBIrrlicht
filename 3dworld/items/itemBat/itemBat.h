#ifndef ITEMBAT_H_INCLUDED
#define ITEMBAT_H_INCLUDED

#include "../items.h"

class itemBat : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemBat();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_BAT;
        }

        bool updateItem();

        static itemBat* createItemBat();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMBAT_H_INCLUDED
