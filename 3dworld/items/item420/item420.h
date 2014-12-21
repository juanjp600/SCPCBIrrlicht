#ifndef ITEM420_H_INCLUDED
#define ITEM420_H_INCLUDED

#include "../items.h"

class item420 : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        item420();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_420;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static item420* createItem420();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEM420_H_INCLUDED
