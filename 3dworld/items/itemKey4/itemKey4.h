#ifndef ITEMKEY4_H_INCLUDED
#define ITEMKEY4_H_INCLUDED

#include "../items.h"

class itemKey4 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemKey4();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_KEY4;
        }

        bool updateItem();

        static itemKey4* createItemKey4();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMKEY4_H_INCLUDED
