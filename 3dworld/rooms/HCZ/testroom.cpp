#include "../room.h"
#include "testroom.h"

RMesh* testroom::baseRMesh = nullptr;

void testroom::setBase(RMesh* inRme) {
	if (testroom::baseRMesh==nullptr) {
		testroom::baseRMesh = inRme;
	}
}

testroom* testroom::createNew(irr::core::vector3df inPosition,char inAngle) {
	testroom* retRoom = new testroom;

	retRoom->loadAssets(testroom::baseRMesh,inPosition,inAngle);
	return retRoom;
}

const std::vector<irr::video::SLight>& testroom::getPointLights() {
	return testroom::baseRMesh->pointlights;
}

void testroom::findWPPath(irr::core::vector3df startPos,irr::core::vector3df destPos,std::vector<irr::core::vector3df> &posList) {
	room::findWPPath(testroom::baseRMesh,startPos,destPos,posList);
}
