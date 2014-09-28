#include "room.h"

#include <cmath>

dynRegister* room::dynamics = nullptr;

void room::setDynamics(dynRegister* inDyn) {
	if (room::dynamics==nullptr) room::dynamics = inDyn;
}

void room::setActivation(bool s) {
	if (s!=isActivated) {
		if (s) {
			dynamics->sharedRegisterRBody(node,rbody,-1);
			node->setVisible(true);
		} else {
			dynamics->sharedUnregisterRBody(rbody);
			node->setVisible(false);
		}
		isActivated = s;
	}
}
