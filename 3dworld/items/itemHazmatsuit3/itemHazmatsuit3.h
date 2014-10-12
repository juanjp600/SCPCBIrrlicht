#ifndef ITEMHAZMATSUIT3_H_INCLUDED
#define ITEMHAZMATSUIT3_H_INCLUDED

#include "../items.h"

class itemHazmatsuit3 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemHazmatsuit3();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_HAZMATSUIT3;
        }

        bool updateItem();

        static itemHazmatsuit3* createItemHazmatsuit3();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMHAZMATSUIT3_H_INCLUDED
