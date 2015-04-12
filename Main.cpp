/**
SCPCBIrrlicht
=============

SCP - Containment Breach on the Irrlicht Engine
http://scpcbgame.com/

The game is based on the works of the SCP Foundation community (http://www.scp-wiki.net/).

This game is licensed under Creative Commons Attribution-ShareAlike 3.0 License.

http://creativecommons.org/licenses/by-sa/3.0/


Libraries used:
  - Irrlicht Engine: http://irrlicht.sourceforge.net/
  - Bullet Physics: http://bulletphysics.org/
  - OpenAL-Soft: http://kcat.strangesoft.net/openal.html
  - FreeType: http://www.freetype.org/
**/

#include <iostream>

#include <irrlicht.h>

#include <CGUITTFont.h>

#include "Sound/SoundWrapper.h"
#include "World/World.h"

#include <AL/al.h>

int main() {

    World* mainWorld = new World(1280,720,false);

    while (mainWorld->run()) {};

    delete mainWorld;
    return 0;
}
