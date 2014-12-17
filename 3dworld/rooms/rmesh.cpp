#include "rmesh.h"
#include "../3dworld.h"

#include <BulletCollision/CollisionDispatch/btInternalEdgeUtility.h>

RMesh* loadRMesh(std::string path,irr::io::IFileSystem* fs,irr::video::IVideoDriver* driver,irr::video::ITexture* reflection,irr::video::E_MATERIAL_TYPE RoomShader) {

    irr::io::IReadFile* file = fs->createAndOpenFile(path.c_str());

	if (!file) { std::cout<<"Could not load \""<<path<<"\""<<"\n"; return nullptr; }

    int readInt1;//,readInt2;
    int i1,i2,i3;
    unsigned char readChar1,readChar2,readChar3;
    unsigned char cr,cg,cb;
    float readFloat1,readFloat2;
    float fx,fy,fz;
    float fpitch,fyaw,froll;
    float fu1,fv1,fu2,fv2;

    std::string readString1,readString2,readString3;

    struct tempWaypoint {
		irr::core::vector3df pos;
		unsigned char connected[10];
		unsigned char id;
    };

	std::vector<tempWaypoint> tempWaypoints;
	tempWaypoint newWP;

    readString1 = "";
    //int = 4 octets
    //char = 1 octet
    //float = 4 octets
    //string = int + char * value_of_int

    /*file->read(&readInt1,sizeof(int));
    for (int i=0;i<readInt1;i++) {
        file->read(&readChar1,sizeof(unsigned char));
        readString1+=readChar1;
    }*/
	for (int i=0;i<3;i++) {
        file->read(&readChar1,sizeof(unsigned char));
        readString1+=readChar1;
    }
    if (readString1=="RM2") { //this is a valid RMesh
        std::cout<<"Loading \""<<path<<"\"\n";
        RMesh* retRMesh = new RMesh;

        retRMesh->path = path;
        int initialSize = path.size()-1;
        for (int i=initialSize;i>=0;i--) {
            if (path[i]!='\\' && path[i]!='/') {
                path.erase(i);
            } else {
                break;
            }
        }
        irr::scene::SMesh* mesh = new irr::scene::SMesh();
        irr::scene::SMeshBufferLightMap* bufLM = nullptr;
        irr::scene::SMeshBuffer* buf = nullptr;

        btTriangleMesh *pTriMesh = new btTriangleMesh();
		pTriMesh->m_weldingThreshold = 3.f;

		struct loadedTexture {
			irr::video::ITexture* tex;
			irr::video::ITexture* bump;
			unsigned char alpha;
		};

		std::vector<loadedTexture> loadedTextures;

		file->read(&readChar1,sizeof(unsigned char));
		for (unsigned char i=0;i<readChar1;i++) {
			readString1="";
			file->read(&readChar2,sizeof(unsigned char));
			for (unsigned char j=0;j<readChar2;j++) {
				file->read(&readChar3,sizeof(unsigned char));
				readString1+=readChar3;
			}
			loadedTexture newTex;
			newTex.tex = driver->getTexture((path+readString1).c_str());
			newTex.bump = nullptr;
			file->read(&newTex.alpha,sizeof(unsigned char));

            driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false); //mipmaps are not good for bump maps
			if (readString1=="tilefloor.jpg") {
				newTex.bump = driver->getTexture("GFX/map/tilebump.jpg");
			} else if (readString1=="concretefloor.jpg") {
				newTex.bump = driver->getTexture("GFX/map/concretefloorbump.jpg");
			} else if (readString1=="whitewall.jpg") {
				newTex.bump = driver->getTexture("GFX/map/whitewallbump.jpg");
			} else if (readString1=="metal3.jpg") {
				newTex.bump = driver->getTexture("GFX/map/metal3bump.jpg");
			} else if (readString1=="vent.jpg") {
				newTex.bump = driver->getTexture("GFX/map/ventbump.jpg");
			} else if (readString1=="dirtymetal.jpg") {
				newTex.bump = driver->getTexture("GFX/map/dirtymetalbump.jpg");
			} else if (readString1=="misc.jpg") {
				newTex.bump = driver->getTexture("GFX/map/miscbump.jpg");
			} else if (readString1=="rockmoss.jpg") {
				newTex.bump = driver->getTexture("GFX/map/rockmossbump.jpg");
			} else if (readString1=="metalpanels.jpg") {
				newTex.bump = driver->getTexture("GFX/map/metalpanelsbump.jpg");
			} else if (readString1=="metalpanels2.jpg") {
				newTex.bump = driver->getTexture("GFX/map/metalpanels2bump.jpg");
			} else if (readString1=="concretewall.jpg") {
				newTex.bump = driver->getTexture("GFX/map/concretewallbump.jpg");
			}
			driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, true);

			loadedTextures.push_back(newTex);
		}

		unsigned char entAmount = 0;
        file->read(&entAmount,sizeof(unsigned char));

        for (unsigned char i=0;i<entAmount;i++) {
            /*file->read(&readInt1,sizeof(int));
            readString1="";
            for (int j=0;j<readInt1;j++) {
                file->read(&readChar1,sizeof(unsigned char));
                readString1+=readChar1;
            }*/
            irr::video::SColorf lightColor; irr::video::SLight newLight;
            std::vector<irr::video::S3DVertex2TCoords> vertices;
			std::vector<irr::core::vector3di> indices;
            unsigned char entType;
            file->read(&entType,sizeof(unsigned char));
            char blendType = 0;
			unsigned char textures[2];
            switch (entType) {
				case 0: //solid, drawn
				case 1: //not solid, drawn
					file->read(&readChar3,sizeof(unsigned char));

					for (unsigned char k=0;k<readChar3;k++) {
						vertices.resize(0); indices.resize(0);
						blendType = 0;
						textures[0]=0; textures[1]=0;

						file->read(&readChar1,sizeof(unsigned char));
						if (readChar1 & 1) {
							file->read(&readChar2,sizeof(unsigned char));
							textures[0] = readChar2;

							if (blendType!=1) { //alpha blending is final
								blendType = 2;
								if (loadedTextures[textures[0]-1].alpha==3) blendType = 1;
							}
						}
						if (readChar1 & 2) {
							file->read(&readChar2,sizeof(unsigned char));
							textures[1] = readChar2;

							if (blendType!=1) { //alpha blending is final
								blendType = 2;
								if (loadedTextures[textures[1]-1].alpha==3) blendType = 1;
							}
						}

						file->read(&readInt1,sizeof(int));
						for (int j=0;j<readInt1;j++) { //vertices
							file->read(&fx,sizeof(float));
							file->read(&fy,sizeof(float));
							file->read(&fz,sizeof(float));

							file->read(&fu1,sizeof(float));
							file->read(&fv1,sizeof(float));
							file->read(&fu2,sizeof(float));
							file->read(&fv2,sizeof(float));

							file->read(&cr,sizeof(unsigned char));
							file->read(&cg,sizeof(unsigned char));
							file->read(&cb,sizeof(unsigned char));
							vertices.push_back(irr::video::S3DVertex2TCoords(fx*RoomScale*0.1f,fy*RoomScale*0.1f,fz*RoomScale*0.1f,0,0,0,irr::video::SColor(255,cr,cg,cb),fu1,fv1,fu2,fv2));
						}

						file->read(&readInt1,sizeof(int));
						for (int j=0;j<readInt1;j++) { //polys
							file->read(&i1,sizeof(int));
							file->read(&i2,sizeof(int));
							file->read(&i3,sizeof(int));

							indices.push_back(irr::core::vector3di(i1,i2,i3));

							float x1 = vertices[i1].Pos.X; float y1 = vertices[i1].Pos.Y; float z1 = vertices[i1].Pos.Z;
							float x2 = vertices[i2].Pos.X; float y2 = vertices[i2].Pos.Y; float z2 = vertices[i2].Pos.Z;
							float x3 = vertices[i3].Pos.X; float y3 = vertices[i3].Pos.Y; float z3 = vertices[i3].Pos.Z;

							float nx1 = (y3-y1)*(z2-z1)-(y2-y1)*(z3-z1);
							float ny1 = (z3-z1)*(x2-x1)-(z2-z1)*(x3-x1);
							float nz1 = (x3-x1)*(y2-y1)-(x2-x1)*(y3-y1);
							float fac1=sqrt((nx1*nx1)+(ny1*ny1)+(nz1*nz1));
							nx1 = nx1/fac1;
							ny1 = ny1/fac1;
							nz1 = nz1/fac1;

							vertices[i1].Normal.set(nx1,ny1,nz1);
							vertices[i2].Normal.set(nx1,ny1,nz1);
							vertices[i3].Normal.set(nx1,ny1,nz1);

							if (entType==0) {
								btVector3 btVertices[3];
								btVertices[0] = btVector3(vertices[i1].Pos.X,vertices[i1].Pos.Y,vertices[i1].Pos.Z);
								btVertices[1] = btVector3(vertices[i2].Pos.X,vertices[i2].Pos.Y,vertices[i2].Pos.Z);
								btVertices[2] = btVector3(vertices[i3].Pos.X,vertices[i3].Pos.Y,vertices[i3].Pos.Z);
								//faces that barely make a difference to the shape of the object are removed
								if (btVertices[0].distance(btVertices[1])>5.f || btVertices[1].distance(btVertices[2])>5.f || btVertices[0].distance(btVertices[2])>5.f) {
									pTriMesh->addTriangle(btVertices[0], btVertices[1], btVertices[2],true);
								}
							}
						}

						if (indices.size()!=0) {
							switch (blendType) {
								case 2: //opaque
									if (textures[0]!=0 && textures[1]!=0) { //lightmapped
										bufLM = new irr::scene::SMeshBufferLightMap();

										mesh->addMeshBuffer(bufLM);

										bufLM->drop();

										bufLM->Vertices.reallocate(vertices.size());
										bufLM->Vertices.set_used(vertices.size());

										for (unsigned int j=0;j<vertices.size();j++) {
											bufLM->Vertices[j]=vertices[j];
										}

										bufLM->Indices.reallocate(indices.size()*3);
										bufLM->Indices.set_used(indices.size()*3);
										for (unsigned int j=0;j<indices.size();j++) {
											bufLM->Indices[j*3]=indices[j].X;
											bufLM->Indices[(j*3)+1]=indices[j].Y;
											bufLM->Indices[(j*3)+2]=indices[j].Z;
										}

										bufLM->getMaterial().setTexture(0,loadedTextures[textures[1]-1].tex);
										bufLM->getMaterial().setTexture(1,loadedTextures[textures[0]-1].tex);

										bufLM->getMaterial().MaterialType = irr::video::EMT_LIGHTMAP;
										if (loadedTextures[textures[1]-1].bump!=nullptr) {
											bufLM->getMaterial().MaterialType = RoomShader;
											bufLM->getMaterial().setTexture(2,loadedTextures[textures[1]-1].bump);
											//for (int ri=0;ri<6;ri++) {
                                            bufLM->getMaterial().setTexture(3,reflection);
                                            //    std::cout<<(3+ri)<<" vs. "<<irr::video::MATERIAL_MAX_TEXTURES<<"\n";
                                            //}
										}
										/*if (textureRead[1]==path+"tilefloor.jpg") {
											bufLM->getMaterial().MaterialType = RoomShader;
											bufLM->getMaterial().setTexture(2,driver->getTexture("GFX/map/tilebump.jpg"));
										} else if (textureRead[1]==path+"concretefloor.jpg") {
											bufLM->getMaterial().MaterialType = RoomShader;
											bufLM->getMaterial().setTexture(2,driver->getTexture("GFX/map/concretefloorbump.jpg"));
										} else if (textureRead[1]==path+"whitewall.jpg") {
											bufLM->getMaterial().MaterialType = RoomShader;
											bufLM->getMaterial().setTexture(2,driver->getTexture("GFX/map/whitewallbump.jpg"));
										} else if (textureRead[1]==path+"metal3.jpg") {
											bufLM->getMaterial().MaterialType = RoomShader;
											bufLM->getMaterial().setTexture(2,driver->getTexture("GFX/map/metal3bump.jpg"));
										} else if (textureRead[1]==path+"vent.jpg") {
											bufLM->getMaterial().MaterialType = RoomShader;
											bufLM->getMaterial().setTexture(2,driver->getTexture("GFX/map/ventbump.jpg"));
										} else if (textureRead[1]==path+"dirtymetal.jpg") {
											bufLM->getMaterial().MaterialType = RoomShader;
											bufLM->getMaterial().setTexture(2,driver->getTexture("GFX/map/dirtymetalbump.jpg"));
										} else if (textureRead[1]==path+"misc.jpg") {
											bufLM->getMaterial().MaterialType = RoomShader;
											bufLM->getMaterial().setTexture(2,driver->getTexture("GFX/map/miscbump.jpg"));
										} else if (textureRead[1]==path+"rockmoss.jpg") {
											bufLM->getMaterial().MaterialType = RoomShader;
											bufLM->getMaterial().setTexture(2,driver->getTexture("GFX/map/rockmossbump.jpg"));
										} else if (textureRead[1]==path+"metalpanels.jpg") {
											bufLM->getMaterial().MaterialType = RoomShader;
											bufLM->getMaterial().setTexture(2,driver->getTexture("GFX/map/metalpanelsbump.jpg"));
										} else if (textureRead[1]==path+"metalpanels2.jpg") {
											bufLM->getMaterial().MaterialType = RoomShader;
											bufLM->getMaterial().setTexture(2,driver->getTexture("GFX/map/metalpanels2bump.jpg"));
										} else if (textureRead[1]==path+"concretewall.jpg") {
											bufLM->getMaterial().MaterialType = RoomShader;
											bufLM->getMaterial().setTexture(2,driver->getTexture("GFX/map/concretewallbump.jpg"));
										}*/

										bufLM->recalculateBoundingBox();
									} else { //only one texture
										buf = new irr::scene::SMeshBuffer();

										mesh->addMeshBuffer(buf);

										buf->drop();

										buf->Vertices.reallocate(vertices.size());
										buf->Vertices.set_used(vertices.size());

										for (unsigned int j=0;j<vertices.size();j++) {
											buf->Vertices[j]=vertices[j];
										}

										buf->Indices.reallocate(indices.size()*3);
										buf->Indices.set_used(indices.size()*3);
										for (unsigned int j=0;j<indices.size();j++) {
											buf->Indices[j*3]=indices[j].X;
											buf->Indices[(j*3)+1]=indices[j].Y;
											buf->Indices[(j*3)+2]=indices[j].Z;
										}

										if (textures[1]!=0) {
											buf->getMaterial().setTexture(0,loadedTextures[textures[1]-1].tex);
										} else {
											buf->getMaterial().setTexture(0,loadedTextures[textures[0]-1].tex);
										}

										buf->getMaterial().Lighting = false;

										buf->getMaterial().MaterialType = irr::video::EMT_SOLID;

										buf->recalculateBoundingBox();
									}
								break;
								case 1: //alpha
									buf = new irr::scene::SMeshBuffer();

									mesh->addMeshBuffer(buf);

									buf->drop();

									buf->Vertices.reallocate(vertices.size());
									buf->Vertices.set_used(vertices.size());

									for (unsigned int j=0;j<vertices.size();j++) {
										buf->Vertices[j]=vertices[j];
									}

									buf->Indices.reallocate(indices.size()*3);
									buf->Indices.set_used(indices.size()*3);
									for (unsigned int j=0;j<indices.size();j++) {
										buf->Indices[j*3]=indices[j].X;
										buf->Indices[(j*3)+1]=indices[j].Y;
										buf->Indices[(j*3)+2]=indices[j].Z;
									}

									if (textures[1]!=0) {
										buf->getMaterial().setTexture(0,loadedTextures[textures[1]-1].tex);
									} else {
										buf->getMaterial().setTexture(0,loadedTextures[textures[0]-1].tex);
									}
									//buf->getMaterial().setTexture(1,driver->getTexture(textureRead[0].c_str()));

									buf->getMaterial().Lighting = false;

									buf->getMaterial().MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;

									buf->getMaterial().BackfaceCulling = false;

									buf->recalculateBoundingBox();
								break;
							}
						}
					}
				break;
				case 2: //solid, not drawn
					//file->read(&readChar1,sizeof(unsigned char));

					//for (unsigned k=0;k<readChar1;k++) {
					{
						file->read(&readInt1,sizeof(int)); //vertices

						std::vector<btVector3> vertices;

						for (int j=0;j<readInt1;j++) {
							file->read(&fx,sizeof(float));
							file->read(&fy,sizeof(float));
							file->read(&fz,sizeof(float));

							fx*=RoomScale*0.1; fy*=RoomScale*0.1; fz*=RoomScale*0.1;
							vertices.push_back(btVector3(fx,fy,fz));
						}

						file->read(&readInt1,sizeof(int)); //polys
						for (int j=0;j<readInt1;j++) {
							file->read(&i1,sizeof(int));
							file->read(&i2,sizeof(int));
							file->read(&i3,sizeof(int));

							if (vertices[i1].distance(vertices[i2])>5.f || vertices[i2].distance(vertices[i3])>5.f || vertices[i1].distance(vertices[i3])>5.f) {
								pTriMesh->addTriangle(vertices[i1], vertices[i2], vertices[i3],true);
							}
						}
					}
					//\}
				break;
				case 3: //screen
					file->read(&fx,sizeof(float));
					file->read(&fy,sizeof(float));
					file->read(&fz,sizeof(float));

					file->read(&readChar2,sizeof(unsigned char));
					readString2="";
					for (unsigned char j=0;j<readChar2;j++) {
						file->read(&readChar1,sizeof(unsigned char));
						readString2+=readChar1;
					}
				break;
				case 4: //waypoint
					file->read(&fx,sizeof(float));
					file->read(&fy,sizeof(float));
					file->read(&fz,sizeof(float));

					newWP.pos = irr::core::vector3df(fx,fy,fz);

					file->read(&readChar1,sizeof(unsigned char));
					for (unsigned char j=0;j<10;j++) {
						newWP.connected[j]=0;
					}
					for (unsigned char j=0;j<readChar1;j++) {
						file->read(&readChar2,sizeof(unsigned char));
						newWP.connected[j]=readChar2;
					}
					tempWaypoints.push_back(newWP);
				break;
				case 5: //light
					file->read(&fx,sizeof(float));
					file->read(&fy,sizeof(float));
					file->read(&fz,sizeof(float));

					file->read(&fu1,sizeof(float)); //range

					cr=0;cg=0;cb=0;
					file->read(&cr,sizeof(unsigned char));
					file->read(&cg,sizeof(unsigned char));
					file->read(&cb,sizeof(unsigned char));

					file->read(&fv1,sizeof(float)); //intensity

					lightColor = irr::video::SColorf(float(cr)/255.f*fv1,float(cg)/255.f*fv1,float(cb)/255.f*fv1);
					newLight.DiffuseColor = lightColor;
					newLight.Radius = fu1*0.05f*RoomScale;
					newLight.Position = irr::core::vector3df(fx*0.1f*RoomScale,fy*0.1f*RoomScale,fz*0.1f*RoomScale);

					retRMesh->pointlights.push_back(newLight);
				break;
				case 6: //spotlight //TODO: add spotlights to the lighting shader
					file->read(&fx,sizeof(float));
					file->read(&fy,sizeof(float));
					file->read(&fz,sizeof(float));

					file->read(&fu1,sizeof(float)); //range

					cr=0;cg=0;cb=0;
					file->read(&cr,sizeof(unsigned char));
					file->read(&cg,sizeof(unsigned char));
					file->read(&cb,sizeof(unsigned char));

					file->read(&fv1,sizeof(float)); //intensity

					file->read(&fpitch,sizeof(float));
					file->read(&fyaw,sizeof(float));
					file->read(&froll,sizeof(float));

					file->read(&readFloat1,sizeof(float)); //inner cone angle
					file->read(&readFloat2,sizeof(float)); //outer cone angle
				break;
				case 7: //soundemitter"
					file->read(&fx,sizeof(float));
					file->read(&fy,sizeof(float));
					file->read(&fz,sizeof(float));

					file->read(&readChar2,sizeof(unsigned char));

					file->read(&fu1,sizeof(float)); //range
				break;
				default:
					std::cout<<"Unknown RMesh entity: \""<<(int)readChar1<<"\"\n";
				break;
			}
        }


		for (unsigned char i=0;i<tempWaypoints.size();i++) {
			RMesh::waypoint* finalWP;
			finalWP = new RMesh::waypoint;
			finalWP->position = tempWaypoints[i].pos;
			retRMesh->waypoints.push_back(finalWP);
		}

		for (unsigned char i=0;i<retRMesh->waypoints.size();i++) {
			for (unsigned char j=0;j<20;j++) {
				retRMesh->waypoints[i]->connected[j]=nullptr;
			}
			unsigned char shift = 0;
			while (retRMesh->waypoints[i]->connected[shift]!=nullptr) { shift++; }
			for (unsigned char j=0;j<10;j++) {
				if (tempWaypoints[i].connected[j+shift]==0) { break; }
				retRMesh->waypoints[i]->connected[j+shift]=retRMesh->waypoints[tempWaypoints[i].connected[j]-1];
				unsigned char shift2 = 0;
				while (retRMesh->waypoints[i]->connected[j+shift]->connected[shift2]!=nullptr) { shift2++; }
				retRMesh->waypoints[i]->connected[j+shift]->connected[shift2]=retRMesh->waypoints[i];
			}
		}

        mesh->recalculateBoundingBox();
        mesh->setHardwareMappingHint(irr::scene::EHM_STATIC);
        retRMesh->mesh = mesh;
        retRMesh->shape = new btBvhTriangleMeshShape(pTriMesh,true);
        retRMesh->shape->setMargin(1.0f);
        btTriangleInfoMap* triangleInfoMap = new btTriangleInfoMap();
        triangleInfoMap->m_edgeDistanceThreshold = 1.5f;
		btGenerateInternalEdgeInfo(retRMesh->shape, triangleInfoMap);

        file->drop();
        return retRMesh;

    } else {
        std::cout<<"\""<<path<<"\" isn't a valid RMesh";
    }

    file->drop();
    return nullptr;

}
