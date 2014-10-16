#ifndef ITEMFINEFIRSTAID_H_INCLUDED
#define ITEMFINEFIRSTAID_H_INCLUDED

#include "../items.h"

class itemFinefirstaid : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemFinefirstaid();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_FINEFIRSTAID;
        }

        bool updateItem();

        static itemFinefirstaid* createItemFinefirstaid();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMFINEFIRSTAID_H_INCLUDED
