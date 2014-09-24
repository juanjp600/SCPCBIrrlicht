#include "room.h"

dynRegister* room::dynamics = nullptr;

void room::setDynamics(dynRegister* inDyn) {
	if (room::dynamics==nullptr) room::dynamics = inDyn;
}
