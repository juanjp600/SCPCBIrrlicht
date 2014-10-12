#ifndef ITEMHAZMATSUIT_H_INCLUDED
#define ITEMHAZMATSUIT_H_INCLUDED

#include "../items.h"

class itemHazmatsuit : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemHazmatsuit();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_HAZMATSUIT;
        }

        bool updateItem();

        static itemHazmatsuit* createItemHazmatsuit();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMHAZMATSUIT_H_INCLUDED
