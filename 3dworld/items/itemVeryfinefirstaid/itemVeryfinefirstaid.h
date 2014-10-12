#ifndef ITEMVERYFINEFIRSTAID_H_INCLUDED
#define ITEMVERYFINEFIRSTAID_H_INCLUDED

#include "../items.h"

class itemVeryfinefirstaid : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemVeryfinefirstaid();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_VERYFINEFIRSTAID;
        }

        bool updateItem();

        static itemVeryfinefirstaid* createItemVeryfinefirstaid();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMVERYFINEFIRSTAID_H_INCLUDED
