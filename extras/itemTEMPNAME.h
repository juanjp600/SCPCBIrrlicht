#ifndef ITEMTMPNAME_H_INCLUDED
#define ITEMTMPNAME_H_INCLUDED

#include "../items.h"

class itemTEMPNAME : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemTEMPNAME();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_TMPNAME;
        }

        bool updateItem();

        static itemTEMPNAME* createItemTEMPNAME();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMTMPNAME_H_INCLUDED
