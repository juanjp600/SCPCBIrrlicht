#include "ItemSupergasmask.h"

#include "../World.h"
#include "../Player.h"

irr::scene::IMeshSceneNode* ItemSupergasmask::baseNode = nullptr;
btConvexHullShape* ItemSupergasmask::shape = nullptr;
irr::core::vector3df ItemSupergasmask::offset = irr::core::vector3df(0,0,0);

ItemSupergasmask::ItemSupergasmask() {}

void ItemSupergasmask::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (ItemSupergasmask::baseNode == nullptr) {
        ItemSupergasmask::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

ItemSupergasmask* ItemSupergasmask::createItemSupergasmask() {
    if (ItemSupergasmask::baseNode == nullptr) return nullptr;

    if (ItemSupergasmask::shape == nullptr) {
		Item::createShapeFromNode(ItemSupergasmask::baseNode,ItemSupergasmask::shape,ItemSupergasmask::offset);
	}

    ItemSupergasmask* newIt = new ItemSupergasmask;

    newIt->loadAssets(ItemSupergasmask::baseNode,ItemSupergasmask::shape);

    return newIt;
}

irr::core::vector3df ItemSupergasmask::getOffset() {
	return ItemSupergasmask::offset;
}

std::string ItemSupergasmask::getInvName() {
	return std::string("Gas Mask");
}

std::string ItemSupergasmask::getInvImgPath() {
	return std::string("GFX/Items/INVgasmask.jpg");
}

bool ItemSupergasmask::updateItem() {
    Item::mainPlayer->selectGasMask(this);
	return false;
}

void ItemSupergasmask::updateWearing() {
    Item::mainPlayer->booststamina(0.1f,50.f);
}

void ItemSupergasmask::drawItem() {
    irr::video::ITexture* maskTex = Item::irrDriver->getTexture("GFX/GasmaskOverlay.png");
    unsigned short width = maskTex->getSize().Width;
    unsigned short height = maskTex->getSize().Height;
    Item::irrDriver->draw2DImage(maskTex,irr::core::recti(0,(Item::screenHeight/2)-((height/2)*screenWidth/screenHeight*0.7f),Item::screenWidth,(Item::screenHeight/2)+((height/2)*screenWidth/screenHeight*0.7f)),irr::core::recti(0,0,width,height),nullptr,nullptr,true);
}
