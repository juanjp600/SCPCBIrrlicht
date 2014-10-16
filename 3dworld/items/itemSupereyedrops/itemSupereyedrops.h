#ifndef ITEMSUPEREYEDROPS_H_INCLUDED
#define ITEMSUPEREYEDROPS_H_INCLUDED

#include "../items.h"

class itemSupereyedrops : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemSupereyedrops();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_SUPEREYEDROPS;
        }

        bool updateItem();

        static itemSupereyedrops* createItemSupereyedrops();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMSUPEREYEDROPS_H_INCLUDED
