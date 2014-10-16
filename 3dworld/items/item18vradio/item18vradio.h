#ifndef ITEM18VRADIO_H_INCLUDED
#define ITEM18VRADIO_H_INCLUDED

#include "../items.h"

class item18vradio : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        item18vradio();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_18VRADIO;
        }

        bool updateItem();

        static item18vradio* createItem18vradio();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();
		virtual std::string getInvImgPath();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEM18VRADIO_H_INCLUDED
