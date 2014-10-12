#ifndef ITEMVERYFINEVEST_H_INCLUDED
#define ITEMVERYFINEVEST_H_INCLUDED

#include "../items.h"

class itemVeryfinevest : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemVeryfinevest();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_VERYFINEVEST;
        }

        bool updateItem();

        static itemVeryfinevest* createItemVeryfinevest();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMVERYFINEVEST_H_INCLUDED
