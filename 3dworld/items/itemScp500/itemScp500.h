#ifndef ITEMSCP500_H_INCLUDED
#define ITEMSCP500_H_INCLUDED

#include "../items.h"

class itemScp500 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemScp500();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_SCP500;
        }

        bool updateItem();

        static itemScp500* createItemScp500();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMSCP500_H_INCLUDED
