#include "itemSupergasmask.h"

#include "../../3dworld.h"
#include "../../player.h"

irr::scene::IMeshSceneNode* itemSupergasmask::baseNode = nullptr;
btConvexHullShape* itemSupergasmask::shape = nullptr;
irr::core::vector3df itemSupergasmask::offset = irr::core::vector3df(0,0,0);

itemSupergasmask::itemSupergasmask() {}

void itemSupergasmask::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemSupergasmask::baseNode == nullptr) {
        itemSupergasmask::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemSupergasmask* itemSupergasmask::createItemSupergasmask() {
    if (itemSupergasmask::baseNode == nullptr) return nullptr;

    if (itemSupergasmask::shape == nullptr) {
		item::createShapeFromNode(itemSupergasmask::baseNode,itemSupergasmask::shape,itemSupergasmask::offset);
	}

    itemSupergasmask* newIt = new itemSupergasmask;

    newIt->loadAssets(itemSupergasmask::baseNode,itemSupergasmask::shape);

    return newIt;
}

irr::core::vector3df itemSupergasmask::getOffset() {
	return itemSupergasmask::offset;
}

std::string itemSupergasmask::getInvName() {
	return std::string("Gas Mask");
}

std::string itemSupergasmask::getInvImgPath() {
	return std::string("GFX/items/INVgasmask.jpg");
}

bool itemSupergasmask::updateItem() {
    item::mainPlayer->selectGasMask(this);
	return false;
}

void itemSupergasmask::updateWearing() {
    item::mainPlayer->boostStamina(0.1f,50.f);
}

void itemSupergasmask::drawItem() {
    irr::video::ITexture* maskTex = item::irrDriver->getTexture("GFX/GasmaskOverlay.png");
    unsigned short width = maskTex->getSize().Width;
    unsigned short height = maskTex->getSize().Height;
    item::irrDriver->draw2DImage(maskTex,irr::core::recti(0,(item::screenHeight/2)-((height/2)*screenWidth/screenHeight*0.7f),item::screenWidth,(item::screenHeight/2)+((height/2)*screenWidth/screenHeight*0.7f)),irr::core::recti(0,0,width,height),nullptr,nullptr,true);
}
