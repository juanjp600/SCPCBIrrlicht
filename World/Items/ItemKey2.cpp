#include "ItemKey2.h"

irr::scene::IMeshSceneNode* ItemKey2::baseNode = nullptr;
btConvexHullShape* ItemKey2::shape = nullptr;
irr::core::vector3df ItemKey2::offset = irr::core::vector3df(0,0,0);

ItemKey2::ItemKey2() {}

void ItemKey2::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemKey2::baseNode == nullptr) {
        ItemKey2::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemKey2* ItemKey2::createItemKey2() {
    if (ItemKey2::baseNode == nullptr) return nullptr;

    if (ItemKey2::shape == nullptr) {
		Item::createShapeFromNode(ItemKey2::baseNode,ItemKey2::shape,ItemKey2::offset);
	}

    ItemKey2* newIt = new ItemKey2;

    newIt->loadAssets(ItemKey2::baseNode,ItemKey2::shape);

    return newIt;
}

irr::core::vector3df ItemKey2::getOffset() {
	return ItemKey2::offset;
}

std::string ItemKey2::getInvName() {
	return std::string("Level 2 Key Card");
}

std::string ItemKey2::getInvImgPath() {
	return std::string("GFX/Items/INVkey2.jpg");
}

bool ItemKey2::updateItem() {
	return true;
}

void ItemKey2::updateWearing() {}

void ItemKey2::drawItem() {
    irr::video::ITexture* keyTex = Item::irrDriver->getTexture(getInvImgPath().c_str());
    Item::irrDriver->draw2DImage(keyTex,irr::core::recti(Item::screenWidth/2-32,Item::screenHeight/2-32,Item::screenWidth/2+32,Item::screenHeight/2+32),irr::core::recti(0,0,64,64));
}
