#ifndef ITEMCIGARETTE_H_INCLUDED
#define ITEMCIGARETTE_H_INCLUDED

#include "../items.h"

class itemCigarette : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemCigarette();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_CIGARETTE;
        }

        bool updateItem();

        static itemCigarette* createItemCigarette();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMCIGARETTE_H_INCLUDED
