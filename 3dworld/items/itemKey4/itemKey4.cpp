#include "itemKey4.h"

irr::scene::IMeshSceneNode* itemKey4::baseNode = nullptr;
btConvexHullShape* itemKey4::shape = nullptr;
irr::core::vector3df itemKey4::offset = irr::core::vector3df(0,0,0);

itemKey4::itemKey4() {}

void itemKey4::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemKey4::baseNode == nullptr) {
        itemKey4::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemKey4* itemKey4::createItemKey4() {
    if (itemKey4::baseNode == nullptr) return nullptr;

    if (itemKey4::shape == nullptr) {
		item::createShapeFromNode(itemKey4::baseNode,itemKey4::shape,itemKey4::offset);
	}

    itemKey4* newIt = new itemKey4;

    newIt->loadAssets(itemKey4::baseNode,itemKey4::shape);

    return newIt;
}

irr::core::vector3df itemKey4::getOffset() {
	return itemKey4::offset;
}

std::string itemKey4::getInvName() {
	return std::string("Level 4 Key Card");
}

std::string itemKey4::getInvImgPath() {
	return std::string("GFX/items/INVkey4.jpg");
}

bool itemKey4::updateItem() {
	return true;
}

void itemKey4::updateWearing() {}

void itemKey4::drawItem() {
    irr::video::ITexture* keyTex = item::irrDriver->getTexture(getInvImgPath().c_str());
    item::irrDriver->draw2DImage(keyTex,irr::core::recti(item::screenWidth/2-32,item::screenHeight/2-32,item::screenWidth/2+32,item::screenHeight/2+32),irr::core::recti(0,0,64,64));
}
