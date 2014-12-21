#ifndef ITEMKEY2_H_INCLUDED
#define ITEMKEY2_H_INCLUDED

#include "../items.h"

class itemKey2 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemKey2();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_KEY2;
        }

        bool updateItem();
        void updateWearing();
		void drawItem();

        static itemKey2* createItemKey2();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);

		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMKEY2_H_INCLUDED
