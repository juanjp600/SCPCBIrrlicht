#ifndef ITEMSCP1025_H_INCLUDED
#define ITEMSCP1025_H_INCLUDED

#include "../items.h"

class itemScp1025 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemScp1025();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_SCP1025;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static itemScp1025* createItemScp1025();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMSCP1025_H_INCLUDED
