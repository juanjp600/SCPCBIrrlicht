@ECHO OFF

ECHO frag
glslangValidator ZBufferFrag.frag
ECHO frag
glslangValidator VertLightingFrag.frag
ECHO frag
glslangValidator RoomFragShader_noNormal.frag
ECHO frag
glslangValidator RoomFragShader.frag
ECHO frag
glslangValidator PostProcessFrag.frag
ECHO frag
glslangValidator NewNormalFrag_old.frag
ECHO frag
glslangValidator NewNormalFrag.frag
ECHO frag
glslangValidator LightingFrag_sm.frag
ECHO frag
glslangValidator LightingFrag.frag
ECHO frag
glslangValidator GammaFragShader.frag
ECHO frag
glslangValidator FogBillboardFrag.frag
ECHO frag
glslangValidator DeferredLightQuadFrag.frag

PAUSE