#include "itemGasmask.h"

#include "../../3dworld.h"
#include "../../player.h"

irr::scene::IMeshSceneNode* itemGasmask::baseNode = nullptr;
btConvexHullShape* itemGasmask::shape = nullptr;
irr::core::vector3df itemGasmask::offset = irr::core::vector3df(0,0,0);

itemGasmask::itemGasmask() {}

void itemGasmask::setMeshNode(irr::scene::IMeshSceneNode* node) {
    if (itemGasmask::baseNode == nullptr) {
        itemGasmask::baseNode = node;
        node->setVisible(false);
    }
    node = nullptr;
}

itemGasmask* itemGasmask::createItemGasmask() {
    if (itemGasmask::baseNode == nullptr) return nullptr;

    if (itemGasmask::shape == nullptr) {
		item::createShapeFromNode(itemGasmask::baseNode,itemGasmask::shape,itemGasmask::offset);
	}

    itemGasmask* newIt = new itemGasmask;

    newIt->loadAssets(itemGasmask::baseNode,itemGasmask::shape);

    return newIt;
}

irr::core::vector3df itemGasmask::getOffset() {
	return itemGasmask::offset;
}

std::string itemGasmask::getInvName() {
	return std::string("Gas Mask");
}

std::string itemGasmask::getInvImgPath() {
	return std::string("GFX/items/INVgasmask.jpg");
}

bool itemGasmask::updateItem() {
    item::mainPlayer->selectGasMask(this);
	return false;
}

void itemGasmask::updateWearing() {}

void itemGasmask::drawItem() {
    irr::video::ITexture* maskTex = item::irrDriver->getTexture("GFX/GasmaskOverlay.png");
    unsigned short width = maskTex->getSize().Width;
    unsigned short height = maskTex->getSize().Height;
    item::irrDriver->draw2DImage(maskTex,irr::core::recti(0,(item::screenHeight/2)-((height/2)*screenWidth/screenHeight*0.7f),item::screenWidth,(item::screenHeight/2)+((height/2)*screenWidth/screenHeight*0.7f)),irr::core::recti(0,0,width,height),nullptr,nullptr,true);
}
