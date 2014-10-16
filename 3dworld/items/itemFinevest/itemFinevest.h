#ifndef ITEMFINEVEST_H_INCLUDED
#define ITEMFINEVEST_H_INCLUDED

#include "../items.h"

class itemFinevest : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemFinevest();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_FINEVEST;
        }

        bool updateItem();

        static itemFinevest* createItemFinevest();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMFINEVEST_H_INCLUDED
