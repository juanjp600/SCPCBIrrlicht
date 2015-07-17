#include "ItemGasmask.h"

#include "../World.h"
#include "../Player.h"

irr::scene::IMeshSceneNode* ItemGasmask::baseNode = nullptr;
btConvexHullShape* ItemGasmask::shape = nullptr;
irr::core::vector3df ItemGasmask::offset = irr::core::vector3df(0,0,0);

ItemGasmask::ItemGasmask() {}

void ItemGasmask::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemGasmask::baseNode == nullptr) {
        ItemGasmask::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemGasmask* ItemGasmask::createItemGasmask() {
    if (ItemGasmask::baseNode == nullptr) return nullptr;

    if (ItemGasmask::shape == nullptr) {
		Item::createShapeFromNode(ItemGasmask::baseNode,ItemGasmask::shape,ItemGasmask::offset);
	}

    ItemGasmask* newIt = new ItemGasmask;

    newIt->loadAssets(ItemGasmask::baseNode,ItemGasmask::shape);

    return newIt;
}

irr::core::vector3df ItemGasmask::getOffset() {
	return ItemGasmask::offset;
}

std::string ItemGasmask::getInvName() {
	return std::string("Gas Mask");
}

std::string ItemGasmask::getInvImgPath() {
	return std::string("GFX/Items/INVgasmask.jpg");
}

bool ItemGasmask::updateItem() {
    Item::mainPlayer->selectGasMask(this);
	return false;
}

void ItemGasmask::updateWearing() {}

void ItemGasmask::drawItem() {
    irr::video::ITexture* maskTex = Item::irrDriver->getTexture("GFX/GasmaskOverlay.png");
    unsigned short width = maskTex->getSize().Width;
    unsigned short height = maskTex->getSize().Height;
    Item::irrDriver->draw2DImage(maskTex,irr::core::recti(0,(Item::screenHeight/2)-((height/2)*screenWidth/screenHeight*0.7f),Item::screenWidth,(Item::screenHeight/2)+((height/2)*screenWidth/screenHeight*0.7f)),irr::core::recti(0,0,width,height),nullptr,nullptr,true);
}

void ItemGasmask::setDrawCoords(irr::core::vector2di newCoords) {}
