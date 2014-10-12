#ifndef ITEMKILLBAT_H_INCLUDED
#define ITEMKILLBAT_H_INCLUDED

#include "../items.h"

class itemKillbat : public item {
    private:
        static irr::scene::IMeshSceneNode* baseNode; //the node that's cloned when a new item is created
        static btConvexHullShape* shape; //the collision shape used for the rigid body
        static irr::core::vector3df offset;

        itemKillbat();

        //static irr::scene::IMesh* getMesh();
    public:

        itemTempIDs getTempID() {
            return itemTempIDs::ITEM_KILLBAT;
        }

        bool updateItem();

        static itemKillbat* createItemKillbat();

        static void setMeshNode(irr::scene::IMeshSceneNode* node);
		
		virtual std::string getInvName();

        virtual irr::core::vector3df getOffset();
};

#endif // ITEMKILLBAT_H_INCLUDED
