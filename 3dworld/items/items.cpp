#include "../3dworld.h"
#include "items.h"

/*itemtemplate::itemtemplate(const itemTempConstructParams &params) {
    name = params.name;
    id = params.id;
    color = params.color;
    //owner = caller;
    scale = params.scale;

    //get the mesh
    irrMesh = params.mesh;

    //generate the collision shape
    irr::core::vector3df scaling = irr::core::vector3df(scale,scale,scale);
	irr::u32 i,k,numVertices,precLoss;
	dynShape = new btConvexHullShape();

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

			while (numVertices/precLoss>250) {
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

			while (numVertices/precLoss>250) {
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

			while (numVertices/precLoss>250) {
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

	mOffset.X = (mx+Mx)/2.f;
	mOffset.Y = (my+My)/2.f;
	mOffset.Z = (mz+Mz)/2.f;

	ccdThreshold = std::min(Mx-mx,std::min(My-my,Mz-mz));

    for (i=0; i<bufferCount; i++) //set offset to vertices
	{
		irr::scene::IMeshBuffer* mb=irrMesh->getMeshBuffer(i);
		if(mb->getVertexType()==irr::video::EVT_STANDARD)
		{
			irr::video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)mb->getVertices();
			numVertices = mb->getVertexCount();

			precLoss = 1;

			while (numVertices/precLoss>250) {
                precLoss++;
			}

			for (k=0;k<numVertices;k+=precLoss) {
				irr::core::vector3df irrPos2 = mb_vertices[k].Pos;

				btVector3 btPos2((irrPos2.X * scaling.X)-mOffset.X, (irrPos2.Y * scaling.Y)-mOffset.Y, (irrPos2.Z * scaling.Z)-mOffset.Z);
				dynShape->addPoint(btPos2);
			}
		}
		else if(mb->getVertexType()==irr::video::EVT_2TCOORDS)
		{
			irr::video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
			numVertices = mb->getVertexCount();

			precLoss = 1;

			while (numVertices/precLoss>250) {
                precLoss++;
			}

			for (k=0;k<numVertices;k+=precLoss) {
				irr::core::vector3df irrPos2 = mb_vertices[k].Pos;

				btVector3 btPos2((irrPos2.X * scaling.X)-mOffset.X, (irrPos2.Y * scaling.Y)-mOffset.Y, (irrPos2.Z * scaling.Z)-mOffset.Z);
				dynShape->addPoint(btPos2);
			}
		}
		else if(mb->getVertexType()==irr::video::EVT_TANGENTS)
		{
			irr::video::S3DVertexTangents* mb_vertices=(irr::video::S3DVertexTangents*)mb->getVertices();
			numVertices = mb->getVertexCount();

			precLoss = 1;

			while (numVertices/precLoss>250) {
                precLoss++;
			}

			for (k=0;k<numVertices;k+=precLoss) {
				irr::core::vector3df irrPos2 = mb_vertices[k].Pos;

				btVector3 btPos2((irrPos2.X * scaling.X)-mOffset.X, (irrPos2.Y * scaling.Y)-mOffset.Y, (irrPos2.Z * scaling.Z)-mOffset.Z);
				dynShape->addPoint(btPos2);
			}
		}
	}
}*/

/*item::item(const world &owner) {
    //Template = params.temp;
    //name = params.name;
    //if (name=="") { name = params.temp->getName(); }
    for (int i=0;i<3;i++) { state[i]=0; }

    //irrObj = params.node;
    //rbody = params.rbody;

    irrObj = params.getNode(getTempID());
    rbody = params.getRBody(getTempID());
    offset = params.getOffset(getTempID());

    registerRBody = params.*registerRBody;
    unregisterRBody = params.*unregisterRBody;

    picked = true;
}*/
item::item() {}

dynRegister* item::dynamics = nullptr;
void item::setDynamics(dynRegister* dyn) {
    if (item::dynamics == nullptr) {
        item::dynamics = dyn;
    }
}

/*void item::Pick() {
    if (!picked) {
        rbody->translate(-rbody->getCenterOfMassPosition()+btVector3(0,1000000.0*RoomScale,0));
        irrObj->setVisible(false);
        unregisterRBody(rbody);
        picked = true;
    }
}

void item::Unpick(irr::core::vector3df position) {
    if (picked) {
        rbody->translate(-btVector3(0,1000000.0*RoomScale,0)+btVector3(position.X,position.Y,position.Z));
        irrObj->setVisible(true);
        registerRBody(irrObj,rbody,5.0,2,2,getOffset());
        picked = false;
    }
}*/

/*item* item::updateItem() {
    //TODO: add items
}*/
