#include "ItemKey5.h"

irr::scene::IMeshSceneNode* ItemKey5::baseNode = nullptr;
btConvexHullShape* ItemKey5::shape = nullptr;
irr::core::vector3df ItemKey5::offset = irr::core::vector3df(0,0,0);

ItemKey5::ItemKey5() {}

void ItemKey5::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemKey5::baseNode == nullptr) {
        ItemKey5::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemKey5* ItemKey5::createItemKey5() {
    if (ItemKey5::baseNode == nullptr) return nullptr;

    if (ItemKey5::shape == nullptr) {
		Item::createShapeFromNode(ItemKey5::baseNode,ItemKey5::shape,ItemKey5::offset);
	}

    ItemKey5* newIt = new ItemKey5;

    newIt->loadAssets(ItemKey5::baseNode,ItemKey5::shape);

    return newIt;
}

irr::core::vector3df ItemKey5::getOffset() {
	return ItemKey5::offset;
}

std::string ItemKey5::getInvName() {
	return std::string("Level 5 Key Card");
}

std::string ItemKey5::getInvImgPath() {
	return std::string("GFX/Items/INVkey5.jpg");
}

bool ItemKey5::updateItem() {
	return true;
}

void ItemKey5::updateWearing() {}

void ItemKey5::drawItem() {
    irr::video::ITexture* keyTex = Item::irrDriver->getTexture(getInvImgPath().c_str());
    Item::irrDriver->draw2DImage(keyTex,irr::core::recti(Item::screenWidth/2-32,Item::screenHeight/2-32,Item::screenWidth/2+32,Item::screenHeight/2+32),irr::core::recti(0,0,64,64));
}
