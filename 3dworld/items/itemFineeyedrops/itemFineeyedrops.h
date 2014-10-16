#ifndef ITEMFINEEYEDROPS_H_INCLUDED
#define ITEMFINEEYEDROPS_H_INCLUDED

#include "../items.h"

class itemFineeyedrops : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemFineeyedrops();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_FINEEYEDROPS;
        }

        bool updateItem();

        static itemFineeyedrops* createItemFineeyedrops();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMFINEEYEDROPS_H_INCLUDED
