//SCP - Containment Breach, Irrlicht port

#include <iostream>

#include <irrlicht.h>

#include <CGUITTFont.h>

#include "3dworld/3dworld.h"

#include <AL/al.h>
#include <alure.h>

int main() {

    int countt;
    const ALCchar** alureDevices = alureGetDeviceNames(false,&countt);
    for (int i=0;i<countt;i++) {
        std::cout<<"Sound Device "<<i<<": "<<alureDevices[i]<<"\n";
    }

    //Note to self: initialize ALURE only after Irrlicht has initialized
    //Otherwise input is buggy
    world * mainWorld = new world(1280,720,false);

    alureInitDevice(alureDevices[0],nullptr);

    alureFreeDeviceNames(alureDevices);

    while (mainWorld->run()) {}

    alureShutdownDevice();

    delete mainWorld;
    return 0;
}
