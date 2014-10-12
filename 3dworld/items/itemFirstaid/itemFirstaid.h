#ifndef ITEMFIRSTAID_H_INCLUDED
#define ITEMFIRSTAID_H_INCLUDED

#include "../items.h"

class itemFirstaid : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemFirstaid();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_FIRSTAID;
        }

        bool updateItem();

        static itemFirstaid* createItemFirstaid();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMFIRSTAID_H_INCLUDED
