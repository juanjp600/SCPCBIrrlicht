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
  - libogg & libvorbis: http://www.xiph.org/downloads/
  
### How to compile

1. Download source code for Irrlicht SVN revision 5113, shader-pipeline branch ( http://sourceforge.net/p/irrlicht/code/5113/tree/branches/shader-pipeline/ )
2. Apply the patches in the "Irrlicht/source" and "Irrlicht/include" folders in this repository, and add the new files.
3. Compile Irrlicht in Debug mode.
4. Download (and compile if necessary) all the other libraries listed above.
5. Open SCPCBIrrlicht.cbp in Code::Blocks, add proper include directories and linker options, and compile.

