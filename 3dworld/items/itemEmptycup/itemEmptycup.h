#ifndef ITEMEMPTYCUP_H_INCLUDED
#define ITEMEMPTYCUP_H_INCLUDED

#include "../items.h"

class itemEmptycup : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemEmptycup();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_EMPTYCUP;
        }

        bool updateItem();
		void updateWearing();
		void drawItem();

        static itemEmptycup* createItemEmptycup();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMEMPTYCUP_H_INCLUDED
