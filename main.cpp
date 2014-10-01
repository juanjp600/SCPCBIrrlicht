//SCP - Containment Breach, Irrlicht port

#include <iostream>

#include <irrlicht.h>

#include <CGUITTFont.h>

#include "sound/soundWrapper.h"
#include "3dworld/3dworld.h"

#include <AL/al.h>

int main() {

    world * mainWorld = new world(1280,720,false);

    while (mainWorld->run()) {};

    delete mainWorld;
    return 0;
}
