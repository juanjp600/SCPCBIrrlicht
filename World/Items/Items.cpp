#include "../World.h"
#include "Items.h"

Item::Item() {}

irrDynamics* Item::dynamics = nullptr;
Player* Item::mainPlayer = nullptr;
irr::video::IVideoDriver* Item::irrDriver = nullptr;
unsigned short Item::screenWidth = 0;
unsigned short Item::screenHeight = 0;

void Item::setDynamics(irrDynamics* dyn) {
    if (Item::dynamics == nullptr) {
        Item::dynamics = dyn;
    }
}

void Item::setPlayer(Player* inPlayer) {
    if (Item::mainPlayer == nullptr) {
        Item::mainPlayer = inPlayer;
    }
}

void Item::setDriver(irr::video::IVideoDriver* inDriver) {
    if (Item::irrDriver == nullptr) {
        Item::irrDriver = inDriver;
    }
}

void Item::setDimensions(unsigned short width,unsigned short height) {
    Item::screenWidth = width;
    Item::screenHeight = height;
}

irr::core::aabbox3df Item::getBBox() {
	return irrObj->getBoundingBox();
}

irr::core::matrix4 Item::getTransform() {
	return irrObj->getAbsoluteTransformation();
}

irr::core::vector3df Item::getPosition() {
    irrObj->updateAbsolutePosition();
    return irrObj->getAbsolutePosition();
}

void Item::pick() {
    if (!picked) {
        //rbody->translate(-rbody->getCenterOfMassPosition()+btVector3(0,1000000.0*RoomScale,0));
        irrObj->setVisible(false);
        dynamics->unregisterRBody(rbody);
        rbody->setLinearVelocity(btVector3(0,0,0)); rbody->setAngularVelocity(btVector3(0,0,0));
        rbody->setLinearFactor(btVector3(0,0,0)); rbody->setAngularFactor(btVector3(0,0,0));
        rbody->forceActivationState(DISABLE_SIMULATION);
        picked = true;
    }
}

void Item::unpick(irr::core::vector3df position) {
    if (picked) {
        rbody->translate(-rbody->getCenterOfMassPosition()+btVector3(position.X,position.Y,position.Z));
        irrObj->setVisible(true);
        dynamics->registerNewRBody(irrObj,rbody,-1,2,2,getOffset());
        rbody->setLinearVelocity(btVector3(0,0,0)); rbody->setAngularVelocity(btVector3(0,0,0));
        rbody->setLinearFactor(btVector3(1,1,1)); rbody->setAngularFactor(btVector3(1,1,1));
        rbody->forceActivationState(ACTIVE_TAG); rbody->activate();
        picked = false;
    }
}

void Item::createShapeFromNode(irr::scene::IMeshSceneNode* node,btConvexHullShape* &outShape,irr::core::vector3df &offset) {
	//get the mesh
	irr::scene::IMesh* irrMesh = node->getMesh();

	//generate the collision shape
	irr::core::vector3df scaling = node->getScale();//*irr::core::vector3df(0.85f,0.85f,0.85f);
	irr::u32 i,k,numVertices,precLoss;
	btConvexHullShape* dynShape = new btConvexHullShape();

	float mx,Mx,my,My,mz,Mz;
	Mx = My = Mz = -INFINITY;
	mx = my = mz = INFINITY;

	irr::u32 bufferCount = irrMesh->getMeshBufferCount();
	for (i=0; i<bufferCount; i++) //get mesh boundaries
	{
		irr::scene::IMeshBuffer* mb=irrMesh->getMeshBuffer(i);
		if(mb->getVertexType()==irr::video::EVT_STANDARD)
		{
			irr::video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)mb->getVertices();
			//mb_indices = mb->getIndices();
			numVertices = mb->getVertexCount();

			precLoss = 1;

			while (numVertices/precLoss>100) {
				precLoss++;
			}

			for (k=0;k<numVertices;k+=precLoss) {
				irr::core::vector3df irrPos2 = mb_vertices[k].Pos;

				mx = std::min(mx,irrPos2.X * scaling.X);
				Mx = std::max(Mx,irrPos2.X * scaling.X);
				my = std::min(my,irrPos2.Y * scaling.Y);
				My = std::max(My,irrPos2.Y * scaling.Y);
				mz = std::min(mz,irrPos2.Z * scaling.Z);
				Mz = std::max(Mz,irrPos2.Z * scaling.Z);
			}
		}
		else if(mb->getVertexType()==irr::video::EVT_2TCOORDS)
		{
			irr::video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
			numVertices = mb->getVertexCount();

			precLoss = 1;

			while (numVertices/precLoss>100) {
				precLoss++;
			}

			for (k=0;k<numVertices;k+=precLoss) {
				irr::core::vector3df irrPos2 = mb_vertices[k].Pos;

				mx = std::min(mx,irrPos2.X * scaling.X);
				Mx = std::max(Mx,irrPos2.X * scaling.X);
				my = std::min(my,irrPos2.Y * scaling.Y);
				My = std::max(My,irrPos2.Y * scaling.Y);
				mz = std::min(mz,irrPos2.Z * scaling.Z);
				Mz = std::max(Mz,irrPos2.Z * scaling.Z);
			}
		}
		else if(mb->getVertexType()==irr::video::EVT_TANGENTS)
		{
			irr::video::S3DVertexTangents* mb_vertices=(irr::video::S3DVertexTangents*)mb->getVertices();
			numVertices = mb->getVertexCount();

			precLoss = 1;

			while (numVertices/precLoss>100) {
				precLoss++;
			}

			for (k=0;k<numVertices;k+=precLoss) {
				irr::core::vector3df irrPos2 = mb_vertices[k].Pos;

				mx = std::min(mx,irrPos2.X * scaling.X);
				Mx = std::max(Mx,irrPos2.X * scaling.X);
				my = std::min(my,irrPos2.Y * scaling.Y);
				My = std::max(My,irrPos2.Y * scaling.Y);
				mz = std::min(mz,irrPos2.Z * scaling.Z);
				Mz = std::max(Mz,irrPos2.Z * scaling.Z);
			}
		}
	}

	offset.X = (mx+Mx)/2.f;
	offset.Y = (my+My)/2.f;
	offset.Z = (mz+Mz)/2.f;

	//float ccdThreshold = std::min(Mx-mx,std::min(My-my,Mz-mz));

	for (i=0; i<bufferCount; i++) //set offset to vertices
	{
		irr::scene::IMeshBuffer* mb=irrMesh->getMeshBuffer(i);
		if(mb->getVertexType()==irr::video::EVT_STANDARD)
		{
			irr::video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)mb->getVertices();
			numVertices = mb->getVertexCount();

			precLoss = 1;

			while (numVertices/precLoss>100) {
				precLoss++;
			}

			for (k=0;k<numVertices;k+=precLoss) {
				irr::core::vector3df irrPos2 = mb_vertices[k].Pos;

				btVector3 btPos2((irrPos2.X * scaling.X)-offset.X, (irrPos2.Y * scaling.Y)-offset.Y, (irrPos2.Z * scaling.Z)-offset.Z);
				dynShape->addPoint(btPos2);
			}
		}
		else if(mb->getVertexType()==irr::video::EVT_2TCOORDS)
		{
			irr::video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
			numVertices = mb->getVertexCount();

			precLoss = 1;

			while (numVertices/precLoss>100) {
				precLoss++;
			}

			for (k=0;k<numVertices;k+=precLoss) {
				irr::core::vector3df irrPos2 = mb_vertices[k].Pos;

				btVector3 btPos2((irrPos2.X * scaling.X)-offset.X, (irrPos2.Y * scaling.Y)-offset.Y, (irrPos2.Z * scaling.Z)-offset.Z);
				dynShape->addPoint(btPos2);
			}
		}
		else if(mb->getVertexType()==irr::video::EVT_TANGENTS)
		{
			irr::video::S3DVertexTangents* mb_vertices=(irr::video::S3DVertexTangents*)mb->getVertices();
			numVertices = mb->getVertexCount();

			precLoss = 1;

			while (numVertices/precLoss>100) {
				precLoss++;
			}

			for (k=0;k<numVertices;k+=precLoss) {
				irr::core::vector3df irrPos2 = mb_vertices[k].Pos;

				btVector3 btPos2((irrPos2.X * scaling.X)-offset.X, (irrPos2.Y * scaling.Y)-offset.Y, (irrPos2.Z * scaling.Z)-offset.Z);
				dynShape->addPoint(btPos2);
			}
		}
	}

	outShape = dynShape;
	outShape->setMargin(0.01f);
}

void Item::loadAssets(irr::scene::IMeshSceneNode* node,btConvexHullShape* shape) {
	irrObj = node->clone();

    //Add the Bullet rigid body
    irrObj->updateAbsolutePosition();
	irr::core::vector3df irrPos = irrObj->getAbsolutePosition();
	btVector3 btPos(irrPos.X, irrPos.Y, irrPos.Z);
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(btPos);

	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

    btVector3 localInertia;
	shape->calculateLocalInertia(20.0, localInertia);

    rbody = new btRigidBody(20.0, MotionState, shape, localInertia);
    rbody->setSleepingThresholds(2.f,2.f);

	rbody->setCcdMotionThreshold(0.01f);
	rbody->setCcdSweptSphereRadius(0.06f);

    irrObj->setVisible(false);
}

irr::scene::IMeshSceneNode* World::genItemNode(const std::string &meshPath,const std::string &texPath,float scale) {
	irr::scene::IMeshSceneNode* node = irrSmgr->addMeshSceneNode(irrSmgr->getMesh(meshPath.c_str()));

	if (texPath!="") {
		irr::video::ITexture* tex = irrDriver->getTexture(texPath.c_str());
		node->setMaterialTexture(0,tex);
	}

    node->setMaterialType(plainLightShader);

    node->setScale(irr::core::vector3df(scale,scale,scale));
    node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

    return node;
}
