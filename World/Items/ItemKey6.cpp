#include "ItemKey6.h"

irr::scene::IMeshSceneNode* ItemKey6::baseNode = nullptr;
btConvexHullShape* ItemKey6::shape = nullptr;
irr::core::vector3df ItemKey6::offset = irr::core::vector3df(0,0,0);

ItemKey6::ItemKey6() {}

void ItemKey6::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemKey6::baseNode == nullptr) {
        ItemKey6::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemKey6* ItemKey6::createItemKey6() {
    if (ItemKey6::baseNode == nullptr) return nullptr;

    if (ItemKey6::shape == nullptr) {
		Item::createShapeFromNode(ItemKey6::baseNode,ItemKey6::shape,ItemKey6::offset);
	}

    ItemKey6* newIt = new ItemKey6;

    newIt->loadAssets(ItemKey6::baseNode,ItemKey6::shape);

    return newIt;
}

irr::core::vector3df ItemKey6::getOffset() {
	return ItemKey6::offset;
}

std::string ItemKey6::getInvName() {
	return std::string("Key Card Omni");
}

std::string ItemKey6::getInvImgPath() {
	return std::string("GFX/Items/INVkeyomni.jpg");
}

bool ItemKey6::updateItem() {
	return true;
}

void ItemKey6::updateWearing() {}

void ItemKey6::drawItem() {
    irr::video::ITexture* keyTex = Item::irrDriver->getTexture(getInvImgPath().c_str());
    Item::irrDriver->draw2DImage(keyTex,irr::core::recti(Item::screenWidth/2-32,Item::screenHeight/2-32,Item::screenWidth/2+32,Item::screenHeight/2+32),irr::core::recti(0,0,64,64));
}
