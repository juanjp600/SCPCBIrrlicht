#ifndef ITEMKEY3_H_INCLUDED
#define ITEMKEY3_H_INCLUDED

#include "../items.h"

class itemKey3 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemKey3();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_KEY3;
        }

        bool updateItem();

        static itemKey3* createItemKey3();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMKEY3_H_INCLUDED
