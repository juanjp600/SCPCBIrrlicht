#include "ShaderCallbacks.h"
#include <iostream>

#include "Rooms/RMesh.h"

std::vector<LightsShaderCallBack::sortHelper> LightsShaderCallBack::lightList;

irr::video::SColorf SharedShaderCallBack::ambientLight;
irr::video::SColorf SharedShaderCallBack::addColor;
irr::core::vector3df SharedShaderCallBack::cameraPos;
float SharedShaderCallBack::fogNear;
float SharedShaderCallBack::fogFar;
float LightsShaderCallBack::renderSpecularFactor;
//irr::video::ITexture* SharedShaderCallBack::fogTexture;

void SharedShaderCallBack::setFogConstants(irr::video::IMaterialRendererServices* services) {
    irr::video::IVideoDriver* driver = services->getVideoDriver();
    irr::core::matrix4 worldMat;
	worldMat = driver->getTransform(irr::video::ETS_WORLD);

    float camPos[3];
    SharedShaderCallBack::cameraPos.getAs3Values(camPos);
	services->setVertexShaderConstant("mWorld", worldMat.pointer(), 16);

    services->setVertexShaderConstant("cameraPos",camPos,3);

	services->setPixelShaderConstant("fogNear",&SharedShaderCallBack::fogNear,1);
	services->setPixelShaderConstant("fogFar",&SharedShaderCallBack::fogFar,1);
}

void RoomShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	irr::video::IVideoDriver* driver = services->getVideoDriver();

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("Texture0", &TextureLayerID, 1);
	irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("Texture1", &TextureLayerID2, 1);
	irr::s32 TextureLayerID3 = 2;
	services->setPixelShaderConstant("Texture2", &TextureLayerID3, 1);
	irr::s32 TextureLayerIDNY = 3;
	services->setPixelShaderConstant("reflection", &TextureLayerIDNY, 1);

	services->setPixelShaderConstant("reflectFactor", &reflectFactor, 1);

    SharedShaderCallBack::setFogConstants(services);
    irr::s32 TextureLayerID5 = 4;
	services->setPixelShaderConstant("fogTexture", &TextureLayerID5, 1);
}

void ZBufferShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
    irr::video::IVideoDriver* driver = services->getVideoDriver();

    if (driver->currentlyRenderedNode->getType()==irr::scene::ESCENE_NODE_TYPE::ESNT_ANIMATED_MESH) {
        irr::scene::ISkinnedMesh* mesh = (irr::scene::ISkinnedMesh*)(((irr::scene::IAnimatedMeshSceneNode*)(driver->currentlyRenderedNode))->getMesh());
        irr::f32* JointArray = new irr::f32[mesh->getAllJoints().size() * 16];

        int copyIncrement = 0;

        //std::cout<<"fffff "<<mesh->getAllJoints().size()<<"\n";
        for(int i = 0;i < mesh->getAllJoints().size();++i)
        {
            irr::core::matrix4 JointVertexPull(irr::core::matrix4::EM4CONST_NOTHING);
            JointVertexPull.setbyproduct(
                mesh->getAllJoints()[i]->GlobalAnimatedMatrix,
                mesh->getAllJoints()[i]->GlobalInversedMatrix);

            for(int j = 0;j < 16;++j) {
                JointArray[copyIncrement+j] = JointVertexPull[j];
            }
            copyIncrement += 16;
        }

        services->setVertexShaderConstant("uBone", JointArray, mesh->getAllJoints().size() * 16);

        delete[] JointArray; //TODO: should probably be using a container class that doesn't get wiped until the next pass
    }

    irr::core::matrix4 worldViewProj;
	worldViewProj = driver->getTransform(irr::video::ETS_WORLD);

	services->setVertexShaderConstant("mWorld", worldViewProj.pointer(), 16);
}

void RoomShaderCallBack_noNormals::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	irr::video::IVideoDriver* driver = services->getVideoDriver();

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("Texture0", &TextureLayerID, 1);
	irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("Texture1", &TextureLayerID2, 1);

	SharedShaderCallBack::setFogConstants(services);

	irr::s32 TextureLayerID3 = 2;
	services->setPixelShaderConstant("fogTexture", &TextureLayerID3, 1);
}

void VertLightShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("Texture0", &TextureLayerID, 1);

	SharedShaderCallBack::setFogConstants(services);

	irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("fogTexture", &TextureLayerID2, 1);
}

void PostProcShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	currmBlur += (minBlur-currmBlur) * 0.1f;
	currMBlur += (maxBlur-currMBlur) * 0.1f;

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("Texture0", &TextureLayerID, 1);
	irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("Texture1", &TextureLayerID2, 1);

	services->setPixelShaderConstant("minBlur", &currmBlur, 1);
	services->setPixelShaderConstant("maxBlur", &currMBlur, 1);
}

void GammaShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("Texture0", &TextureLayerID, 1);

	services->setPixelShaderConstant("invGamma", &invGamma, 1);
}

#if 1
void NormalsShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
    irr::video::IVideoDriver* driver = services->getVideoDriver();

    if (driver->currentlyRenderedNode->getType()==irr::scene::ESCENE_NODE_TYPE::ESNT_ANIMATED_MESH) {
        irr::scene::ISkinnedMesh* mesh = (irr::scene::ISkinnedMesh*)(((irr::scene::IAnimatedMeshSceneNode*)(driver->currentlyRenderedNode))->getMesh());
        irr::f32* JointArray = new irr::f32[mesh->getAllJoints().size() * 16];

        int copyIncrement = 0;

        //std::cout<<"fffff "<<mesh->getAllJoints().size()<<"\n";
        for(int i = 0;i < mesh->getAllJoints().size();++i)
        {
            irr::core::matrix4 JointVertexPull(irr::core::matrix4::EM4CONST_NOTHING);
            JointVertexPull.setbyproduct(
                mesh->getAllJoints()[i]->GlobalAnimatedMatrix,
                mesh->getAllJoints()[i]->GlobalInversedMatrix);

            for(int j = 0;j < 16;++j) {
                JointArray[copyIncrement+j] = JointVertexPull[j];
            }
            copyIncrement += 16;
        }

        services->setVertexShaderConstant("uBone", JointArray, mesh->getAllJoints().size() * 16);

        delete[] JointArray; //TODO: should probably be using a container class that doesn't get wiped until the next pass
    }

    SharedShaderCallBack::setFogConstants(services);

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("baseMap", &TextureLayerID, 1);
	irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("normalMap", &TextureLayerID2, 1);
	irr::s32 TextureLayerID3 = 2;
	services->setPixelShaderConstant("specularMap", &TextureLayerID3, 1);
	irr::s32 TextureLayerID4 = 3;
	services->setPixelShaderConstant("fogTexture", &TextureLayerID4, 1);
	irr::s32 TextureLayerID5 = 4;
	services->setPixelShaderConstant("diffuseLight", &TextureLayerID5, 1);
	irr::s32 TextureLayerID6 = 5;
	services->setPixelShaderConstant("specularLight", &TextureLayerID6, 1);
	services->setPixelShaderConstant("renderSpecularFactor", &LightsShaderCallBack::renderSpecularFactor, 1);
}
#endif

#if 0
void NormalsShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	irr::video::IVideoDriver* driver = services->getVideoDriver();

	irr::core::matrix4 invWorld = driver->getTransform(irr::video::ETS_WORLD);
	invWorld.makeInverse();

	irr::core::matrix4 worldViewProj;
	worldViewProj = driver->getTransform(irr::video::ETS_VIEW);
	worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);

	irr::u32 cnt = lightList.size();

	irr::core::matrix4 lightTransform = worldViewProj*invWorld;

    driver->currentlyRenderedNode->updateAbsolutePosition();
	sortLights(driver->currentlyRenderedNode->getAbsolutePosition());

	for (irr::u32 i=0;i<4;++i) {
        //std::cout<<lightList[i].dist<<"\n";
		std::string lightPosition = "lightPos";
		lightPosition+=std::to_string(i+1);
		std::string lightColor = "lightColor";
		lightColor+=std::to_string(i+1);

		sortHelper light;

		if (i<cnt) {
			light = lightList[i];
		} else {
			light.color = irr::video::SColorf(0.f,0.f,0.f,1.f);
			light.pos[0]=0.f; light.pos[1]=0.f;
			light.pos[2]=0.f; light.pos[3]=0.f;
		}

		light.color.a = 1.0f;

        irr::core::vector3df lightPos(light.pos[0],light.pos[1],light.pos[2]);
		lightTransform.transformVect(lightPos);
		light.pos[0]=lightPos.X; light.pos[1]=lightPos.Y; light.pos[2]=lightPos.Z;

		services->setVertexShaderConstant(lightPosition.c_str(), (float*)(light.pos), 4);
		services->setPixelShaderConstant(lightColor.c_str(), (float*)(&light.color), 4);
	}

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("baseMap", &TextureLayerID, 1);
	irr::s32 TextureLayerID1 = 1;
	services->setPixelShaderConstant("bumpMap", &TextureLayerID1, 1);
	irr::s32 TextureLayerID2 = 2;
	services->setPixelShaderConstant("specMap", &TextureLayerID2, 1);

	services->setPixelShaderConstant("ambientLight", (float*)(&SharedShaderCallBack::ambientLight), 4);
	services->setPixelShaderConstant("fogNear",&SharedShaderCallBack::fogNear,1);
	services->setPixelShaderConstant("fogFar",&SharedShaderCallBack::fogFar,1);
}
#endif

void LightsShaderCallBack::setLights(const std::vector<pointLight> &inList,unsigned int prioritize) {
	lightList.resize(inList.size());
	for (unsigned int i=0;i<lightList.size();i++) {
		sortHelper lig;
		//lig.light = inList[i];
		lig.color = inList[i].color;
		lig.pos[0] = inList[i].position.X;
		lig.pos[1] = inList[i].position.Y;
		lig.pos[2] = inList[i].position.Z;
		lig.pos[3] = inList[i].radius*inList[i].radius;
		lig.intensity = inList[i].intensity;
		lig.dist = 0;
		lig.prioritize = (i<prioritize);
		lightList[i] = lig;
	}
}

void LightsShaderCallBack::sortLights(irr::core::vector3df nodePos) {
	for (unsigned int i=0;i<lightList.size();i++) {
		irr::core::vector3df pos(lightList[i].pos[0],lightList[i].pos[1],lightList[i].pos[2]);
		lightList[i].dist = (nodePos-pos).getLengthSQ();
		if (lightList[i].prioritize) { lightList[i].dist-=15000.f; }
	}
	std::sort(lightList.begin(),lightList.end());
}

#if 1
void PlainLightShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
    irr::video::IVideoDriver* driver = services->getVideoDriver();

    if (driver->currentlyRenderedNode->getType()==irr::scene::ESCENE_NODE_TYPE::ESNT_ANIMATED_MESH) {
        irr::scene::ISkinnedMesh* mesh = (irr::scene::ISkinnedMesh*)(((irr::scene::IAnimatedMeshSceneNode*)(driver->currentlyRenderedNode))->getMesh());
        irr::f32* JointArray = new irr::f32[mesh->getAllJoints().size() * 16];

        int copyIncrement = 0;

        //std::cout<<"fffff "<<mesh->getAllJoints().size()<<"\n";
        for(int i = 0;i < mesh->getAllJoints().size();++i)
        {
            irr::core::matrix4 JointVertexPull(irr::core::matrix4::EM4CONST_NOTHING);
            JointVertexPull.setbyproduct(
                mesh->getAllJoints()[i]->GlobalAnimatedMatrix,
                mesh->getAllJoints()[i]->GlobalInversedMatrix);

            for(int j = 0;j < 16;++j) {
                JointArray[copyIncrement+j] = JointVertexPull[j];
            }
            copyIncrement += 16;
        }

        services->setVertexShaderConstant("uBone", JointArray, mesh->getAllJoints().size() * 16); //TODO: should probably be using a container class that doesn't get wiped until the next pass

        delete[] JointArray;
    }

    SharedShaderCallBack::setFogConstants(services);

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("baseMap", &TextureLayerID, 1);
	irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("fogTexture", &TextureLayerID2, 1);
	irr::s32 TextureLayerID3 = 2;
	services->setPixelShaderConstant("diffuseLight", &TextureLayerID3, 1);
	irr::s32 TextureLayerID4 = 3;
	services->setPixelShaderConstant("specularLight", &TextureLayerID4, 1);
	services->setPixelShaderConstant("renderSpecularFactor", &LightsShaderCallBack::renderSpecularFactor, 1);
}
#endif

#if 0
void PlainLightShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	irr::video::IVideoDriver* driver = services->getVideoDriver();

	irr::core::matrix4 invWorld = driver->getTransform(irr::video::ETS_WORLD);
	invWorld.makeInverse();

	irr::core::matrix4 worldViewProj;
	worldViewProj = driver->getTransform(irr::video::ETS_VIEW);
	worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);

	irr::u32 cnt = lightList.size();

	irr::core::matrix4 lightTransform = worldViewProj*invWorld;

    driver->currentlyRenderedNode->updateAbsolutePosition();
	sortLights(driver->currentlyRenderedNode->getAbsolutePosition());

	for (irr::u32 i=0;i<4;++i) {
		std::string lightPosition = "lightPos";
		lightPosition+=std::to_string(i+1);
		std::string lightColor = "lightColor";
		lightColor+=std::to_string(i+1);

		sortHelper light;

		if (i<cnt) {
			light = lightList[i];
		} else {
			light.color = irr::video::SColorf(0.f,0.f,0.f,1.f);
			light.pos[0]=0.f; light.pos[1]=0.f;
			light.pos[2]=0.f; light.pos[3]=0.f;
		}

		light.color.a = 1.0f;

        irr::core::vector3df lightPos(light.pos[0],light.pos[1],light.pos[2]);
		lightTransform.transformVect(lightPos);
		light.pos[0]=lightPos.X; light.pos[1]=lightPos.Y; light.pos[2]=lightPos.Z;

		services->setVertexShaderConstant(lightPosition.c_str(), (float*)(light.pos), 4);
		services->setPixelShaderConstant(lightColor.c_str(), (float*)(&light.color), 4);
	}

	if (driver->currentlyRenderedNode->getType()==irr::scene::ESCENE_NODE_TYPE::ESNT_ANIMATED_MESH) {
        irr::scene::ISkinnedMesh* mesh = (irr::scene::ISkinnedMesh*)(((irr::scene::IAnimatedMeshSceneNode*)(driver->currentlyRenderedNode))->getMesh());
        irr::f32* JointArray = new irr::f32[mesh->getAllJoints().size() * 16];

        int copyIncrement = 0;

        //std::cout<<"fffff "<<mesh->getAllJoints().size()<<"\n";
        for(int i = 0;i < mesh->getAllJoints().size();++i)
        {
            irr::core::matrix4 JointVertexPull(irr::core::matrix4::EM4CONST_NOTHING);
            JointVertexPull.setbyproduct(
                mesh->getAllJoints()[i]->GlobalAnimatedMatrix,
                mesh->getAllJoints()[i]->GlobalInversedMatrix);

            for(int j = 0;j < 16;++j) {
                JointArray[copyIncrement+j] = JointVertexPull[j];
            }
            copyIncrement += 16;
        }

        services->setVertexShaderConstant("uBone", JointArray, mesh->getAllJoints().size() * 16); //TODO: should probably be using a container class that doesn't get wiped until the next pass

        delete[] JointArray;
    }

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("baseMap", &TextureLayerID, 1);

	services->setPixelShaderConstant("ambientLight", (float*)(&SharedShaderCallBack::ambientLight), 4);

	SharedShaderCallBack::setFogConstants(services);

	irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("fogTexture", &TextureLayerID2, 1);
}
#endif

#if 0
void PlainLightShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
	irr::video::IVideoDriver* driver = services->getVideoDriver();

	irr::core::matrix4 invWorld = driver->getTransform(irr::video::ETS_WORLD);
	invWorld.makeInverse();

	irr::core::matrix4 worldViewProj;
	worldViewProj = driver->getTransform(irr::video::ETS_VIEW);
	worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);

	irr::u32 cnt = lightList.size();

	irr::core::matrix4 lightTransform = worldViewProj*invWorld;

	//sortLights(lightTransform);

	//for (irr::u32 i=0;i<4;++i) {
    std::string lightPosition = "lightPos1";
    std::string lightColor = "lightColor1";

    services->setVertexShaderConstant("lightMatrix", (lightMatrix*driver->getTransform(irr::video::ETS_WORLD)).pointer(), 16);

    sortHelper light = lightList[pass];

    light.color.a = 1.0f;

    /*light.color.r *= light.intensity;
    light.color.g *= light.intensity;
    light.color.b *= light.intensity;*/

    irr::core::vector3df lightPos(light.pos[0],light.pos[1],light.pos[2]);
    lightTransform.transformVect(lightPos);
    light.pos[0]=lightPos.X; light.pos[1]=lightPos.Y; light.pos[2]=lightPos.Z;
    //light.pos[3]*=100.f;

    services->setVertexShaderConstant(lightPosition.c_str(), (float*)(light.pos), 4);
    services->setPixelShaderConstant(lightColor.c_str(), (float*)(&light.color), 4);
	//\}

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("renderedLights", &TextureLayerID, 1);
	irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("lightDepthMap", &TextureLayerID2, 1);

	services->setPixelShaderConstant("ambientLight", (float*)(&SharedShaderCallBack::ambientLight), 4);

	float TBN[3];
	TBN[0] = useTBN.X; TBN[1] = useTBN.Y; TBN[2] = useTBN.Z;
	services->setPixelShaderConstant("useTBN", TBN, 3);
}
#endif

void RenderDeferredLightShaderCallBack::OnSetConstants(irr::video::IMaterialRendererServices* services,irr::s32 userData) {
    irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("specularFactor", &TextureLayerID, 0);
    irr::s32 TextureLayerID2 = 1;
	services->setPixelShaderConstant("normalInWorld", &TextureLayerID2, 1);
	irr::s32 TextureLayerID3 = 2;
	services->setPixelShaderConstant("positionInWorld", &TextureLayerID3, 1);

    services->setPixelShaderConstant("lightPos",LightsShaderCallBack::lightList[lightToRender].pos,4);
    services->setPixelShaderConstant("lightColor",(float*)(&LightsShaderCallBack::lightList[lightToRender].color), 4);

    float camPos[3];
    SharedShaderCallBack::cameraPos.getAs3Values(camPos);
    services->setVertexShaderConstant("cameraPos",camPos,3);
}
