#ifndef ITEMKEY1_H_INCLUDED
#define ITEMKEY1_H_INCLUDED

#include "../items.h"

class itemKey1 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemKey1();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_KEY1;
        }

        bool updateItem();

        static itemKey1* createItemKey1();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMKEY1_H_INCLUDED
