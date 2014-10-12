#ifndef ITEMSCP714_H_INCLUDED
#define ITEMSCP714_H_INCLUDED

#include "../items.h"

class itemScp714 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemScp714();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_SCP714;
        }

        bool updateItem();

        static itemScp714* createItemScp714();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMSCP714_H_INCLUDED
