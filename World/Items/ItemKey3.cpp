#include "ItemKey3.h"

irr::scene::IMeshSceneNode* ItemKey3::baseNode = nullptr;
btConvexHullShape* ItemKey3::shape = nullptr;
irr::core::vector3df ItemKey3::offset = irr::core::vector3df(0,0,0);

ItemKey3::ItemKey3() {}

void ItemKey3::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemKey3::baseNode == nullptr) {
        ItemKey3::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemKey3* ItemKey3::createItemKey3() {
    if (ItemKey3::baseNode == nullptr) return nullptr;

    if (ItemKey3::shape == nullptr) {
		Item::createShapeFromNode(ItemKey3::baseNode,ItemKey3::shape,ItemKey3::offset);
	}

    ItemKey3* newIt = new ItemKey3;

    newIt->loadAssets(ItemKey3::baseNode,ItemKey3::shape);

    return newIt;
}

irr::core::vector3df ItemKey3::getOffset() {
	return ItemKey3::offset;
}

std::string ItemKey3::getInvName() {
	return std::string("Level 3 Key Card");
}

std::string ItemKey3::getInvImgPath() {
	return std::string("GFX/Items/INVkey3.jpg");
}

bool ItemKey3::updateItem() {
	return true;
}

void ItemKey3::updateWearing() {}

void ItemKey3::drawItem() {
    irr::video::ITexture* keyTex = Item::irrDriver->getTexture(getInvImgPath().c_str());
    Item::irrDriver->draw2DImage(keyTex,irr::core::recti(Item::screenWidth/2-32,Item::screenHeight/2-32,Item::screenWidth/2+32,Item::screenHeight/2+32),irr::core::recti(0,0,64,64));
}
