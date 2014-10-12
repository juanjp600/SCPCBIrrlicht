#ifndef ITEMNAV_H_INCLUDED
#define ITEMNAV_H_INCLUDED

#include "../items.h"

class itemNav : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemNav();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_NAV;
        }

        bool updateItem();

        static itemNav* createItemNav();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMNAV_H_INCLUDED
