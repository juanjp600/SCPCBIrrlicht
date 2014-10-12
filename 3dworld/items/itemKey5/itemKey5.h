#ifndef ITEMKEY5_H_INCLUDED
#define ITEMKEY5_H_INCLUDED

#include "../items.h"

class itemKey5 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemKey5();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_KEY5;
        }

        bool updateItem();

        static itemKey5* createItemKey5();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMKEY5_H_INCLUDED
