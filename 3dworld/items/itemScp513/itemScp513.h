#ifndef ITEMSCP513_H_INCLUDED
#define ITEMSCP513_H_INCLUDED

#include "../items.h"

class itemScp513 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemScp513();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_SCP513;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static itemScp513* createItemScp513();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMSCP513_H_INCLUDED
