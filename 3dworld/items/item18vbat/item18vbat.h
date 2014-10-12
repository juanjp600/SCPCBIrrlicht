#ifndef ITEM18VBAT_H_INCLUDED
#define ITEM18VBAT_H_INCLUDED

#include "../items.h"

class item18vbat : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        item18vbat();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_18VBAT;
        }

        bool updateItem();

        static item18vbat* createItem18vbat();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEM18VBAT_H_INCLUDED
