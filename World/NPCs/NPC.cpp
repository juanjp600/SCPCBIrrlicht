#include "NPC.h"

class World* NPC::owner = nullptr;
class irrDynamics* NPC::dynamics = nullptr;
class Player* NPC::player = nullptr;

NPC::NPC() {}
NPC::~NPC() {}
