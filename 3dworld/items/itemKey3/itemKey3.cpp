#include "itemKey3.h"

irr::scene::IMeshSceneNode* itemKey3::baseNode = nullptr;
btConvexHullShape* itemKey3::shape = nullptr;
irr::core::vector3df itemKey3::offset = irr::core::vector3df(0,0,0);

itemKey3::itemKey3() {}

void itemKey3::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemKey3::baseNode == nullptr) {
        itemKey3::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemKey3* itemKey3::createItemKey3() {
    if (itemKey3::baseNode == nullptr) return nullptr;

    if (itemKey3::shape == nullptr) {
		item::createShapeFromNode(itemKey3::baseNode,itemKey3::shape,itemKey3::offset);
	}

    itemKey3* newIt = new itemKey3;

    newIt->loadAssets(itemKey3::baseNode,itemKey3::shape);

    return newIt;
}

irr::core::vector3df itemKey3::getOffset() {
	return itemKey3::offset;
}

std::string itemKey3::getInvName() {
	return std::string("Level 3 Key Card");
}

std::string itemKey3::getInvImgPath() {
	return std::string("GFX/items/INVkey3.jpg");
}

bool itemKey3::updateItem() {
	return true;
}

void itemKey3::updateWearing() {}

void itemKey3::drawItem() {
    irr::video::ITexture* keyTex = item::irrDriver->getTexture(getInvImgPath().c_str());
    item::irrDriver->draw2DImage(keyTex,irr::core::recti(item::screenWidth/2-32,item::screenHeight/2-32,item::screenWidth/2+32,item::screenHeight/2+32),irr::core::recti(0,0,64,64));
}
