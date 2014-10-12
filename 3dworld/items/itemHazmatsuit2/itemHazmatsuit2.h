#ifndef ITEMHAZMATSUIT2_H_INCLUDED
#define ITEMHAZMATSUIT2_H_INCLUDED

#include "../items.h"

class itemHazmatsuit2 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemHazmatsuit2();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_HAZMATSUIT2;
        }

        bool updateItem();

        static itemHazmatsuit2* createItemHazmatsuit2();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMHAZMATSUIT2_H_INCLUDED
