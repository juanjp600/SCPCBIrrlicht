#ifndef ITEMFIRSTAID2_H_INCLUDED
#define ITEMFIRSTAID2_H_INCLUDED

#include "../items.h"

class itemFirstaid2 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemFirstaid2();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_FIRSTAID2;
        }

        bool updateItem();

        static itemFirstaid2* createItemFirstaid2();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMFIRSTAID2_H_INCLUDED
