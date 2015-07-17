#include "ItemKey4.h"

irr::scene::IMeshSceneNode* ItemKey4::baseNode = nullptr;
btConvexHullShape* ItemKey4::shape = nullptr;
irr::core::vector3df ItemKey4::offset = irr::core::vector3df(0,0,0);

ItemKey4::ItemKey4() {}

void ItemKey4::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemKey4::baseNode == nullptr) {
        ItemKey4::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemKey4* ItemKey4::createItemKey4() {
    if (ItemKey4::baseNode == nullptr) return nullptr;

    if (ItemKey4::shape == nullptr) {
		Item::createShapeFromNode(ItemKey4::baseNode,ItemKey4::shape,ItemKey4::offset);
	}

    ItemKey4* newIt = new ItemKey4;

    newIt->loadAssets(ItemKey4::baseNode,ItemKey4::shape);

    return newIt;
}

irr::core::vector3df ItemKey4::getOffset() {
	return ItemKey4::offset;
}

std::string ItemKey4::getInvName() {
	return std::string("Level 4 Key Card");
}

std::string ItemKey4::getInvImgPath() {
	return std::string("GFX/Items/INVkey4.jpg");
}

bool ItemKey4::updateItem() {
	return true;
}

void ItemKey4::updateWearing() {}

void ItemKey4::drawItem() {
    irr::video::ITexture* keyTex = Item::irrDriver->getTexture(getInvImgPath().c_str());
    Item::irrDriver->draw2DImage(keyTex,irr::core::recti(drawCoords.X-32,drawCoords.Y-32,drawCoords.X+32,drawCoords.Y+32),irr::core::recti(0,0,64,64));
}

void ItemKey4::setDrawCoords(irr::core::vector2di newCoords) {
    drawCoords = newCoords;
}
